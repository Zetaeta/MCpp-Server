
#ifndef TEMPLATEUTILS_HPP
#define TEMPLATEUTILS_HPP

template <typename T, typename... Others>
struct FirstType {
    typedef T type;
};

template <typename T>
struct GenericFirst {

template <T N, T... Others>
struct First {
    static const T value = N;
};

};

#endif
