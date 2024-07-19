#pragma once
#include "ttf.hpp"
#include "bitmap.hpp"

class ttfRender {
public:
    static i32 RenderGlyphToBitmap(Glyph tGlyph, Bitmap *bmp, float scale = 1.0f);
};