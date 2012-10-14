
#include <Util/ErrorHandler.hpp>

#include "Condition.hpp"

using Util::throwException;
using Util::getMessage;

namespace MCServer {

void Condition::wait() {
    bool alreadyLocked = true;
    if (!lock->isLocked()) {
        alreadyLocked = false;
        lock->lock();
    }

    int returned = pthread_cond_wait(cond, &lock->getMutex());

    if (returned) {
        throwException(returned, ErrorType::PTHREAD_LOCK);
    }
    else if (!alreadyLocked) {
        lock->unLock();
    }
}

void Condition::signal() {
    int returned = pthread_cond_signal(cond);

    if (returned) {
        throwException(returned);
    }
}

void Condition::broadcast() {
    int returned = pthread_cond_signal(cond);

    if (!returned) {
        throwException(returned);
    }
}

}

