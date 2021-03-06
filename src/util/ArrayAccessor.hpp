
#ifndef ARRAYACCESSOR_HPP
#define ARRAYACCESSOR_HPP

#include <iostream>
#include <type_traits>

#include "TemplateUtils.hpp"

#define ARRAY_ACCESSOR_DEBUG_CHECKING

#ifdef ARRAY_ACCESSOR_DEBUG_CHECKING
#include <assert.h>
#define CHECK_RANGE(range, index) assert(index < range);
#else
#define CHECK_RANGE(range, index)
#endif

namespace MCServer {

template<class T, size_t N, size_t... Dimensions>
class ArrayAccessor {
public:

    typedef T type;
    typedef const typename std::remove_const<T>::type const_type;

    ArrayAccessor(T *array)
    :array(array), offset(0), multiplier(1) {
    }

    ArrayAccessor<T, Dimensions...> operator[](size_t index) {
        CHECK_RANGE(N, index)
        return ArrayAccessor<T, Dimensions...>(array, offset + (index * multiplier),
                                            GenericFirst<int>::First<Dimensions...>::value);
    }

    ArrayAccessor<const_type, Dimensions...> operator[](size_t index) const {
        CHECK_RANGE(N, index)
        return ArrayAccessor<const_type, Dimensions...>(array, offset + (index * multiplier),
                                             GenericFirst<int>::First<Dimensions...>::value);
    }
private:
    ArrayAccessor(T *array, size_t offset, size_t multiplier)
    :array(array), offset(offset), multiplier(multiplier) {}

    template <class T2, size_t N2, size_t...Dims>
    friend class ArrayAccessor;

    T *array;
    size_t offset; // Offset from the start of the array, from the index into the previous accessors.
    /**
     * Value by which to multiply the current index, from the previous accessor's first Dimensions... value.
     * "Why do I not just use the current accessor's N?" you might ask (or rather, I asked a couple of months later).
     * If I used the current N, that would be used even if this is the first in a sequence of accessors.
     * The first one however should not have a multiplier, hence a multiplier of 1 by default and only another
     * via the private constructor.
     */
    size_t multiplier;

};

template<class T, size_t N>
class ArrayAccessor<T, N> {
public:

    typedef T type;
    typedef const typename std::remove_const<T>::type const_type;

    ArrayAccessor(T *array)
    :array(array), offset(0) {}

    T & operator[](size_t index) {
        CHECK_RANGE(N, index)
        return array[offset + (index * multiplier)];
    }

    const_type & operator[](size_t index) const {
        CHECK_RANGE(N, index)
        return array[offset + (index * multiplier)];
    }
private:
    ArrayAccessor(T *array, size_t offset, size_t multiplier)
    :array(array), offset(offset), multiplier(multiplier) {}

    template <class T2, size_t N2, size_t...Dims>
    friend class ArrayAccessor;

    T *array;
    size_t offset;
    size_t multiplier;
};

}

#endif
