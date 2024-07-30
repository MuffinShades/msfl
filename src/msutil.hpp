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

#define GMask(l) ((1 << (l)) - 1)

struct ErrorDescriptor {
    i32 code = 0;
    std::string msg = "No error";
};

template<class _Ty> class iErrorWrap {
private:
    std::vector<ErrorDescriptor*> genericErrs;
public:
    _Ty v;
    iErrorWrap(_Ty v, ErrorDescriptor* err = nullptr) : v(v), err(err) {};
    struct ErrorDescriptor* err;
    operator _Ty() const {
        return this->v;
    }
    bool error() {
        if (this->err != nullptr)
            return this->err->code == 0;
        else
            return false;
    }

    static ErrorDescriptor* CreateGenericError(i32 code, std::string desc) {
        return nullptr;
    }
};

static inline std::vector<std::string> SplitString(std::string str, const char delim) {
    std::vector<std::string> res;
    const size_t len = str.length();
    std::string collector = "";
    const char *cStr = str.c_str();

    for (size_t i = 0; i < len; i++) {
        if (cStr[i] == delim) {
            res.push_back(collector);
            collector = "";
        } else
            collector += cStr[i];
    }

    res.push_back(collector);

    return res;
}

#define MAKE_LONG_BE(b0, b1, b2, b3, b4, b5, b6, b7) \
    ((b0) << 56) | \
    ((b1) << 48) | \
    ((b2) << 40) | \
    ((b3) << 32) | \
    ((b4) << 24) | \
    ((b5) << 16) | \
    ((b6) <<  8) | \
    ((b7) <<  0)

#define MAKE_LONG_LE(b0, b1, b2, b3, b4, b5, b6, b7) \
    ((b7) << 56) | \
    ((b6) << 48) | \
    ((b5) << 40) | \
    ((b4) << 32) | \
    ((b3) << 24) | \
    ((b2) << 16) | \
    ((b1) <<  8) | \
    ((b0) <<  0)

#define MAKE_INT_BE(b0, b1, b2, b3) \
    ((b0) << 24) | \
    ((b1) << 16) | \
    ((b2) <<  8) | \
    ((b3) <<  0)

#define MAKE_INT_LE(b0, b1, b2, b3) \
    ((b3) << 24) | \
    ((b2) << 16) | \
    ((b1) <<  8) | \
    ((b0) <<  0)

#define MAKE_SHORT_BE(b0, b1) \
    ((b0) <<  8) | \
    ((b1) <<  0)

#define MAKE_SHORT_LE(b0, b1) \
    ((b1) <<  8) | \
    ((b0) <<  0)

static inline u64 modifyByte(u64 val, i32 b, byte v) {
    u64 mask = ((1 << (sizeof(u64) << 3)) - 1);
    b <<= 3;
    mask ^= 0xff << b;
    val &= mask;
    return val | (v << b);
}