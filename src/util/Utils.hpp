
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#define STD_PREDECL(x) namespace std {\
    class x;\
    }

namespace MCServer {

inline uint32_t floatToIntBits(float f) {
    uint32_t *i = reinterpret_cast<uint32_t *>(&f);
    return *i;
}

inline uint64_t doubleToInt64Bits(double d) {
    uint64_t *i = reinterpret_cast<uint64_t *>(&d);
    return *i;
}

template<class T>
inline T max(T a, T b) {
    return a < b ? b : a;
}

template<class T>
inline T min(T a, T b) {
    return a < b ? a : b;
}

void invalidOption(std::string message, int exitCode = 1);

void errorExit(std::string message, int exitCode = 1);

extern std::string commandName;

bool inUsrShare();

//std::string demangle(const std::string &);

long diffms(timespec start, timespec end);

long currentTimeMillis();

}

#endif /* UTILS_HPP_ */
