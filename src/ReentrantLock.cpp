
#include <errno.h>
#include <string.h>
#include <iostream>

#include <Util/ErrorHandler.hpp>

#include "ReentrantLock.hpp"

using Util::MaybePointer;
namespace ErrorType = Util::ErrorType;

namespace MCServer {

ReentrantLock::ReentrantLock()
:mutex(new pthread_mutex_t), locked(false) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

ReentrantLock::ReentrantLock(const MaybePointer<pthread_mutex_t> &mutex)
:mutex(mutex), locked(false) {}

ReentrantLock::ReentrantLock(const ReentrantLock &other)
:mutex(other.mutex), locked(other.locked.load()) {}

void ReentrantLock::lock() {
    int returned = pthread_mutex_lock(mutex);
    if (returned == 0) {
        locked = true;
    }
    else {
        throwException(returned, ErrorType::PTHREAD_LOCK);
    }
}

void ReentrantLock::unLock() {
    int returned = pthread_mutex_unlock(mutex);
    if (returned == 0) {
        locked = false;
    }
    else {
        throwException(returned, ErrorType::PTHREAD_LOCK);
    }
}

bool ReentrantLock::tryLock() {
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
