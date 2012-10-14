
#include <sstream>
#include <functional>
#include <iostream>

#include "SchedulerThread.hpp"
#include "Scheduler.hpp"

using std::cerr;

namespace MCServer {

void SchedulerThread::start(int id) {
    std::ostringstream oss;
    oss << "Scheduler-worker-";
    oss << id;
    name = oss.str();
    scheduler->startThread(name, std::function<void (SchedulerThread *)>(&SchedulerThread::actualStart), static_cast<SchedulerThread *>(this));
}

void SchedulerThread::actualStart() {
    while (true) {
        if (functionReady) {
            busy = true;
            toExecute();
            busy = false;
            functionReady = false;
        }
        cond.wait();
    }
}

void SchedulerThread::execute() {
    try {
        toExecute();
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
