#pragma once
#include <iostream>
#include <cstring>
#include "balloon.hpp"

enum ByteStreamMode {
	bmode_LittleEndian,
	bmode_BigEndian
};

class ByteStream {
private:
	byte* bytes;
	size_t len;
	size_t allocSz;
	size_t readPos = 0;
	size_t chunkSz = 0xfff;
public:
	int mode = bmode_BigEndian;
	ByteStream(byte* dat, size_t sz) {
		if (dat == nullptr || sz <= 0) return;

		this->bytes = new byte[sz];
		this->len = sz;
		this->allocSz = this->len;
		memcpy(this->bytes, dat, this->len);
	}

	ByteStream() {
		this->bytes = nullptr;
		this->len = 0;
		this->allocSz = 0;
	};

	byte readByte() {
		if (this->readPos >= this->len - 1)
			return NULL;

		return this->bytes[this->readPos++];
	}

	unsigned long readBytesAsVal(size_t nBytes) {
		if (nBytes <= 0) return NULL;
		unsigned long res = 0x00;
		switch (this->mode) {
		case bmode_BigEndian: {
			for (int i = nBytes - 1; i >= 0; i--)
				res |= (this->readByte() << (i * 8));
			break;
		}
		case bmode_LittleEndian: {
			for (int i = 0; i < nBytes; i++)
				res |= (this->readByte() << (i * 8));
			break;
		}
		}
		return res;
	}

	short readInt16() {
		return (short)this->readBytesAsVal(sizeof(short));
	}

	unsigned short readUInt16() {
		return (unsigned)this->readInt16();
	}

	int readInt32() {
		return (int)this->readBytesAsVal(sizeof(int));
	}

	unsigned int readUInt32() {
		return (unsigned)this->readInt32();
	}

	long readInt64() {
		return (long)this->readBytesAsVal(sizeof(long));
	}

	unsigned long readUInt64() {
		return (unsigned)this->readInt64();
	}
};