#include <iostream>
#include <windows.h>
#include "pch.h"
#include "png.hpp"
#include "pak.hpp"
#include "ptrconverter.hpp"
#include "jparse.hpp"

#define CONSOLE_COLOR_DBLUE 1
#define CONSOLE_COLOR_DGREEN 2
#define CONSOLE_COLOR_DCYAN 3
#define CONSOLE_COLOR_DRED 4
#define CONSOLE_COLOR_DMAGENTA 5
#define CONSOLE_COLOR_DYELLOW 6
#define CONSOLE_COLOR_GRAY 7
#define CONSOLE_COLOR_DGRAY 8
#define CONSOLE_COLOR_BLUE 9
#define CONSOLE_COLOR_GREEN 10 
#define CONSOLE_COLOR_CYAN 11
#define CONSOLE_COLOR_RED 12
#define CONSOLE_COLOR_MAGENTA 13
#define CONSOLE_COLOR_YELLOW 14
#define CONSOLE_COLOR_WHITE 15

#define _CRT_SECURE_NO_WARNINGS

//#ifndef MSFL_EXPORTS
HANDLE cHandle = GetStdHandle(
	STD_OUTPUT_HANDLE);
void SetConsoleColor(int color) {
	SetConsoleTextAttribute(cHandle, color);
}

int main() {

	//zlib testing
	std::cout << "----Zlib Test----" << std::endl;
	SetConsoleColor(CONSOLE_COLOR_BLUE);
	//
	size_t tSz = 1 << 8;
	//
	std::cout << "Encoding: " << tSz << " bytes" << std::endl;
	u32* tBytes = new u32[tSz];
	srand(time(NULL));
	for (int i = 0; i < tSz; i++) {
		tBytes[i] = (u32)(rand() % 0xff) + 0x00;
		std::cout << std::hex << tBytes[i] << " ";
	}
	std::cout << std::endl;
	SetConsoleColor(CONSOLE_COLOR_YELLOW);
	std::cout << "Deflate Start..." << std::endl;
	ZResult deflateDat = Zlib::Deflate(tBytes, tSz, 15, 1);
	SetConsoleColor(CONSOLE_COLOR_YELLOW);
	std::cout << "Deflate done!" << std::endl;
	SetConsoleColor(CONSOLE_COLOR_GRAY);
	std::cout << std::dec;
	//for (int i = 0; i < deflateDat.len; i++) {
		//std::cout << deflateDat.bytes[i] << " ";
	//}
	std::cout << std::endl;
	SetConsoleColor(CONSOLE_COLOR_WHITE);
	std::cout << "Inflate Start..." << std::endl;
	ZResult* inflateDat = Zlib::Inflate(deflateDat.bytes, deflateDat.len);
	delete[] deflateDat.bytes;
	std::cout << "Inflate Done!" << std::endl;
	SetConsoleColor(CONSOLE_COLOR_DCYAN);
	std::cout << "CheckSum: " << std::hex << inflateDat->checkSum << std::dec << std::endl;
	SetConsoleColor(CONSOLE_COLOR_MAGENTA);
	std::cout << "-------Starting Byte Check-------" << std::endl;
	size_t checkEvery = 1u << 18;
	float tested = 0;
	float correct = 0;
	for (int i = 0; i < tSz; i++) {
		if (i % checkEvery == 0 && i > 1) {
			std::cout << "Checked: " << tested << " / " << tSz << " bytes | " << (correct / tested) << std::endl;
		}

		tested++;

		if (inflateDat->bytes[i] == tBytes[i]) correct++;
		else
			std::cout << "RBytes: " << (int)inflateDat->bytes[i] << " " << tBytes[i] << " [" << i << "]" << std::endl;
	}

	//pak testing
	std::cout << "Err Code: " << pak::PakFilesInDirectory("C:\\paktesting\\testpak\\", "C:\\paktesting\\res.pak", 1, 2);
	pak::ExtractContentsToDisk("C:\\paktesting\\res.pak", "C:\\paktesting\\pakoutput\\");

	//png testing
	SetConsoleColor(CONSOLE_COLOR_WHITE);
	//png_file pngTestFile = PngParse::Decode("C:\\msfltestfiles\\png_test.png");

	//json testing
	arr_container<byte> json_bytes = FileWriter::ReadBytesFromBinFile("C:\\Users\\chris\\Downloads\\large-file.json");
	JStruct testRes = jparse::parseStr(
		(const char*)ptrconverter::convertTo<byte, char>(json_bytes.dat, json_bytes.sz)
	);
	delete[] json_bytes.dat;

	testRes.print();

	return 0;
} 

//#endif //ifndef MSFL_EXPORTS