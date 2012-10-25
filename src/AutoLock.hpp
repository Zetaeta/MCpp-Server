
#ifndef AUTOLOCK_HPP
#define AUTOLOCK_HPP

#include "Lock.hpp"

namespace MCServer {

class AutoLock {
public:
    inline AutoLock(Lock &lock)
    :lock(lock) {
        lock.lock();
    }

    inline ~AutoLock() {
        lock.unLock();
    }

private:
    Lock &lock;
};

}

#endif
