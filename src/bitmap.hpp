#pragma once
#include <iostream>
#include "ByteStream.hpp"
#include "FileWriter.h"

struct BitmapHeader {
    size_t fSz = 0;
    i16 bmpSig = 0;
    size_t w = 0, h = 0;
    i32 compressionMode = 0;
    u16 colorPlanes = 1;
    u16 bitsPerPixel = 32;
    i32 vResolution = 1, hResolution = 1;
    size_t nPalleteColors = 0;
    size_t importantColors = 0;
};

class Bitmap {
public:
    BitmapHeader header;
    byte *data;
    static Bitmap CreateBitmap(i32 w, i32 h);
};

class BitmapParse {
public:
    static i32 WriteToFile(std::string src, Bitmap *bmp);
};