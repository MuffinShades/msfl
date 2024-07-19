#include <iostream>
#include "FileWriter.h"
#include "ByteStream.hpp"
#include "ttfRender.hpp"

typedef long long ttfLongDateTime;

enum iTable {
    iTable_glyph,
    iTable_head,
    iTable_loca,
    iTable_cmap,
    iTable_hmtx,
    iTable_NA
};

//ttf file stuff starts here
struct offsetTable {
    u32 checkSum;
    size_t off;
    size_t len;
    std::string tag;
    enum iTable iTag = iTable_NA;
};

/**
 * 
 * All the structs for a ttf file
 * 
 * yeah it's a lot
 * 
 */
//head
struct table_head {
    float fVersion;
    float fontRevision;
    u32 checkSumAdjust;
    u32 magic;
    u16 flags;
    u16 unitsPerEm;
    ttfLongDateTime created;
    ttfLongDateTime modified;
    float xMin, yMin, xMax, yMax;
    u16 macStyle;
    u16 lowestRecPPEM;
    i16 fontDirectionHint;
    i16 idxToLocFormat;
    i16 glyphDataFormat;
};

class ttfStream : public ByteStream {
public:
    short readFWord();
    unsigned short readUFWord();
    float readF2Dot14();
    float readFixed();
    std::string readString(size_t sz);
    ttfLongDateTime readDate();
    using ByteStream::ByteStream;
};

struct Glyph {
    i16 nContours;
    float xMin, yMin, xMax, yMax;
    Point *points = nullptr;
    byte *flags = nullptr;
    i32 *contourEnds = nullptr;
    size_t nPoints;
};

enum CMapMode {
    CMap_Unicode,
    CMap_Mac,
    CMap_Reserved,
    CMap_Microsoft,
    CMap_null
};

class ttfFile {
public:
    u32 scalarType;
    u16 searchRange;
    u16 entrySelector;
    u16 rangeShift;
    offsetTable head_table;
    offsetTable loca_table;
    offsetTable glyph_table;
    offsetTable cmap_table;
    CMapMode charMapMode = CMap_null;
    i32 charMapModeId = 0;
    std::vector<offsetTable> tables;
    table_head header;
};

class ttfParse {
public:
    static ttfFile ParseTTFFile(std::string src);
    static ttfFile ParseBin(byte *dat, size_t sz);
};