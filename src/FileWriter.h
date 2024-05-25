#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>

class FileWriter {
	bool WriteToBinFile(std::string path, unsigned char* bytes, int len);
	struct RawBytes ReadBytesFromBinFile(std::string path);
	bool WriteToBinFileLPCWSTR(LPCWSTR path, unsigned char* bytes, int len);
	struct RawBytes ReadBytesFromBinFileLPCWSTR(LPCWSTR path);
};