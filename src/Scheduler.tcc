
#include <iostream>
#include <sstream>

#include <Util/StringUtils.hpp>

namespace MCServer {

using std::move;
using std::forward;
using std::tuple;
using std::string;
using std::unique_ptr;
using std::function;
using std::tie;
using std::cout;

template <typename R>
struct GetSubmitReturnValue {
    static R * func(R &r) {
        return new R(r);
    }

    static const bool requiresDeletion = true;
};

template <typename R>
struct GetSubmitReturnValue<R &> {
    static R * func(R &r) {
        return &r;
    }

    static const bool requiresDeletion = false;
};

/**
 * Serves as a placeholder for passing the list of argument indices to callFunctionWithTuple
 */
template <int...>
struct PlaceHolder {};

template <int N, int... S>
struct ArgumentEnumerator : ArgumentEnumerator<N-1, N-1, S...> {};

template <int... S>
struct ArgumentEnumerator<0, S...> {
    typedef PlaceHolder<S...> type;
};

template <typename Ret, typename... Args, int... S>
Ret callFunctionWithTupleImpl(const function<Ret (Args...)> &func, tuple<Args...> &&args, PlaceHolder<S...>) {
    return func(std::get<S>(args)...);
}

template <typename Ret, typename... Args>
Ret callFunctionWithTuple(const function<Ret (Args...)> &func, tuple<Args...> &&arguments)  {
    return callFunctionWithTupleImpl(func, forward<decltype(arguments)>(arguments), typename ArgumentEnumerator<sizeof...(Args)>::type());
}


template <typename Ret, typename... Args>
Future<Ret> Scheduler::submitAsyncImpl(const function<Ret (Args...)> &func, Args&&... args) {
    tuple<Args...> arguments = tie(args...);
    Future<Ret> future;
    function<void ()> _func([=] () mutable {
        Ret returnVal = callFunctionWithTuple(func, forward<tuple<Args...>>(arguments));
        typedef GetSubmitReturnValue<Ret> RetVal;
        future.set(RetVal::func(returnVal), RetVal::requiresDeletion);
    });

    struct FuncWrapper {

        FuncWrapper(const function<Ret (Args...)> &func, const Future<Ret> &future, const tuple<Args...> &arguments, int i)
        :func(func), future(future), arguments(arguments), i(i) {
            cout << "Scheduler::submitAsync()::FuncWrapper::FuncWrapper(Future<Ret>, tuple<Args...>): " << i << '\n';
        }

        FuncWrapper(const FuncWrapper &f)
        :func(f.func), future(f.future), arguments(f.arguments), i(f.i) {
            cout << "Scheduler::submitAsync()::FuncWrapper::FuncWrapper(const FuncWrapper &): " << i << '\n';
        }

        FuncWrapper(FuncWrapper &&f)
        :func(f.func), future(f.future), arguments(f.arguments), i(f.i) {
            cout << "Scheduler::submitAsync()::FuncWrapper::FuncWrapper(FuncWrapper &&): " << i << '\n';
        }

        ~FuncWrapper() {
            cout << "Scheduler::submitAsync()::FuncWrapper::~FuncWrapper(): " << i << '\n';
        }

        void operator()() {
            Ret returnVal = callFunctionWithTuple(func, forward<tuple<Args...>>(arguments));
            typedef GetSubmitReturnValue<Ret> RetVal;
            future.set(RetVal::func(returnVal), RetVal::requiresDeletion);
        }
    private:
        function<Ret (Args...)> func;
        Future<Ret> future;
        tuple<Args...> arguments;
        int i;
    };

    asyncFunctions.push(_func);
//    static int i = 0;
//    FuncWrapper func_(func, future, arguments, i++);
//    asyncFunctions.push(func_);
    return future;
}


template <typename... Args>
Future<void> Scheduler::submitAsyncImpl(const function<void (Args...)> &func, Args&&... args) {
    tuple<Args...> arguments = tie(args...);
    Future<void> future;
    function<void ()> _func([=] () mutable {
        callFunctionWithTuple(func, forward<tuple<Args...>>(arguments));
        future.set();
    });

    struct FuncWrapper {

        FuncWrapper(const function<void (Args...)> &func, const Future<void> &future, const tuple<Args...> &arguments, int i)
        :func(func), future(future), arguments(arguments), i(i) {
            cout << "Scheduler::submitAsync()::FuncWrapper::FuncWrapper(Future<void>, tuple<Args...>): " << i << '\n';
        }

        FuncWrapper(const FuncWrapper &f)
        :func(f.func), future(f.future), arguments(f.arguments), i(f.i) {
            cout << "Scheduler::submitAsync()::FuncWrapper::FuncWrapper(const FuncWrapper &): " << i << '\n';
        }

        FuncWrapper(FuncWrapper &&f)
        :func(f.func), future(f.future), arguments(f.arguments), i(f.i) {
            cout << "Scheduler::submitAsync()::FuncWrapper::FuncWrapper(FuncWrapper &&): " << i << '\n';
        }

        ~FuncWrapper() {
            cout << "Scheduler::submitAsync()::FuncWrapper::~FuncWrapper(): " << i << '\n';
        }

        void operator()() {
            callFunctionWithTuple(func, forward<tuple<Args...>>(arguments));
            future.set();
        }
    private:
        function<void (Args...)> func;
        Future<void> future;
        tuple<Args...> arguments;
        int i;
    };

    asyncFunctions.push(_func);
//    static int i = 0;
//    FuncWrapper func_(func, future, arguments, i++);
//    asyncFunctions.push(func_);
    return future;
}

template <typename Ret, typename... Args>
Future<Ret> Scheduler::submitSyncImpl(const function<Ret (Args...)> &func, Args&&... args) {
    tuple<Args...> arguments = tie(args...);
    Future<Ret> future;
    syncFunctions.push([=] () mutable {
        Ret returnVal = callFunctionWithTuple(forward(func), forward(arguments));
        typedef GetSubmitReturnValue<Ret> RetVal;
        future.set(RetVal::func(returnVal), RetVal::requiresDeletion);
    });
    return future;
}

template <typename... Args>
Future<void> Scheduler::submitSyncImpl(const function<void (Args...)> &func, Args&&... args) {
    tuple<Args...> arguments = tie(args...);
    Future<void> future;
    syncFunctions.push([=] () mutable {
        callFunctionWithTuple(forward(func), forward(arguments));
        future.set();
    });
    return future;
}


template <typename Ret, typename... Args>
struct StartThreadData {
    std::function<Ret (Args...)> function;
    tuple<Args...> args;
    string name;
};


template <typename Ret, typename... Args>
void * templateStartThreadImpl(void *arg) {
    unique_ptr<StartThreadData<Ret, Args...>> data(static_cast<StartThreadData<Ret, Args...> *>(arg));
    string &name = data->name;
    if (name == "") {
        std::ostringstream oss;
        oss << pthread_self();
        name = oss.str();
    }
    try {
        callFunctionWithTuple(data->function, move(data->args));
    }
    catch (std::exception &e) {
        std::cerr << "Unhandled exception in server (thread " << name << "): " << Util::demangle(typeid(e).name()) << ": "<< e.what() << '\n';
    }
    catch (const char *str) {
        std::cerr << "Unhandled (const char *) exception in server (thread " << name << "): " << str << '\n';
    }
    catch (string &str) {
        std::cerr << "Unhandled (string) exception in server (thread " << name << "): " << str << '\n';
    }

    return nullptr;
}

template <typename Ret, typename... Args>
pthread_t Scheduler::startThreadImpl(const string &name, pthread_attr_t *attributes, const function<Ret (Args...)> &function, Args&&... args) {
    pthread_t threadId;
    pthread_create(&threadId, attributes, &templateStartThreadImpl<Ret, Args...>, new StartThreadData<Ret, Args...>{function, tie(args...), name});
    return threadId;
}

}

