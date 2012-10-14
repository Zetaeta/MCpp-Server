
#ifndef SCHEDULERTHREAD_HPP
#define SCHEDULERTHREAD_HPP

#include <functional>
#include <pthread.h>

#include "Condition.hpp"

namespace MCServer {

class Scheduler;

class SchedulerThread {
public:
    SchedulerThread()
    :started(false), busy(false), functionReady(false) {
    }

    void start(int id);

    void actualStart();

    bool isBusy() const {
        return busy;
    }

    bool isAvailable() const {
        return started && !busy;
    }

    bool isStarted() const {
        return started;
    }

    void execute(std::function<void ()> &function) {
        toExecute = function;
        functionReady = true;
        cond.broadcast();
    }

    Condition & getCondition() {
        return cond;
    }

private:
    void execute();

    pthread_t threadId;
    std::string name;
    bool started;
    volatile bool busy;
    bool functionReady;
    Scheduler *scheduler;
    Condition cond;

    std::function<void ()> toExecute;
};

}

#endif
