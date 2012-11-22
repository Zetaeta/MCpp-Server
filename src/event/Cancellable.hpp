
#ifndef CANCELLABLE_HPP
#define CANCELLABLE_HPP

namespace MCServer {

namespace Events {

class Cancellable {
public:
    virtual void setCancelled(bool c) {
        cancelled = c;
    }

    virtual ~Cancellable() = default;

    virtual bool isCancelled() {
        return cancelled;
    }
private:
    bool cancelled;
};

}

}

#endif
