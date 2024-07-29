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
    iErrorWrap(_Ty v, ErrorDescriptor* err = nullptr) : v(v) : err(err) {};
    struct ErrorDescriptor* err;
    _Ty operator _Ty() {
        return this->v;
    }
    bool err() {
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