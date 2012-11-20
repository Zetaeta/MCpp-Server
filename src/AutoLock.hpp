
#ifndef AUTOLOCK_HPP
#define AUTOLOCK_HPP

namespace MCServer {

template <class L>
class AutoLock {
public:
    inline AutoLock(L &lock)
    :lock(lock), owns(true) {
        lock.lock();
    }

    inline AutoLock(const AutoLock<L> &other)
    :lock(other.lock), owns(true) {
        other.owns = false;
    }

    inline AutoLock(AutoLock<L> &&other)
    :lock(other.lock), owns(true) {
        other.owns = false;
    }

    inline ~AutoLock() {
        lock.unLock();
    }

private:
    L &lock;
    mutable bool owns;
};

template <class L>
inline AutoLock<L> autoLock(L &lock) {
    return AutoLock<L>(lock);
}

#define AUTOLOCK(lock) \
    volatile auto _auto_Lock_ = autoLock(lock)

}

#endif
