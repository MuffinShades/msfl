#pragma once
#include <iostream>
#include <cstring>
#include <cmath>
#include "balloon.hpp"
#include "msutil.hpp"

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

enum ByteStreamMode {
	bmode_LittleEndian,
	bmode_BigEndian
};

class ByteStream {
private:
	byte* bytes = nullptr;
	size_t len = 0;
	size_t allocSz = 0;
	size_t readPos = 0;
	size_t chunkSz = 0xfff;
public:
	int mode = bmode_BigEndian;
	void allocNewChunk();
	void allocBytes(size_t sz);
	ByteStream(byte* dat, size_t sz);
	ByteStream();
	byte readByte();
	size_t seek(size_t pos);
	unsigned long long readBytesAsVal(size_t nBytes);
	short readInt16();
	unsigned short readUInt16();
	int readInt32();
	unsigned int readUInt32();
	int64_t readInt64();
	uint64_t readUInt64();
	void writeByte(byte b);
	void writeNBytesAsVal(unsigned long long v, size_t nBytes);
	void writeInt16(short v);
	void writeUInt16(unsigned short v);
	void writeInt32(int v);
	void writeUInt32(unsigned int v);
	void writeInt64(int64_t v);
	void writeUInt64(uint64_t v);
	void clip();
	void writeBytes(byte *dat, size_t sz);
	void free() {
		if (this->bytes != nullptr) delete[] this->bytes;
		this->bytes = nullptr;
	}
	~ByteStream();
	size_t getSize() {
		return this->len;
	};
	size_t getAllocSize() {
		return this->allocSz;
	};
	byte *getBytePtr() {
		return this->bytes;
	}
	size_t tell() {
		return this->readPos;
	}
	size_t skipBytes(size_t nBytes);
	const char *readCStr(size_t len);
	std::string readStr(size_t len);
};