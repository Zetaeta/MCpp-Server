
#ifndef LOCK_HPP
#define LOCK_HPP

#include <pthread.h>

namespace MCServer {

class Lock {
public:
    Lock() {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
        pthread_mutex_init(&mutex, &attr);
        pthread_mutexattr_destroy(&attr);
    }

    void lock() {
        if (pthread_mutex_lock(&mutex)) {
            locked = true;
        }
        // TODO: Proper error handling
    }
    void unlock() {
        if (!pthread_mutex_unlock(&mutex)) {
            locked = false;
        }
        // TODO: Proper error handling
    }
    bool tryLock() {
        int ret = pthread_mutex_trylock(&mutex);
        if (ret == EBUSY) {
            return false;
        }
        else if (!ret) {
        locked = true;
        return true;
        }
        else {
            return false;
        }
        // TODO: Proper error handling
    }
    bool isLocked() {
        return locked;
    }
private:
    pthread_mutex_t mutex;
    volatile bool locked;
};

}


#endif
