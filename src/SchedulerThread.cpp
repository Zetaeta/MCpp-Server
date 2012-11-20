
#include <sstream>
#include <functional>
#include <iostream>

#include "SchedulerThread.hpp"
#include "Scheduler.hpp"
#include "AutoLock.hpp"
#include "logging/Logger.hpp"

using std::cerr;

namespace MCServer {

using Logging::log;

void SchedulerThread::start(int id) {
    started = true;
    log.lock();
    log.unLock();
    std::ostringstream oss;
    oss << "Scheduler-worker-";
    oss << id;
    name = oss.str();
    scheduler->startThread(name, std::function<void (SchedulerThread *)>(&SchedulerThread::actualStart), static_cast<SchedulerThread *>(this));
}

void SchedulerThread::actualStart() {
    actualStarted = true;
    while (true) {
        while (functionReady) {
            busy = true;
            functionReady = false;
            execute();
            busy = false;
        }
        cond.wait();
    }
}

void SchedulerThread::execute(const std::function<void ()> &function) {
    auto a = autoLock(lock);
    // New function is created around `function`
    // New function is swapped with `toExecute`
    // Both functions now have pointer to original func.
    // function reaches end of scope.
    // original func is deleted.
    next = function;
    functionReady = true;
    cond.broadcast();
}

void SchedulerThread::execute(std::function<void ()> &&function) {
    auto a = autoLock(lock);
    next = std::move(function);
    functionReady = true;
    cond.broadcast();
}

void SchedulerThread::execute() {
    auto a = autoLock(lock);
    swap(current, next);
    try {
        current();
    }
    catch (std::exception &e) {
        cerr << "Unhandled exception in asynchronous scheduler task, (thread = " << name << "): " << Util::demangle(typeid(e).name()) << ": "<< e.what() << '\n';
    }
    catch (const char *str) {
        cerr << "Unhandled (const char *) exception in asynchronous scheduler task (thread = " << name << "): " << str << '\n';
    }
    catch (string &str) {
        cerr << "Unhandled (string) exception in asynchronous scheduler task (thread = " << name << "): " << str << '\n';
    }
}

}
