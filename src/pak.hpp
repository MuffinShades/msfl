#pragma once
/*

	MPAK v1.0

	Written in C++ by James Weigand. December 2023. Updated May 2024
	Copyright James Weigand 2023 - Present. All credit goes to
	me =^._.^=
*/


#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "balloon.hpp"

#define COMPRESSION_HUFFMAN 0x01
#define COMPRESSION_LZSS 0x02
#define COMPRESSION_MZIP 0x03

#define PAK_MAX_FILES 65536

//#define READWRITE_CHUNKSIZE 0xffffff

#define PAK_ERROR_BLANK_SRC 0xd04
#define PAK_ERROR_INVALID_SRC 0xd05

#define PAK_NOERROR 0x00

#define PAKRESULT_NOFILEFOUND 0xf01

#ifndef MSFL_EXP
#ifdef MSFL_EXPORTS
#define MSFL_EXP __declspec(dllexport)
#else
#define MSFL_EXP __declspec(dllimport)
#endif
#endif

extern "C" {

	struct PakChunk {
		std::string fName = "";
		unsigned char* fData = nullptr;
		int fSize = 0;
		int compressType = 0x00; //compressType = 0 -> no compression, 1 -> compression
		std::string fType = "";
		unsigned char* addHeaderInf = nullptr;
		int adhiLen = 0;
		int _fileIndex = -1;
		int compressionLevel = 2;
		int resType = 0x00;
	};

	struct PakStructure {
		std::vector<PakChunk> files;
		int status = 0;
		int totalByteSize = 0;
		unsigned char* rawHeader = nullptr;
		int rawHeaderLen = 0;
		unsigned char* addtionalHeader = nullptr;
		int addtionalHeaderLen = 0;
		int resType = 0x00;
	};

	//info for getting file can use either or both when extracting file
	struct PakInfo {
		int nFile = 0;
		std::string fileName = "";
	};

	class pak
	{
	public:
		MSFL_EXP static int PackToFile(std::string src, PakStructure data);
		MSFL_EXP static PakStructure ExtractContents(std::string src);
		MSFL_EXP static int ExtractContentsToDisk(std::string pakSrc, std::string resSrc);
		MSFL_EXP static int PakFilesInDirectory(std::string targetDir, std::string outputSrc, int compressionType = 0, int compressionLevel = 2);
		MSFL_EXP static PakChunk ExtractFile(std::string src, PakInfo file);
	};
}
