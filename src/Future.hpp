
#ifndef FUTURE_HPP
#define FUTURE_HPP

#include <memory>
//#include <iostream>
#include <atomic>

#include "Lock.hpp"
#include "AutoLock.hpp"
#include "Condition.hpp"

namespace MCServer {

//using std::cout;

template <typename T>
class Future {
public:
    Future()
    :impl(new Impl), i(_i++) {
//        cout << "\tFuture::Future(): impl.use_count() = " << impl.use_count() << ", i = " << i << '\n';
    }

    Future(const Future &other)
    :impl(other.impl), i(other.i) {
//        cout << "\tFuture::Future(const Future &): impl.use_count() = " << impl.use_count() << ", i = " << i << '\n';
    }

    Future(Future &&other)
    :impl(std::move(other.impl)), i(other.i) {
//        cout << "\tFuture::Future(Future &&): impl.use_count() = " << impl.use_count() << ", i = " << i << '\n';
    }

    template <typename U>
    Future operator=(U t) = delete;

    Future operator=(const Future &) = delete;

    Future operator=(Future &&) = delete;

    ~Future() {
//        cout << "\tFuture::~Future(): impl.use_count() = " << impl.use_count() << ", i = " << i << '\n';
    }

    void set(T *object, bool toDelete) {
        auto lock = autoLock(impl->objectLock);
/*        cout << "\tFuture::set(): impl.use_count() = " << impl.use_count() << ", i = " << i << '\n';
        std::cout << "Future::set: impl = " << &*impl << '\n';
        std::cout << "Future::set: impl->object = " << impl->object << '\n';
        std::cout << "Future::set: object = " << object << '\n'; */
        impl->object = object;
        impl->exists = true;
        impl->toDelete = toDelete;
        impl->cond.broadcast();
    }

    bool available() {
        auto lock = autoLock(impl->objectLock);
        return impl->exists;
    }

    T get() {

        while (!impl->exists) {
            impl->cond.wait();
        }

        auto lock = autoLock(impl->objectLock);
        const T &t = *impl->object;
        return t;
    }
private:
    struct Impl {
        T *object;
        Lock objectLock;
        bool toDelete;
        bool exists;
        Condition cond;
//        Lock lock;

        Impl()
        :object(nullptr), toDelete(false), exists(false) {
//            cout << "\tFuture::Impl::Impl()\n";
        }

        Impl(const Impl &) = delete;
        Impl(Impl &&) = delete;

        ~Impl() {
//            cout << "\tFuture::Impl::~Impl()\n";
            if (toDelete) {
                delete object;
            }
        }
    };

    static int _i;

    std::shared_ptr<Impl> impl;
    int i;
};

template <>
class Future<void> {
public:
    Future()
    :impl(new Impl), i(_i++) {
//        cout << "\tFuture::Future(): impl.use_count() = " << impl.use_count() << ", i = " << i << '\n';
    }

    Future(const Future &other)
    :impl(other.impl), i(other.i) {
//        cout << "\tFuture::Future(const Future &): impl.use_count() = " << impl.use_count() << ", i = " << i << '\n';
    }

    Future(Future &&other)
    :impl(std::move(other.impl)), i(other.i) {
//        cout << "\tFuture::Future(Future &&): impl.use_count() = " << impl.use_count() << ", i = " << i << '\n';
    }

    template <typename U>
    Future operator=(U t) = delete;

    ~Future() {
//        cout << "\tFuture::~Future(): impl.use_count() = " << impl.use_count() << ", i = " << i << '\n';
    }

    void set() {
//        cout << "\tFuture::set(): impl.use_count() = " << impl.use_count() << ", i = " << i << '\n';
//        std::cout << "Future::set: impl = " << &*impl << '\n';
        impl->complete = true;
        impl->cond.broadcast();
    }

    bool available() {
        return impl->complete;
    }

    bool get() {
        while (!impl->complete) {
            impl->cond.wait();
        }
        return impl->complete;
    }
private:
    struct Impl {
        std::atomic<bool> complete;
        Lock lock;
        Condition cond;

        Impl()
        :complete(false) {
//            cout << "\tFuture::Impl::Impl()\n";
        }

        Impl(const Impl &) = delete;
        Impl(Impl &&) = delete;

        ~Impl() {
//            cout << "\tFuture::Impl::~Impl()\n";
        }
    };

    static int _i;

    std::shared_ptr<Impl> impl;
    int i;
};

template <typename T>
int Future<T>::_i = 0;

//int Future<void>::_i = 0;

}

#endif
