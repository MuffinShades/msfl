#include "ttfRender.hpp"
#include "msutil.hpp"
#include "bitmapRender.hpp"
#include <vector>

/**
 * 
 * All le code for rendering dem bezier curves
 * 
 * Written by muffinshades 2024
 * 
 */
Point pLerp(Point p0, Point p1, float t) {
    return {
        lerp(p0.x, p1.x, t),
        lerp(p0.y, p1.y, t)
    };
}

Point bezier3(Point p0, Point p1, Point p2, float t) {
    Point i0 = pLerp(p0, p1, t),
          i1 = pLerp(p1, p2, t);
    
    return pLerp(i0, i1, t);
}

Point bezier4(Point p0, Point p1, Point p2, Point p3, float t) {
    Point i0 = pLerp(p0, p1, t),
          i1 = pLerp(p1, p2, t),
          i2 = pLerp(p2, p3, t),
          ii0 = pLerp(i0, i1, t),
          ii1 = pLerp(i1, i2, t);

    return pLerp(ii0, ii1, t);
}

#include <cassert>

Point bezier(std::vector<Point> points, float t) {
    assert(points.size() >= 3);
    std::vector<Point> *i = new std::vector<Point>();
    std::vector<Point> *tg = &points;
    size_t itr = 0;
    do {
        const size_t tlp = tg->size() - 1;
        for (size_t p = 0; p < tlp; p++)
            i->push_back(
                pLerp((*tg)[p], (*tg)[p + 1], t)
            );
        if (itr > 0)
            delete tg;
        tg = i;
        i = new std::vector<Point>();
        itr++;
    } while (tg->size() > 1);

    Point r = (*tg)[0];
    delete tg, i;
    return r;
};

i32 ttfRender::RenderGlyphToBitmap(Glyph tGlyph, Bitmap *bmp, float scale) {
    i32 mapW = tGlyph.xMax - tGlyph.xMin,
          mapH = tGlyph.yMax - tGlyph.yMin;

    if (scale <= 0.0f)
        return 1;

    mapW *= scale;
    mapH *= scale;

    bmp->header.w = mapW;
    bmp->header.h = mapH;
    bmp->data = new byte[mapW * mapH * sizeof(u32)];
    bmp->header.fSz = mapW * mapH * sizeof(u32);

    ZeroMem(bmp->data, bmp->header.fSz);

    //std::cout << bmp->header.w << " " << bmp->header.h << std::endl;
 
    //render the glyph
    //gonna render white for now

    size_t onCount = 0, offCount = 0;

    const float nSteps = 150.0f * scale, invStep = 1.0f / nSteps;

    BitmapGraphics g(bmp);

    bool contourStart = false;
    size_t currentContour = 0;

    for (size_t p = 1; p < tGlyph.nPoints; p++) {
        for (float v = 0.0f; v < 1.0f; v += invStep) {
            Point pos = pLerp({tGlyph.points[p].x * scale, tGlyph.points[p].y * scale}, {tGlyph.points[p - 1].x * scale, tGlyph.points[p - 1].y * scale}, v);
            g.SetColor(255,255,255,255);
            g.DrawPixel((u32)pos.x, (u32)pos.y);

            //std::cout << "Pos: " << pos.x << " " << pos.y << std::endl;
        }
    }
    
    return 0;
}