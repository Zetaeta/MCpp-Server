
#include <errno.h>
#include <string.h>
#include <iostream>

#include "Lock.hpp"

using Util::MaybePointer;

namespace MCServer {

Lock::Lock()
:mutex(new pthread_mutex_t), locked(false) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

Lock::Lock(const MaybePointer<pthread_mutex_t> &mutex)
:mutex(mutex), locked(false) {}

void Lock::lock() {
    if (pthread_mutex_lock(mutex) == 0) {
        locked = true;
    }
    else {
        std::cerr << strerror(errno) << '\n';
    }
    // TODO: Proper error handling
}

void Lock::unLock() {
    if (pthread_mutex_unlock(mutex) == 0) {
        locked = false;
    }
    else {
        std::cerr << strerror(errno) << '\n';
    }
    // TODO: Proper error handling
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
        std::cerr << strerror(errno) << '\n';
        return false;
    }
    // TODO: Proper error handling
}

}
