
#ifndef LOCK_HPP
#define LOCK_HPP

#include <pthread.h>

#include <Util/MaybePointer.hpp>

namespace MCServer {

class Lock {
public:
    Lock(bool recursive = true);
    Lock(const Util::MaybePointer<pthread_mutex_t> &mutex);

    void lock();

    void unLock();

    bool tryLock();
    bool isLocked() {
        return locked;
    }

    pthread_mutex_t & getMutex() {
        return *mutex;
    }

private:
    Util::MaybePointer<pthread_mutex_t> mutex;
    volatile bool locked;
};

}


#endif
