
#include "util/ArgumentEnumerator.hpp"

void derp() {}

template <typename... Contents>
void readHelper(InputStream &stream, Contents&... contents) {
    derp(stream.operator>>(contents)...);
}

template <typename... Contents>
void writeHelper(InputStream &stream, const Contents&... contents) {
    
}
