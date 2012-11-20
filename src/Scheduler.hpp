
#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <pthread.h>
#include <string>
#include <queue>
#include <tuple>

#ifndef SCHEDULER_SIMPLE_
#include <functional>
#else
#include <Util/stlfwd.hpp>
#endif

#include "Future.hpp"
#include "SchedulerThread.hpp"

namespace MCServer {

using std::forward;
using std::string;
using std::function;
using std::is_convertible;
using std::enable_if;
using std::queue;
using std::vector;
using std::__and_;
using std::is_member_function_pointer;

class Scheduler {
public:
    Scheduler(int asyncThreadCount = 4);

    pthread_t startThread(void * (*function)(void *), void *arg, const string &name = "", pthread_attr_t * = NULL);

    template <typename Ret, typename... Args>
    inline pthread_t startThread(const string &name, pthread_attr_t *attr, const function<Ret (Args...)> &func, Args&&... args) {
        return startThreadImpl(name, attr, func, forward<Args>(args)...);
    }
    
    template <typename Ret, typename... Args>
    inline pthread_t startThread(const function<Ret (Args...)> &func, Args&&... args) {
        return startThread("", nullptr, func, forward<Args>(args)...);
    }

    template <typename Ret, typename... Args>
    inline pthread_t startThread(const string &name, const function<Ret (Args...)> &func, Args&&... args) {
        return startThread(name, nullptr, func, forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    inline auto startThread(Func &&func, Args&&... args) ->
        typename enable_if<
            is_convertible<
                Func, function<decltype(func(args...)) (Args...)>
            >::value, pthread_t
        >::type {
        typedef decltype(func(args...)) ReturnType;
        return startThreadImpl(function<ReturnType (Args...)>(func), forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    inline auto startThread(const string &name, Func &&func, Args&&... args) ->
        typename enable_if<
            is_convertible<
                Func, function<decltype(func(args...)) (Args...)>
            >::value, pthread_t
        >::type {
        typedef decltype(func(args...)) ReturnType;
        return startThreadImpl(name, function<ReturnType (Args...)>(func), forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    inline auto startThread(const string &name, pthread_attr_t *attr, Func &&func, Args&&... args) ->
        typename enable_if<
            is_convertible<
                Func, function<decltype(func(args...)) (Args...)>
            >::value, pthread_t
        >::type {
        typedef decltype(func(args...)) ReturnType;
        return startThreadImpl(name, attr, function<ReturnType (Args...)>(func), forward<Args>(args)...);
    }

    template <typename MemberFunc, typename Class, typename... Args>
    inline auto startThread(MemberFunc &&func, Class *obj, Args&&... args) ->
        typename enable_if<
            __and_<
                is_convertible<
                    MemberFunc, function<decltype((obj->*func)(args...)) (Args...)>
                >,
                is_member_function_pointer<MemberFunc>
            >::value, pthread_t
        >::type {
        typedef decltype((obj->*func)(args...)) ReturnType;
        return startThreadImpl<ReturnType, Class *, Args...>(function<ReturnType (Class *, Args...)>(func), std::move(obj), forward<Args>(args)...);
    }

    template <typename MemberFunc, typename Class, typename... Args>
    inline auto startThread(const string &name, MemberFunc &&func, Class *obj, Args&&... args) ->
        typename enable_if<
            __and_<
                is_convertible<
                    MemberFunc, function<decltype((obj->*func)(args...)) (Args...)>
                >,
                is_member_function_pointer<MemberFunc>
            >::value, pthread_t
        >::type {
        typedef decltype((obj->*func)(args...)) ReturnType;
        return startThreadImpl<ReturnType, Class *, Args...>(name, function<ReturnType (Args...)>(func), std::move(obj), forward<Args>(args)...);
    }

    template <typename MemberFunc, typename Class, typename... Args>
    inline auto startThread(const string &name, pthread_attr_t *attr, MemberFunc &&func, Class *obj, Args&&... args) ->
        typename enable_if<
            __and_<
                is_convertible<
                    MemberFunc, function<decltype((obj->*func)(args...)) (Args...)>
                >,
                is_member_function_pointer<MemberFunc>
            >::value, pthread_t
        >::type {
        typedef decltype((obj->*func)(args...)) ReturnType;
        return startThreadImpl<ReturnType, Class *, Args...>(name, attr, function<ReturnType (Args...)>(func), std::move(obj), forward<Args>(args)...);
    }


    template <typename Ret, typename... Args>
    inline pthread_t startImportantThread(const string &name, pthread_attr_t *attr, const function<Ret (Args...)> &func, Args&&... args) {
        return startImportantThreadImpl(name, attr, func, forward<Args>(args)...);
    }
    
    template <typename Ret, typename... Args>
    inline pthread_t startImportantThread(const function<Ret (Args...)> &func, Args&&... args) {
        return startImportantThreadImpl("", nullptr, func, forward<Args>(args)...);
    }

    template <typename Ret, typename... Args>
    inline pthread_t startImportantThread(const string &name, const function<Ret (Args...)> &func, Args&&... args) {
        return startImportantThreadImpl(name, nullptr, func, forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    inline auto startImportantThread(Func &&func, Args&&... args) ->
        typename enable_if<
            is_convertible<
                Func, function<decltype(func(args...)) (Args...)>
            >::value, pthread_t
        >::type {
        typedef decltype(func(args...)) ReturnType;
        return startImportantThreadImpl(function<ReturnType (Args...)>(func), forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    inline auto startImportantThread(const string &name, Func &&func, Args&&... args) ->
        typename enable_if<
            is_convertible<
                Func, function<decltype(func(args...)) (Args...)>
            >::value, pthread_t
        >::type {
        typedef decltype(func(args...)) ReturnType;
        return startImportantThreadImpl(name, function<ReturnType (Args...)>(func), forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    inline auto startImportantThread(const string &name, pthread_attr_t *attr, Func &&func, Args&&... args) ->
        typename enable_if<
            is_convertible<
                Func, function<decltype(func(args...)) (Args...)>
            >::value, pthread_t
        >::type {
        typedef decltype(func(args...)) ReturnType;
        return startImportantThreadImpl(name, attr, function<ReturnType (Args...)>(func), forward<Args>(args)...);
    }

    template <typename MemberFunc, typename Class, typename... Args>
    inline auto startImportantThread(MemberFunc &&func, Class *obj, Args&&... args) ->
        typename enable_if<
            __and_<
                is_convertible<
                    MemberFunc, function<decltype((obj->*func)(args...)) (Args...)>
                >,
                is_member_function_pointer<MemberFunc>
            >::value, pthread_t
        >::type {
        typedef decltype((obj->*func)(args...)) ReturnType;
        return startImportantThreadImpl<ReturnType, Class *, Args...>(function<ReturnType (Class *, Args...)>(func), std::move(obj), forward<Args>(args)...);
    }

    template <typename MemberFunc, typename Class, typename... Args>
    inline auto startImportantThread(const string &name, MemberFunc &&func, Class *obj, Args&&... args) ->
        typename enable_if<
            __and_<
                is_convertible<
                    MemberFunc, function<decltype((obj->*func)(args...)) (Args...)>
                >,
                is_member_function_pointer<MemberFunc>
            >::value, pthread_t
        >::type {
        typedef decltype((obj->*func)(args...)) ReturnType;
        return startImportantThreadImpl<ReturnType, Class *, Args...>(name, function<ReturnType (Class *, Args...)>(func), std::move(obj), forward<Args>(args)...);
    }

    template <typename MemberFunc, typename Class, typename... Args>
    inline auto startImportantThread(const string &name, pthread_attr_t *attr, MemberFunc &&func, Class *obj, Args&&... args) ->
        typename enable_if<
            __and_<
                is_convertible<
                    MemberFunc, function<decltype((obj->*func)(args...)) (Args...)>
                >,
                is_member_function_pointer<MemberFunc>
            >::value, pthread_t
        >::type {
        typedef decltype((obj->*func)(args...)) ReturnType;
        return startImportantThreadImpl<ReturnType, Class *, Args...>(name, attr, function<ReturnType (Class *, Args...)>(func), std::move(obj), forward<Args>(args)...);
    }


    template <typename Ret, typename... Args>
    Future<Ret> submitAsync(const function<Ret (Args...)> &func, Args&&... args) {
        return submitAsync(func, forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    inline auto submitAsync(Func &&func, Args&&... args) ->
        Future<
            typename enable_if<
                is_convertible<
                    Func, function<decltype(func(args...)) (Args...) >
                >::value ,  decltype(func(args...))
            >::type
        > {
        typedef decltype(func(args...)) ReturnType;
        return submitAsyncImpl(function<ReturnType (Args...)>(func), forward<Args>(args)...);
    }

    template <typename MemberFunc, typename Class, typename... Args>
    inline auto submitAsync(MemberFunc &&func, Class *obj, Args&&... args) ->
        Future<
            typename enable_if<
                __and_<
                    is_convertible<
                        MemberFunc, function<decltype((obj->*func)(args...)) (Args...) >
                    >,
                    is_member_function_pointer<MemberFunc>
                >::value,  decltype((obj->*func)(args...))
            >::type
        > {
        typedef decltype((obj->*func)(args...)) ReturnType;
        return submitAsyncImpl(function<ReturnType (Args...)>(func), obj, forward<Args>(args)...);
    }

    template <typename Ret, typename... Args>
    Future<Ret> submitSync(const function<Ret (Args...)> &func, Args&&... args) {
        return submitSync(func, forward<Args>(args)...);
    }

    template <typename Func, typename... Args>
    inline auto submitSync(Func &&func, Args&&... args) ->
        Future<
            typename enable_if<
                is_convertible<
                    Func, function<decltype(func(args...)) (Args...)>
                >::value, decltype(func())
            >::type
        > {
        typedef decltype(func(args...)) ReturnType;
        return submitSyncImpl(function<ReturnType (Args...)>(func), forward<Args>(args)...);
    }

    template <typename MemberFunc, typename Class, typename... Args>
    inline auto submitSync(MemberFunc &&func, Class *obj, Args&&... args) ->
        Future<
            typename enable_if<
                __and_<
                    is_convertible<
                        MemberFunc, function<decltype((obj->*func)(args...)) (Args...) >
                    >,
                    is_member_function_pointer<MemberFunc>
                >::value,  decltype((obj->*func)(args...))
            >::type
        > {
        typedef decltype((obj->*func)(args...)) ReturnType;
        return submitSyncImpl(function<ReturnType (Args...)>(func), obj, forward<Args>(args)...);
    }

    void tick(long millis = -1);
private:

    template <typename Ret, typename... Args>
    inline pthread_t startThreadImpl(const string &name, pthread_attr_t *, const function<Ret (Args...)> &func, Args&&... args);
    
    template <typename Ret, typename... Args>
    inline pthread_t startThreadImpl(const function<Ret (Args...)> &func, Args&&... args) {
        return startThreadImpl("", nullptr, func, forward<Args>(args)...);
    }

    template <typename Ret, typename... Args>
    inline pthread_t startThreadImpl(const string &name, const function<Ret (Args...)> &func, Args&&... args) {
        return startThreadImpl(name, nullptr, func, forward<Args>(args)...);
    }


    template <typename Ret, typename... Args>
    inline pthread_t startImportantThreadImpl(const string &name, pthread_attr_t *, const function<Ret (Args...)> &func, Args&&... args);
    
    template <typename Ret, typename... Args>
    inline pthread_t startImportantThreadImpl(const function<Ret (Args...)> &func, Args&&... args) {
        return startImportantThreadImpl("", nullptr, func, forward<Args>(args)...);
    }

    template <typename Ret, typename... Args>
    inline pthread_t startImportantThreadImpl(const string &name, const function<Ret (Args...)> &func, Args&&... args) {
        return startImportantThreadImpl(name, nullptr, func, forward<Args>(args)...);
    }


    template <typename Ret, typename... Args>
    Future<Ret> submitAsyncImpl(const function<Ret (Args...)> &func, Args&&... args);

    template <typename... Args>
    Future<void> submitAsyncImpl(const function<void (Args...)> &func, Args&&... args);
    

    template <typename Ret, typename... Args>
    Future<Ret> submitSyncImpl(const function<Ret (Args...)> &func, Args&&... args);

    template <typename... Args>
    Future<void> submitSyncImpl(const function<void (Args...)> &func, Args&&... args);


    void executeSync(long millis);
    void executeSync(const function<void ()> &);
    SchedulerThread * getThread();

    queue<function<void ()>> asyncFunctions;
    queue<function<void ()>> syncFunctions;

    vector<SchedulerThread> threads;

    pthread_t mainThread;
};

}

#ifndef SCHEDULER_SIMPLE_
#include "Scheduler.tcc"
#endif

#endif
