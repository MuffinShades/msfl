/*
Inflate Reference:

https://pyokagan.name/blog/2019-10-18-zlibinflate/
https://web.archive.org/web/20230528220141/https://pyokagan.name/blog/2019-10-18-zlibinflate/

Deflate Reference:

My brain :)
*/


#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <vector>

#define HUFFMAN_PENDING 0x00
#define HUFFMAN_ERROR 0x01
#define HUFFMAN_RESULT_ENCODE 0x02
#define HUFFMAN_RESULT_DECODE 0x03

#ifdef MSFL_EXPORTS
#define MSFL_EXP __declspec(dllexport)
#else
#define MSFL_EXP
#endif

//types
typedef long i64;
typedef unsigned long u64;
typedef unsigned int u32;
typedef int i32;
typedef unsigned short u16;
typedef short i16;
typedef unsigned char u8;
typedef char i8;
typedef unsigned char byte;

#ifdef MSFL_EXPORTS
extern "C" {
#endif

//huffman tree node
struct HuffmanTreeNode {
	HuffmanTreeNode* left = nullptr;
	HuffmanTreeNode* right = nullptr;
	u32 val = NULL;
	bool branch = false;
	i32 count = 0;
	i32 depth = 0;
	u32* symCodes;
	size_t alphabetSz;
};

//character count
/*struct CharCount {
	i32 count = 0;
	u32 val = NULL;
	operator HuffmanTreeNode() { //conversion
		HuffmanTreeNode res;
		res.val = this->val;
		res.count = this->count;
		return res;
	};
};*/

//byte pointer with length
struct RawBytes {
	u32* bytes = nullptr;
	size_t len = 0;
	void CopyToBuffer(RawBytes* buffer);
	void free() {
		delete[] this->bytes;
	};
};

//result from huffman decode and encode
class HuffmanResult {
public:
	HuffmanTreeNode* tree;
	byte* bytes = nullptr;
	i32 nChars = 0;
	size_t len = 0;
	i32 resultType = HUFFMAN_PENDING;
	~HuffmanResult();
};

//bit stream class for reading and doing stuff with bits
class BitStream {
public:
	u32* bytes = nullptr;
	i32 pos = 0, rPos = 0;
	i32 lBit = 0;
	u32 cByte = 0;
	u32 rBit = 0;
	size_t sz, bsz, asz;
	//bit stream for reading existing bytes / bits
	MSFL_EXP BitStream(u32* bytes, size_t len);
	//basic zero allocation bit stream for writing
	MSFL_EXP BitStream(size_t len);
	MSFL_EXP i32 readBit();
	MSFL_EXP i32 readByte();
	MSFL_EXP i32 readNBits(i32 nBits);
	MSFL_EXP i32 readValue(size_t size);
	MSFL_EXP static u32* bitsToBytes(u32* bits, size_t len);
	MSFL_EXP static u32* bitsToBytes(u32 bits, size_t len);
	MSFL_EXP void checkWritePosition();
	MSFL_EXP void writeBit(u32 bit);
	//write multiple bits
	MSFL_EXP void writeNBits(u32* bits, size_t len);
	//write short, long, int, uint, etc.
	MSFL_EXP void writeValue(byte val);
	MSFL_EXP void writeValue(short val);
	MSFL_EXP void writeValue(i32 val);
	MSFL_EXP void writeValue(u32 val);
	MSFL_EXP void writeValue(i64 val);
	MSFL_EXP void writeValue(u64 val);
	//allocate new bit chunk
	MSFL_EXP void allocNewChunk();
	MSFL_EXP void seek(i32 pos);
	MSFL_EXP void seekw(i32 wPos);
	MSFL_EXP i32 tell();
	MSFL_EXP i32 tellw();
	//remove unused bytes
	MSFL_EXP void clip();
	//allocation function
	MSFL_EXP void calloc(size_t sz);
};

class Huffman {
public:
	MSFL_EXP static void DebugMain();
	MSFL_EXP static HuffmanResult Encode(char* bytes, size_t len, bool encodeTree = true);
	MSFL_EXP static HuffmanResult Decode(char* bytes, size_t len, bool useTree = true, HuffmanTreeNode* tree = nullptr);
};

class lz77 {

};

class ZResult {
public:
	u32* bytes;
	size_t len;
	i32 compressionLevel, compressionMode;
	u32 checkSum;
	MSFL_EXP ZResult() {};
};
/*
Zlib class for the zlib stuff

Deflate levels are 0, 1, or 2 with 0 being no compression,
1 being just huffman, and 2 being both huffman and lz77

Note ill add the level 0-10 sysetm later

*/
class Zlib {
public:
	MSFL_EXP static ZResult* Inflate(u32* bytes, size_t len);
	MSFL_EXP static ZResult Deflate(u32* bytes, size_t len, const size_t winBits, const int level);
};

#ifdef MSFL_EXPORTS
}
#endif


template<typename _Ty> class arr_container {
public:
	_Ty* dat = nullptr;
	size_t sz = 0;
	arr_container(_Ty* dat = nullptr, size_t sz = 0) {
		if (dat && sz > 0) {
			this->sz = sz;
			this->dat = new _Ty[sz];
			memcpy(this->dat, dat, sizeof(_Ty) * sz);
		}
	}
};