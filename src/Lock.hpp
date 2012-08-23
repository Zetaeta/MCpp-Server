
#ifndef LOCK_HPP
#define LOCK_HPP

#include <pthread.h>

class Lock {
public:
    Lock()
    :mutex(PTHREAD_MUTEX_INITIALISER) {
        
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
        int ret = pthread_mutex_trylock(&mutex)
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

#endif
