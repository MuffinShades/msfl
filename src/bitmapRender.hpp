#include "bitmap.hpp"

/**
 *
 * Super simple bitmap graphics class
 * 
 * Used for drawing pixels and stuff to a bitmap.
 * Gonna use for testing and ttf rendering to a
 * bitmap.
 * 
 */
struct _bmpColor {
    byte r, g, b, a;
};

class BitmapGraphics {
private:
    Bitmap *bmp;
    _bmpColor tColor = {0,0,0,0};
public:
    BitmapGraphics(Bitmap *bmp = nullptr) : bmp(bmp) {}
    void SetColor(byte r = 0, byte g = 0, byte b = 0, byte a = 0);
    void DrawPixel(u32 x, u32 y);
    void ClearColor();
    void Clear();
};