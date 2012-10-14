
#ifndef FUTURE_HPP
#define FUTURE_HPP

#include <memory>

template <typename T>
class Future {
public:
    void set(T *object, bool toDelete) {
        impl->object = object;
        impl->toDelete = toDelete;
    }

    bool available() {
        return impl->object;
    }

    T get() {
        return *impl->object;
    }
private:
    struct Impl {
        T *object;
        bool toDelete;

        Impl()
        :object(nullptr) {}

        ~Impl() {
            if (toDelete) {
                delete object;
            }
        }
    };

    std::shared_ptr<Impl> impl;
};

#endif
