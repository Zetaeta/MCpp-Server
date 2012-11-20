
#ifndef REENTRANTLOCK_HPP
#define REENTRANTLOCK_HPP

#include <pthread.h>
#include <atomic>

#include <Util/MaybePointer.hpp>

namespace MCServer {

class ReentrantLock {
public:
    ReentrantLock();
    ReentrantLock(const Util::MaybePointer<pthread_mutex_t> &mutex);

    ReentrantLock(const ReentrantLock &other);

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
    std::atomic<bool> locked;
};

}


#endif
