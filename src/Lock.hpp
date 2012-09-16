
#ifndef LOCK_HPP
#define LOCK_HPP


#include <pthread.h>

namespace MCServer {

class Lock {
public:
    Lock();

    void lock();

    void unLock();

    bool tryLock();
    bool isLocked() {
        return locked;
    }

    pthread_mutex_t & getMutex() {
        return mutex;
    }

private:
    pthread_mutex_t mutex;
    volatile bool locked;
};

}


#endif
