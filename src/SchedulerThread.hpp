
#ifndef SCHEDULERTHREAD_HPP
#define SCHEDULERTHREAD_HPP

#include <functional>
#include <pthread.h>
#include <atomic>

#include "Condition.hpp"

namespace MCServer {

class Scheduler;

static int i = 0;

class SchedulerThread {
public:
    SchedulerThread()
    :started(false), actualStarted(false), busy(false), functionReady(false), i(MCServer::i++) {
    }

    void start(int id);

    void actualStart();

    bool isBusy() const {
        return busy;
    }

    bool isAvailable() const {
        return actualStarted && !busy;
    }

    bool isStarted() const {
        return started;
    }

    pthread_t getThreadId() const {
        return threadId;
    }

    void execute(const std::function<void ()> &function);

    void execute(std::function<void ()> &&function);

    Condition & getCondition() {
        return cond;
    }


private:
    void execute();
    Lock lock;

    pthread_t threadId;
    std::string name;
    std::atomic<bool> started;
    std::atomic<bool> actualStarted;
    std::atomic<bool> busy;
    std::atomic<bool> functionReady;
    Scheduler *scheduler;
    Condition cond;

    std::function<void ()> current;
    std::function<void ()> next;
public:
    int i;
};

}

#endif
