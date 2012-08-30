/*
 * utils.hpp
 *
 *  Created on: 4 Aug 2012
 *      Author: daniel
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#define STD_PREDECL(x) namespace std {\
    class x;\
    }

inline uint32_t floatToIntBits(float f) {
    return *reinterpret_cast<uint32_t *>(&f);
}

inline uint64_t doubleToInt64Bits(double d) {
    return *reinterpret_cast<uint64_t *>(&d);
}

#endif /* UTILS_HPP_ */
