
#include <sstream>
#include <iostream>
#include <algorithm>

#include <signal.h>

#include <Util/StringUtils.hpp>

#include "Scheduler.hpp"
#include "logging/Logger.hpp"

#include "util/Utils.hpp"

using std::string;
using std::ostringstream;
using std::cerr;

using Util::demangle;

namespace MCServer {

using Logging::log;

Scheduler::Scheduler(int asyncThreadCount)
:threads(asyncThreadCount) {
    mainThread = pthread_self();
}

struct ThreadStartInfo {
    void * (*function)(void *);
    void *argument;
    std::string name;
};

void * startThreadBeginner(void *threadStartInfo) {
    ThreadStartInfo *info = static_cast<ThreadStartInfo *>(threadStartInfo);
    string &name = info->name;
    if (name == "") {
        ostringstream oss;
        oss << pthread_self();
        name = oss.str();
    }

//    signal(SIGPIPE, SIG_IGN);

    try {
        (*info->function)(info->argument);
    }
    catch (std::exception &e) {
        cerr << "Unhandled exception in server (thread " << name << "): " << demangle(typeid(e).name()) << ": "<< e.what() << '\n';
    }
    catch (const char *str) {
        cerr << "Unhandled (const char *) exception in server (thread " << name << "): " << str << '\n';
    }
    catch (std::string &str) {
        cerr << "Unhandled (string) exception in server (thread " << name << "): " << str << '\n';
    }
    return nullptr;
}

pthread_t Scheduler::startThread(void *(*function)(void *), void *arg,
                                 const string &name, pthread_attr_t *attributes) {
    pthread_t threadId;
    ThreadStartInfo *info = new ThreadStartInfo {function, arg, name};
    pthread_create(&threadId, attributes, &startThreadBeginner, info);
    return threadId;
}

void Scheduler::tick(long milliseconds) {
    long beforeAsync = currentTimeMillis();
    while (asyncFunctions.size() > 0) {
        SchedulerThread *thread = getThread();
        if (thread) {
            thread->execute(asyncFunctions.front());
            asyncFunctions.pop();
        }
    }
    long afterAsync = currentTimeMillis();
    if (mainThread == pthread_self() && afterAsync - beforeAsync < milliseconds) {
        executeSync(milliseconds - (afterAsync - beforeAsync));
    }
}

SchedulerThread * Scheduler::getThread() {
    static int threadCount;
    auto it = find_if(threads.begin(), threads.end(), [] (const SchedulerThread &st) {
        return st.isAvailable();
    });
    if (it != threads.end()) {
        return &*it;
    }
    it = find_if(threads.begin(), threads.end(), [] (const SchedulerThread &st) {
        return !st.isStarted();
    });
    if (it != threads.end()) {
        it->start(threadCount++);
        return &*it;
    }
    return nullptr;
}

void Scheduler::executeSync(long milliseconds) {
    long start = currentTimeMillis();
    while (syncFunctions.size() > 0 && 
          (milliseconds < 0 || (currentTimeMillis() - start) < milliseconds)) {
        executeSync(syncFunctions.front());
        syncFunctions.pop();
    }
}

void Scheduler::executeSync(const std::function<void ()> &func) {
    try {
        func();
    }
    catch (std::exception &e) {
        cerr << "Unhandled exception in synchronous scheduler task: " << Util::demangle(typeid(e).name()) << ": "<< e.what() << '\n';
    }
    catch (const char *str) {
        cerr << "Unhandled (const char *) exception in synchronous scheduler task: " << str << '\n';
    }
    catch (string &str) {
        cerr << "Unhandled (string) exception in synchronous scheduler task: " << str << '\n';
    }
}

}
