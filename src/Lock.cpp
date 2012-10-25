
#include <errno.h>
#include <string.h>
#include <iostream>

#include <Util/ErrorHandler.hpp>

#include "Lock.hpp"

using Util::MaybePointer;
namespace ErrorType = Util::ErrorType;

namespace MCServer {

Lock::Lock(bool recursive)
:mutex(new pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER)), locked(false) {
    if (recursive) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
        pthread_mutex_init(mutex, &attr);
        pthread_mutexattr_destroy(&attr);
    }
}

Lock::Lock(const MaybePointer<pthread_mutex_t> &mutex)
:mutex(mutex), locked(false) {}

void Lock::lock() {
    int returned = pthread_mutex_lock(mutex);
    if (returned == 0) {
        locked = true;
    }
    else {
        throwException(returned, ErrorType::PTHREAD_LOCK);
    }
}

void Lock::unLock() {
    int returned = pthread_mutex_unlock(mutex);
    if (returned == 0) {
        locked = false;
    }
    else {
        throwException(returned, ErrorType::PTHREAD_LOCK);
    }
}

bool Lock::tryLock() {
    int ret = pthread_mutex_trylock(mutex);
    if (ret == EBUSY) {
        return false;
    }
    else if (ret == 0) {
        locked = true;
        return true;
    }
    else {
        throwException(ret, ErrorType::PTHREAD_LOCK);
        return false;
    }
}

}
