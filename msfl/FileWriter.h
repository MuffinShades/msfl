#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include "balloon.hpp"

class FileWriter {
public:
	static bool WriteToBinFile(std::string path, unsigned char* bytes, int len);
	static arr_container<byte> ReadBytesFromBinFile(std::string path);
	static bool WriteToBinFileLPCWSTR(LPCWSTR path, unsigned char* bytes, int len);
	static arr_container<byte> ReadBytesFromBinFileLPCWSTR(LPCWSTR path);
};