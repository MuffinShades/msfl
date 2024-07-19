#include <iostream>
#include "png.hpp"
#include "pak.hpp"
#include "ptrconverter.hpp"
#include "jparse.hpp"
#include "asset.hpp"
#include "ttf.hpp"
#include "ttfRender.hpp"
#include "bitmap.hpp"
#include "bitmapRender.hpp"

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

#ifdef WIN32
HANDLE cHandle = GetStdHandle(
	STD_OUTPUT_HANDLE);
void SetConsoleColor(int color) {
	SetConsoleTextAttribute(cHandle, color);
}
#else
void SetConsoleColor(int color) {

}
#endif

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
	/*std::cout << "-------Starting Byte Check-------" << std::endl;
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
	}*/

	//pak testing
	//td::cout << "Err Code: " << pak::PakFilesInDirectory("C:\\paktesting\\testpak\\", "C:\\paktesting\\res.pak", 1, 2);
	//pak::ExtractContentsToDisk("C:\\paktesting\\res.pak", "C:\\paktesting\\pakoutput\\");

	//png testing
	SetConsoleColor(CONSOLE_COLOR_WHITE);
	//png_file pngTestFile = PngParse::Decode("C:\\msfltestfiles\\png_test.png");

	//json testing
	/*arr_container<byte> json_bytes = FileWriter::ReadBytesFromBinFile("C:\\Users\\chris\\Downloads\\large-file.json");
	JStruct testRes = jparse::parseStr(
		(const char*)ptrconverter::convertTo<byte, char>(json_bytes.dat, json_bytes.sz)
	);
	delete[] json_bytes.dat;

	testRes.print();*/

	//asset testing
	std::cout << "--Asset Testing--" << std::endl;
	AssetStruct testAsset;
	testAsset.AddAsset(
		"a.b.c.d",
		"/Users/jamesweigand/Desktop/testingFolder/testAsset.rtf"
	);
	testAsset.AddAsset(
		"a.b.g",
		"/Users/jamesweigand/Desktop/testingFolder/asset2.json"
	);
	std::cout << "Writing to file..." << std::endl;
	AssetParse::WriteToFile("/Users/jamesweigand/Desktop/testingFolder/assetPak1.ap", &testAsset);

	//ttf file testing
	ttfFile testFile = ttfParse::ParseTTFFile("/Users/jamesweigand/Desktop/testingFolder/Arial.ttf");

	//Bitmap testing
	std::cout << "--Bitmap Testing--" << std::endl;

	Bitmap testBmp;

	testBmp.data = new byte[4*4*sizeof(u32)];
	testBmp.header.fSz = 4*4*sizeof(u32);
	testBmp.header.w = 4;
	testBmp.header.h = 4;
	testBmp.header.bitsPerPixel = 32;
	ZeroMem(testBmp.data, testBmp.header.fSz);

	BitmapGraphics bmpG = BitmapGraphics(&testBmp);

	bmpG.SetColor(255, 0, 255, 255);
	bmpG.DrawPixel(1,1);

	bmpG.SetColor(255, 255, 255, 255);
	bmpG.DrawPixel(2,2);

	BitmapParse::WriteToFile("/Users/jamesweigand/Desktop/testingFolder/testBmpOut.bmp", &testBmp);
	delete[] testBmp.data;

	Bitmap ttfBitmap;

	Glyph tGlyph = ttfParse::ReadTTFGlyph("/Users/jamesweigand/Desktop/testingFolder/Arial.ttf", 19u);
	
	ttfRender::RenderGlyphToBitmap(tGlyph, &ttfBitmap, 0.1f);

	i32 errCode = BitmapParse::WriteToFile("/Users/jamesweigand/Desktop/testingFolder/testGlyph.bmp", &ttfBitmap);

	std::cout << "BMP ERROR: " << errCode << std::endl;

	return 0;
} 

//#endif //ifndef MSFL_EXPORTS