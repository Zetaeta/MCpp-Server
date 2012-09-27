
#ifndef TRIPLE_HPP
#define TRIPLE_HPP

#include <type_traits>
#include <utility>

namespace MCServer {

template<class T1, class T2, class T3>
struct triple {
    typedef T1 first_type;
    typedef T2 second_type;
    typedef T3 third_type;

    T1 first;
    T2 second;
    T3 third;

    constexpr triple()
    :first(), second(), third() {}

    constexpr triple(const T1 &f, const T2 &s, const T3 &t)
    :first(f), second(s), third(t) {}

    template<class U1, class U2, class U3, class = typename
        std::enable_if<std::__and_<
            std::is_convertible<const U1 &, T1>,
            std::is_convertible<const U2 &, T2>,
            std::is_convertible<const U3 &, T3>
        >::value>::type>
    constexpr triple(const triple<U1, U2, U3> &t)
    :first(t.first), second(t.second), third(t.third) {}

    template<class U1, class U2, class U3, class = typename
        std::enable_if<std::__and_<
            std::is_convertible<const U1 &, T1>,
            std::is_convertible<const U2 &, T2>,
            std::is_convertible<const U3 &, T3>
        >::value>::type>
    constexpr triple(triple<U1, U2, U3> &&t)
    :first(std::forward<U1>(t.first)), second(std::forward<U2>(t.second)), third(std::forward<U3>(t.third)) {}

    triple & operator=(const triple &o) {
        first = o.first;
        second = o.first;
        third = o.third;
        return *this;
    }

    triple & operator=(triple &&o) {
        first = std::forward<T1>(o.first);
        second = std::forward<T2>(o.second);
        third = std::forward<T3>(o.third);
        return *this;
    }

    template<class U1, class U2, class U3>
    triple & operator=(const triple<U1, U2, U3> &o) {
        first = o.first;
        second = o.second;
        third = o.third;
        return *this;
    }

    template<class U1, class U2, class U3>
    triple & operator=(triple<U1, U2, U3> &&o) {
        first = std::forward<U1>(o.first);
        second = std::forward<U2>(o.second);
        third = std::forward<U3>(o.third);
        return *this;
    }

    void swap(triple &o) {
        std::swap(first, o.first);
        std::swap(second, o.second);
        std::swap(third, o.third);
    }

};

template<class T1, class T2, class T3>
inline constexpr bool operator==(const triple<T1, T2, T3> &x, const triple<T1, T2, T3> &y) {
    return x.first == y.first && x.second == y.second && x.third == y.third;
}

template<class T1, class T2, class T3>
inline constexpr bool operator!=(const triple<T1, T2, T3> &x, const triple<T1, T2, T3> &y) {
    return !(x == y);
}

template<class T1, class T2, class T3>
inline constexpr bool operator<(const triple<T1, T2, T3> &x, const triple<T1, T2, T3> &y) {
    return x.first < y.first ? true : 
        (x.second < y.second ? true : (x.third < y.third));
}

template<class T1, class T2, class T3>
inline constexpr bool operator>(const triple<T1, T2, T3> &x, const triple<T1, T2, T3> &y) {
    return y < x;
}

template<class T1, class T2, class T3>
inline constexpr bool operator<=(const triple<T1, T2, T3> &x, const triple<T1, T2, T3> &y) {
    return !(y < x);
}

template<class T1, class T2, class T3>
inline constexpr bool operator>=(const triple<T1, T2, T3> &x, const triple<T1, T2, T3> &y) {
    return !(x < y);
}
template<class T1, class T2, class T3>
inline void swap(triple<T1, T2, T3> &x, triple<T1, T2, T3> &y) {
    x.swap(y);
}

template<class T1, class T2, class T3>
inline triple<T1, T2, T3> make_triple(T1 &&t1, T2 &&t2, T3 &&t3) {
    return triple<T1, T2, T3>(std::forward<T1>(t1), std::forward<T2>(t2), std::forward<T3>(t3));
}

template<class T1, class T2, class T3>
inline triple<T1, T2, T3> make_triple(const T1 &t1, T2 &t2, T3 &t3) {
    return triple<T1, T2, T3>(t1, t2, t3);
}

template<class T1, class T2, class T3>
inline triple<T1, T2, T3> make_triple(T1 t1, T2 t2, T3 t3) {
    return triple<T1, T2, T3>(t1, t2, t3);
}

}

#endif
