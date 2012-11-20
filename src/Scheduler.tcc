
#include <iostream>
#include <sstream>

#include <Util/StringUtils.hpp>

#include "util/ArgumentEnumerator.hpp"

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
 /
template <int...>
struct PlaceHolder {};

template <int N, int... S>
struct ArgumentEnumerator : ArgumentEnumerator<N-1, N-1, S...> {};

template <int... S>
struct ArgumentEnumerator<0, S...> {
    typedef PlaceHolder<S...> type;
};
*/

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

    asyncFunctions.push(_func);
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

    asyncFunctions.push(_func);
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
    bool important;
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
        if (data->important) {
            exit(1);
        }
    }
    catch (const char *str) {
        std::cerr << "Unhandled (const char *) exception in server (thread " << name << "): " << str << '\n';
        if (data->important) {
            exit(1);
        }
    }
    catch (string &str) {
        std::cerr << "Unhandled (string) exception in server (thread " << name << "): " << str << '\n';
        if (data->important) {
            exit(1);
        }
    }

    return nullptr;
}

template <typename Ret, typename... Args>
pthread_t Scheduler::startThreadImpl(const string &name, pthread_attr_t *attributes, const function<Ret (Args...)> &function, Args&&... args) {
    pthread_t threadId;
    pthread_create(&threadId, attributes, &templateStartThreadImpl<Ret, Args...>, new StartThreadData<Ret, Args...>{function, tie(args...), name, false});
    return threadId;
}

template <typename Ret, typename... Args>
pthread_t Scheduler::startImportantThreadImpl(const string &name, pthread_attr_t *attributes, const function<Ret (Args...)> &function, Args&&... args) {
    pthread_t threadId;
    pthread_create(&threadId, attributes, &templateStartThreadImpl<Ret, Args...>, new StartThreadData<Ret, Args...>{function, tie(args...), name, true});
    return threadId;
}

}

