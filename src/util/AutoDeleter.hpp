
#ifndef AUTODELETER_HPP
#define AUTODELETER_HPP

template<class T>
class AutoDeleter {
public:
    AutoDeleter(T *t) :toBeDeleted(t) {}
    ~AutoDeleter() {
        delete toBeDeleted;
    }
private:
    T *toBeDeleted;
};

template<class T>
class ArrayAutoDeleter {
public:
    ArrayAutoDeleter(T *t) :toBeDeleted(t) {}
    ~ArrayAutoDeleter() {
        delete[] toBeDeleted;
    }
private:
    T *toBeDeleted;
};

#endif
