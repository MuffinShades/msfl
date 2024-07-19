#include "BinUtility.h"
#include <iostream>

template<class _T> unsigned char* BinUtility::GetValBytes(_T val) {
	unsigned char* _bytes = new unsigned char[sizeof(val)];

	for (size_t i; i < sizeof(val); i++) {
		*(_bytes + i) = (val >> i * 8) & 0xff;
	}

	return _bytes;
}