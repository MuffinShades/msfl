#pragma once
#include <iostream>
#include <cstring>

template<class _Ty> inline static void ZeroMem(_Ty *dat, size_t sz) {
    if (dat == nullptr) return;
    memset(dat, 0, sizeof(_Ty) * sz);
}

inline static size_t GetNumSz(const unsigned long long num) {
    unsigned long long compare = 1;
    size_t nBytes = 0;
    const size_t mxSz = sizeof(unsigned long long);

    while (num >= compare) {
        nBytes++;
        compare <<= 8;
    }

    return nBytes;
}

inline static float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

struct Point {
    float x, y;
};