
#ifndef CONDITION_HPP
#define CONDITION_HPP

#include "Lock.hpp"

namespace MCServer {

class Condition {
public:
    Condition(const Util::MaybePointer<Lock> &lock = new Lock(false), const Util::MaybePointer<pthread_cond_t> &cond = new pthread_cond_t(PTHREAD_COND_INITIALIZER))
    :lock(lock), cond(cond) {}

    Condition(const Util::MaybePointer<pthread_cond_t> &cond)
    :lock(new Lock(false)), cond(cond) {}

    void wait();

    void signal();

    void broadcast();

private:
    Util::MaybePointer<Lock> lock;
    Util::MaybePointer<pthread_cond_t> cond;
};

}

#endif

