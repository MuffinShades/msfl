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

Point ScalePoint(Point p, float s) {
    return {
        p.x * s,
        p.y * s
    };
}

void DrawPoint(BitmapGraphics *g, float x, float y) {
    float xf = floor(x), xc = ceil(x),
          yf = floor(y), yc = ceil(y);

    float cIx = xc - x, fIx = x - xf,
          cIy = yc - y, fIy = y - yf;

    float i = ((cIx + fIx) / 2.0f + (cIy + fIy) / 2.0f) / 2.0f;

    i *= 255.0f;

    fIx *= 255.0f;
    fIy *= 255.0f;

    g->SetColor(fIx,fIx,fIx, 255);
    g->DrawPixel(x-1, y);

    g->SetColor(cIx,cIx,cIx, 255);
    g->DrawPixel(x, y);
}

i32 ttfRender::RenderGlyphToBitmap(Glyph tGlyph, Bitmap *bmp, float scale) {
    i32 mapW = tGlyph.xMax - tGlyph.xMin,
          mapH = tGlyph.yMax - tGlyph.yMin;

    if (scale <= 0.0f)
        return 1;

    mapW *= scale;
    mapH *= scale;

    mapW+=50;
    mapH+=50;

    bmp->header.w = mapW;
    bmp->header.h = mapH;
    bmp->data = new byte[mapW * mapH * sizeof(u32)];
    bmp->header.fSz = mapW * mapH * sizeof(u32);

    ZeroMem(bmp->data, bmp->header.fSz);
 
    //render the glyph
    //gonna render white for now

    size_t onCurve = 0, offCurve = 0;

    const float nSteps = 150.0f, invStep = 1.0f / nSteps;

    BitmapGraphics g(bmp);

    bool contourStart = false;
    size_t currentContour = 0;

    std::vector<Point> fPoints;
    std::vector<i32> fFlags;

    //first add implied points
    for (size_t i = 0; i < tGlyph.nPoints; i++) {
        fPoints.push_back(tGlyph.points[i]);
        fFlags.push_back(tGlyph.flags[i]);

        i32 pFlag = tGlyph.flags[i];
        Point p = tGlyph.points[i];

        if (i == tGlyph.contourEnds[currentContour] || i >= tGlyph.nPoints - 1) {
            if (currentContour > 0) {
                size_t cPos = tGlyph.contourEnds[currentContour-1]+1;
                fPoints.push_back(tGlyph.points[cPos]);
                assert(cPos < tGlyph.nPoints);
                fFlags.push_back(tGlyph.flags[cPos]);
            }
            else {
                fPoints.push_back(tGlyph.points[0]);
                fFlags.push_back(tGlyph.flags[0]);
            }
            currentContour++;
            continue;
        }

        u32 oCurve;

        if (
            i < tGlyph.nPoints - 1 && 
            (oCurve = GetFlagValue(pFlag, PointFlag_onCurve)) == GetFlagValue(tGlyph.flags[i+1], PointFlag_onCurve)
        ) {
            //add implied point
            fPoints.push_back(pLerp(p, tGlyph.points[i+1], 0.5f));
            fFlags.push_back(ModifyFlagValue(pFlag, PointFlag_onCurve, !oCurve));
        }
    }

    srand(time(NULL));

    for (size_t i = 0; i < fPoints.size(); i++) {
        std::cout << offCurve << " " << onCurve << std::endl;

        i32 pFlag = fFlags[i];
        Point p = fPoints[i];

        if ((bool)GetFlagValue(pFlag, PointFlag_onCurve)) {
            g.SetColor(255,255,255,255);
            if (i == 0)
                g.SetColor(255,255,0,255);
            g.DrawPixel(p.x * scale - tGlyph.xMin * scale, p.y * scale - tGlyph.yMin * scale);

            if (onCurve == 0 && offCurve == 0) {
                onCurve++;
                continue;
            }

            //then just draw a straight line
            if (offCurve == 0) {
                //TODO: change this to a line renderer
                
                std::cout << "DRAWING STRAIGHT LINE" << std::endl;
                offCurve = (onCurve = 0);
                continue;
            }

            g.SetColor(rand()%255,rand()%255,rand()%255, 255.0f);

            //else draw le curve
            for (float t = 0.0f; t < 1.0f; t += invStep) {
                Point np = bezier3(
                    ScalePoint(fPoints[i - 2], scale), 
                    ScalePoint(fPoints[i - 1], scale), 
                    ScalePoint(p, scale), 
                    t
                );

                //g.SetColor((np.x / (float)mapW) * 255.0f, (np.y / (float)mapH) * 255.0f, 255, 255);
                //DrawPoint(&g, np.x - tGlyph.xMin * scale, np.y - tGlyph.yMin * scale);
                //g.SetColor(128.0f, 128.0f, 128.0f, 255.0f);
                g.DrawPixel(np.x - tGlyph.xMin * scale, np.y - tGlyph.yMin * scale);
            }

            offCurve = onCurve = 0;
        } else {
            offCurve++;
            g.SetColor((i / (float) fPoints.size()) * 255.0f,0.0f,255.0f - ((i / (float) fPoints.size()) * 255.0f),255);
            g.DrawPixel(p.x * scale, p.y * scale);
        }
    }
    
    return 0;
}