
#include <errno.h>
#include <string.h>
#include <iostream>

#include <Util/ErrorHandler.hpp>

#include "Lock.hpp"

using Util::MaybePointer;
namespace ErrorType = Util::ErrorType;

namespace MCServer {

Lock::Lock()
:mutex(new pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER)) {}

Lock::Lock(const MaybePointer<pthread_mutex_t> &mutex)
:mutex(mutex) {}

void Lock::lock() {
    int returned = pthread_mutex_lock(mutex);
    if (returned != 0) {
        throwException(returned, ErrorType::PTHREAD_LOCK);
    }
}

void Lock::unLock() {
    int returned = pthread_mutex_unlock(mutex);
    if (returned != 0) {
        throwException(returned, ErrorType::PTHREAD_LOCK);
    }
}

bool Lock::tryLock() {
    int ret = pthread_mutex_trylock(mutex);
    if (ret == EBUSY) {
        return false;
    }
    else if (ret == 0) {
        return true;
    }
    else {
        throwException(ret, ErrorType::PTHREAD_LOCK);
        return false;
    }
}

bool Lock::isLocked() {
    int ret = pthread_mutex_trylock(mutex);
    if (ret == EBUSY) {
        return true;
    }
    else if (ret == 0) {
        if (int ret2 = pthread_mutex_unlock(mutex)) {
            throwException(ret2, ErrorType::PTHREAD_LOCK);
            return false;
        }
        return false;
    }
    else {
        throwException(ret, ErrorType::PTHREAD_LOCK);
        return true;
    }
}

}
