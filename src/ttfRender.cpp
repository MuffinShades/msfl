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

const static float epsilon = 0.00001f;
const static float invEpsilon = 1 / epsilon;
#define EPSILIZE(v) (floor((v)*invEpsilon)*epsilon)

struct f_roots {
    float r0, r1;
    i32 nRoots = 0;
};

f_roots getRoots(float a, float b, float c) {
    if (EPSILIZE(a) == 0.0f) 
        return {
            .nRoots = 0
        };
    const float root = EPSILIZE(((b*b) - (4*a*c))), ida = 1 / (2 * a);
    if (root < 0.0f)
        return {
            .nRoots = 0
        };
    if (root != 0.0f)
        return {
            .r0 = (-b + root) * ida,
            .r1 = (-b - root) * ida,
            .nRoots = 2
        };
    else
        return {
            .r0 = (-b + root) * ida,
            .nRoots = 1
        };
}

i32 intersectsCurve(Point p0, Point p1, Point p2, Point e) {
    p0.y -= e.y;
    p1.y -= e.y;
    p2.y -= e.y;
    const float a = p0.y - 2 * p1.y + p2.y, b = 2 * p1.y - 2 * p0.y, c = p0.y;

    f_roots _roots = getRoots(a, b, c);

    if (_roots.nRoots <= 0) return 0; //no roots so no intersection

    //check le roots
    i32 nRoots = 0;

    if (_roots.r0 >= 0.0f && _roots.r0 <= 1.0f) nRoots++;
    if (_roots.nRoots > 1 && _roots.r1 >= 0.0f && _roots.r1 <= 1.0f) nRoots++;

    return nRoots;
}

i32 ttfRender::RenderGlyphToBitmap(Glyph tGlyph, Bitmap *bmp, float scale) {
    i32 mapW = tGlyph.xMax - tGlyph.xMin,
        mapH = tGlyph.yMax - tGlyph.yMin;

    if (scale <= 0.0f)
        return 1;

    mapW *= scale;
    mapH *= scale;

    mapW ++;
    mapH ++;

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
            size_t cPos = currentContour > 0 ? tGlyph.contourEnds[currentContour-1]+1 : 0;
            assert(cPos < tGlyph.nPoints);

            //check for an implied point
            i32 flg = tGlyph.flags[cPos];
            bool oc = GetFlagValue(flg, PointFlag_onCurve);

            //add implied point in-between if needed
            if (oc == GetFlagValue(pFlag, PointFlag_onCurve)) {
                fPoints.push_back(pLerp(p, tGlyph.points[cPos], 0.5f));
                fFlags.push_back(ModifyFlagValue(pFlag, PointFlag_onCurve, !oc));
            }

            //add point
            fPoints.push_back(tGlyph.points[cPos]);
            fFlags.push_back(flg);
            
            currentContour++;
#ifdef MSFL_TTFRENDER_DEBUG
            std::cout << "Finished Contour: " << currentContour << " / " << tGlyph.nContours << std::endl;
#endif
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

#ifdef MSFL_TTFRENDER_DEBUG
    srand(time(NULL));
#endif

    for (size_t i = 0; i < fPoints.size(); i++) {
        i32 pFlag = fFlags[i];
        Point p = fPoints[i];

        if ((bool)GetFlagValue(pFlag, PointFlag_onCurve)) {
#ifdef MSFL_TTFRENDER_DEBUG
            g.SetColor(255,255,255,255);
            if (i == 0)
                g.SetColor(255,255,0,255);
            g.DrawPixel(p.x * scale - tGlyph.xMin * scale, p.y * scale - tGlyph.yMin * scale);
#endif

            if (onCurve == 0 && offCurve == 0) {
                onCurve++;
                continue;
            }

            //there shouldnt be a stright line so...
            if (offCurve == 0) 
                continue;

            g.SetColor(255.0f, 255.0f, 255.0f, 255.0f);

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
#ifdef MSFL_TTFRENDER_DEBUG
            g.SetColor((i / (float) fPoints.size()) * 255.0f,0.0f,255.0f - ((i / (float) fPoints.size()) * 255.0f),255);
            g.DrawPixel(p.x * scale, p.y * scale);
#endif
        }
    }
    
    return 0;
}