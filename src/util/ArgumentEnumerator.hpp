
#ifndef ARGUMENTENUMERATOR_HPP
#define ARGUMENTENUMERATOR_HPP

namespace MCServer {

template <int...>
struct PlaceHolder {};

template <int N, int... S>
struct ArgumentEnumerator : ArgumentEnumerator<N - 1, N - 1, S...> {};

template <int... S>
struct ArgumentEnumerator<0, S...> {
    typedef PlaceHolder<S...> type;
};

}

#endif
