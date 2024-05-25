#pragma once

/*

Written in C++ by muffinshades

Copyright muffinshades 2023-Present, all rights reserved.

*/

//includes lzss and huffman compression tools for .pak not a header in the dll


#ifdef MSFL_EXPORTS
#define MSFL_EXP __declspec(dllexport)
#else
#define MSFL_EXP __declspec(dllimport)
#endif

class Compress
{
};

#include "Compress.h"


#pragma once
#include <iostream>
#include <vector>
#include <string>

#define _MAX(a,b) (a>b?a:b)
#define _MIN(a,b) (a<b?a:b)

struct lzss_settings {
	int b_buffer = 32768;
	int s_buffer = 256;
	int BYTES_PER_BUFFER = 2;
	lzss_settings() {}
};

struct lzss_result {
	unsigned char* bytes;
	int len = 0;
	float comp_ratio = 0.0f;
	lzss_result() {
		bytes = nullptr;
	}
};

//LZSS
extern "C" {

	class lzss {
	public:

		MSFL_EXP static lzss_result compress(unsigned char* bytes, int len, lzss_settings s);

		MSFL_EXP static lzss_result decompress(unsigned char* bytes, int len, int BUFFER_BYTE_LEN = 2);

		MSFL_EXP static lzss_result decompress(lzss_result dat, int BUFFER_BYTE_LEN = 2);
	};


	//huffman coding written in C++ by James Weigand (@muffinshades Twitter Reddit Discord Instagram)
#define HUFFMAN_PENDING 0x00
#define HUFFMAN_ERROR 0x01
#define HUFFMAN_RESULT_ENCODE 0x02
#define HUFFMAN_RESULT_DECODE 0x03

	struct HuffmanTreeNode {
		HuffmanTreeNode* left = nullptr;
		HuffmanTreeNode* right = nullptr;
		char c = NULL;
		bool branch = false;
		int count = 0;
		~HuffmanTreeNode() {
			//delete[] this->left;
			//delete[] this->right;
		}
	};

	struct CharCount {
		int count = 0;
		char c = NULL;
		operator HuffmanTreeNode() { //conversion
			HuffmanTreeNode res;
			res.c = this->c;
			res.count = this->count;
			return res;
		};
	};

	//byte pointer with length
	struct RawBytes {
		unsigned char* bytes = nullptr;
		int len = 0;
		MSFL_EXP void CopyToBuffer(RawBytes* buffer);
		MSFL_EXP void deleteMemory();
	};

	struct HuffmanResult {
		unsigned char* bytes = nullptr;
		std::vector<CharCount> charCount;
		int nChars = 0;
		int len = 0;
		int bitLen = 0;
		float compressionRatio = 0.0f;
		int bitOverflow = 0;
		MSFL_EXP RawBytes GetFullByteResult(); //gives header + bytes
		int resultType = HUFFMAN_PENDING;
		~HuffmanResult() {
			//delete[] this->bytes;
			//delete[] this->charCount;
		}
	};

	class Huffman {
	public:
		MSFL_EXP static HuffmanResult Encode(unsigned char* bytes, int len);
		MSFL_EXP static HuffmanResult Decode(unsigned char* bytes, int len);
		MSFL_EXP static HuffmanResult Decode(HuffmanResult res);
	};

	class BytePrinter {
	public:
		MSFL_EXP static std::string GetByteString(unsigned char* bytes);
		MSFL_EXP static unsigned char* GetBytesFromString(std::string text);
	};

	struct MZIP_SETTINGS {
		int b_buffer = 32768;
		int s_buffer = 256;
		int BYTES_PER_BUFFER = 2;
		bool alwaysCompress = false;
		MZIP_SETTINGS() {};
	};

	class mzip {
	public:
		MSFL_EXP static RawBytes Encode(unsigned char* bytes, int len, MZIP_SETTINGS settings = MZIP_SETTINGS());
		MSFL_EXP static RawBytes Decode(unsigned char* bytes, int len, MZIP_SETTINGS settings = MZIP_SETTINGS());
	};
}