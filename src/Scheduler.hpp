
#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <pthread.h>
#include <string>
#include <queue>

#ifndef SCHEDULER_SIMPLE_
#include <functional>
#else
#include <Util/stlfwd.hpp>
#endif

#include "Future.hpp"
#include "SchedulerThread.hpp"

namespace MCServer {

class Scheduler {
public:
    Scheduler(int asyncThreadCount = 4);

    pthread_t startThread(void * (*function)(void *), void *arg, const std::string &name = "", pthread_attr_t * = NULL);

    template <typename Ret, typename... Args>
    pthread_t startThread(const std::string &name, pthread_attr_t *, const std::function<Ret (Args...)> &func, Args&&... args);
    
    template <typename Ret, typename... Args>
    pthread_t startThread(const std::function<Ret (Args...)> &func, Args&&... args) {
        return startThread("", nullptr, func, std::forward(args...));
    }

    template <typename Ret, typename... Args>
    pthread_t startThread(const std::string &name, const std::function<Ret (Args...)> &func, Args&&... args) {
        return startThread(name, nullptr, func, std::forward<Args...>(args...));
    }


    template <typename Ret, typename... Args>
    Future<Ret> submitAsync(const std::function<Ret (Args...)> &func, Args&&... args);

    template <typename Ret, typename... Args>
    Future<Ret> submitSync(const std::function<Ret (Args...)> &func, Args&&... args);

    void tick(long millis = -1);
private:
    void executeSync(long millis);
    void executeSync(const std::function<void ()> &);
    SchedulerThread * getThread();

    std::queue<std::function<void ()>> asyncFunctions;
    std::queue<std::function<void ()>> syncFunctions;

    std::vector<SchedulerThread> threads;

    pthread_t mainThread;
};

}

#ifndef SCHEDULER_SIMPLE_
#include "Scheduler.tcc"
#endif

#endif
