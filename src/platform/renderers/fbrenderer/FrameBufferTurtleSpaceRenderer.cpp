/*
 * TOBY -- An abstract interpreter engine and system for learning.
 * Copyright (C) 1999  Ryan C. Gordon.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "platform/renderers/fbrenderer/FrameBufferTurtleSpaceRenderer.h"

FrameBufferTurtleSpaceRenderer::FrameBufferTurtleSpaceRenderer(void)
    : TurtleSpaceRenderer(), defaultSurface(NULL)
{
} // Constructor


FrameBufferTurtleSpaceRenderer::~FrameBufferTurtleSpaceRenderer(void)
{
    deleteSurface();
} // Destructor


bool FrameBufferTurtleSpaceRenderer::resize(int _w, int _h)
{
    if (screenResize(_w, _h) == false)
        return(false);

    toby_uint32 *oldbuf = NULL;
    toby_uint32 *p = getSurface();

    // need an extra copy so we don't createSurface() twice in a row.
    if (p != NULL)
    {
        oldbuf = new toby_uint32[w * h];
        memcpy(oldbuf, p, (w * h) * sizeof (toby_uint32));
        deleteSurface();
    } // if

    createSurface(_w, _h);
    p = getSurface();
    if (p == NULL)
    {
        delete[] oldbuf;
        return(false);
    } // if

    blankPixels(p, _w, _h);
    if (oldbuf != NULL)
    {
        // !!! copy old framebuffer to new, centered.
        delete[] oldbuf;
    } // if

    w = _w;
    h = _h;
    return(true);
} // FrameBufferTurtleSpaceRenderer::resize


void FrameBufferTurtleSpaceRenderer::notifyGrabbed(void)
{
} // FrameBufferTurtleSpaceRenderer::notifyGrabbed(void)


void FrameBufferTurtleSpaceRenderer::notifyUngrabbed(void)
{
} // FrameBufferTurtleSpaceRenderer::notifyUngrabbed


void FrameBufferTurtleSpaceRenderer::blankPixels(toby_uint32 *p, int _w, int _h)
{
        // !!! This could optimize, I bet.
    int max = _w * _h;
    for (int i = 0; i < max; i++)
        p[i] = 0xFF000000;  // Full alpha, no red or green or blue.
} // FrameBufferTurtleSpaceRenderer::blankPixels


double FrameBufferTurtleSpaceRenderer::getTurtleSpaceWidth(void)
{
    return(w);
} // FrameBufferTurtleSpaceRenderer::getTurtleSpaceWidth


double FrameBufferTurtleSpaceRenderer::getTurtleSpaceHeight(void)
{
    return(h);
} // FrameBufferTurtleSpaceRenderer::getTurtleSpaceHeight


double FrameBufferTurtleSpaceRenderer::getDesiredTurtleWidth(void)
{
    return(w * 0.02);
} // FrameBufferTurtleSpaceRenderer::getDesiredTurtleWidth


double FrameBufferTurtleSpaceRenderer::getDesiredTurtleHeight(void)
{
    return(getDesiredTurtleWidth());
} // FrameBufferTurtleSpaceRenderer::getDesiredTurtleHeight


void FrameBufferTurtleSpaceRenderer::renderString(double x, double y,
                                                  double angle,
                                                  float r, float b,
                                                  float g, float a,
                                                    const char *str)
{
} // FrameBufferTurtleSpaceRenderer::renderString


#define LINEBLITCHUNKS 10

void FrameBufferTurtleSpaceRenderer::renderLine(double _x1, double _y1,
                                                double _x2, double _y2,
                                                float r, float b,
                                                float g, float a)
{
    // This is a standard Bresenham line-drawing algorithm, but the specific
    //  implementation was borrowed, optimized, and mangled from
    //  SGE's DoLine code:
    //   http://www.etek.chalmers.se/~e8cal1/sge/

    toby_uint32 pval = constructPixelValue(r, g, b, a);
    int x1 = TobyGeometry::roundDoubleToInt(_x1);
    int y1 = TobyGeometry::roundDoubleToInt(_y1);
    int x2 = TobyGeometry::roundDoubleToInt(_x2);
    int y2 = TobyGeometry::roundDoubleToInt(_y2);

    FrameBufferRect rects[LINEBLITCHUNKS];
    toby_uint32 *p = getSurface();

    _D(("LFB: rendering line...(%d, %d)-(%d, %d), (%f, %f, %f, %f)...\n",
        x1, y1, x2, y2, r, g, b, a));

	int dx, dy, sdx, sdy, x, y, px, py, incr;

    dx = x2 - x1;
    dy = y2 - y1;

    sdx = (dx < 0) ? -1 : 1;
    sdy = (dy < 0) ? -1 : 1;

    dx = sdx * dx + 1;
    dy = sdy * dy + 1;

    x = y = 0;

    px = x1;
    py = y1;

    if ((dx == 1) && (dy == 1))
    {
        _D(("LFB: Single pixel line fastpath.\n"));
        *(((toby_uint32 *) p) + ((py * w) + px)) = pval;
        putToScreen(p, px, py, 1, 1);
        return;
    } // if

    else if (dx == 1)
    {
        _D(("LFB: Vertical line fastpath.\n"));
        toby_uint32 *linep = ((toby_uint32 *) p) + ((py * w) + px);
        incr = w * sdy;
        for (y = 0; y < dy; y++)
        {
            *linep = pval;
            linep += incr;
        } // for
        putToScreen(p, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, 1, dy);
        return;
    } // else if

    else if (dy == 1)
    {
        _D(("LFB: Horizontal line fastpath.\n"));
        toby_uint32 *linep = ((toby_uint32 *) p) + ((py * w) + px);
        incr = sdx;
        for (x = 0; x < dx; x++)
        {
            *linep = pval;
            linep += incr;
        } // for
        putToScreen(p, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, dx, 1);
        return;
    } // else if

    else if (dx >= dy)
    {
        _D(("LFB: Line with major axis of X.\n"));

        int lastX = px;
        int thisY = py;
        int rectidx = 0;

        for (x = 0; x < dx; x++)
        {
            *( ((toby_uint32 *) p) + ((py * w) + px) ) = pval;

            y += dy;
            if (y >= dx)   // run length completed.
            {
                if (rectidx != -1)
                {
                    rects[rectidx].x = (lastX < px) ? lastX : px;
                    rects[rectidx].y = thisY;
                    rects[rectidx].w = (px - lastX) * sdx;
                    rects[rectidx].h = 1;

                    rectidx++;
                    if (rectidx >= LINEBLITCHUNKS)
                        rectidx = -1;
                } // if

                y -= dx;
                py += sdy;

                lastX = px;
                thisY = py;
            } // if
            px += sdx;
        } // for

        if (rectidx != -1)
            putMultToScreen(p, rectidx, rects);
        else
            goto tobylfb_blit_slanted_line;

        return;
    } // else if

    else
    {
        _D(("LFB: Line with major axis of Y.\n"));

        int lastY = py;
        int thisX = px;
        int rectidx = 0;

        for (y = 0; y < dy; y++)
        {
            *( ((toby_uint32 *) p) + ((py * w) + px) ) = pval;

            x += dx;
            if (x >= dy)  // run length completed.
            {
                if (rectidx != -1)
                {
                    rects[rectidx].x = thisX;
                    rects[rectidx].y = (lastY < py) ? lastY : py;
                    rects[rectidx].w = 1;
                    rects[rectidx].h = (py - lastY) * sdy;

                    rectidx++;
                    if (rectidx >= LINEBLITCHUNKS)
                        rectidx = -1;
                } // if

                x -= dy;
                px += sdx;

                lastY = py;
                thisX = px;
            } // if
            py += sdy;
        } // for

        if (rectidx != -1)
            putMultToScreen(p, rectidx, rects);
        else
            goto tobylfb_blit_slanted_line;

        return;
    } // else

    assert(0);  // should have return'd or goto'd by here.

tobylfb_blit_slanted_line:
        // !!! This isn't rendering right in all cases.

    float chunkX = ((float) dx) / ((float) LINEBLITCHUNKS);
    float chunkY = ((float) dy) / ((float) LINEBLITCHUNKS);
    float yincr;
    float fx, fy;

    fx = (float) ((x1 < x2) ? x1 : x2);

    if ( ((sdx > 0) && (sdy > 0)) ||   // line from q1 to q4.
         ((sdx < 0) && (sdy < 0)) )
    {
        fy = (float) ((y1 < y2) ? y1 : y2);
        yincr = chunkY;
    } // if

    else   // line from q3 to q2.
    {
        fy = (float) (((y1 > y2) ? y1 : y2)) - chunkY;
        yincr = -chunkY;
    } // else

    for (int i = 0; i < LINEBLITCHUNKS; i++)
    {
        rects[i].x = (int) (fx);
        rects[i].y = (int) (fy);
        rects[i].w = (int) (chunkX + 0.5);
        rects[i].h = (int) (chunkY + 0.5);

        if (rects[i].w == 0)
            rects[i].w++;

        if (rects[i].h == 0)
            rects[i].h++;

        fx += chunkX;
        fy += yincr;
    } // for

    putMultToScreen(p, LINEBLITCHUNKS, rects);
} // FrameBufferTurtleSpaceRenderer::renderLine


void FrameBufferTurtleSpaceRenderer::renderTurtle(Turtle *turtle) throw (ExecException *)
{
    _D(("LFB: renderTurtle() called. NOT IMPLEMENTED.\n"));
} // FrameBufferTurtleSpaceRenderer::renderTurtle


void FrameBufferTurtleSpaceRenderer::blankTurtle(Turtle *turtle) throw (ExecException *)
{
    _D(("LFB: blankTurtle() called. NOT IMPLEMENTED.\n"));
} // FrameBufferTurtleSpaceRenderer::blankTurtle


void FrameBufferTurtleSpaceRenderer::cleanup(void) throw (ExecException *)
{
    blankPixels(getSurface(), w, h);
} // FrameBufferTurtleSpaceRenderer::cleanup


inline toby_uint32 FrameBufferTurtleSpaceRenderer::constructPixelValue(
                                                                float r,
                                                                float g,
                                                                float b,
                                                                float a)
{
        // !!! check this.
    return(
            (((int) (a * 255.0)) << 24) |
            (((int) (r * 255.0)) << 16) |
            (((int) (b * 255.0)) <<  8) |
            (((int) (g * 255.0))      )
          );

#if 0
        _D(("%f, %f, %f, %f == ", r, b, g, a));

        for (unsigned int i = 1 << 31; i != 0; i >>= 1)
            _D(("%d", (int) ((pval & i) / i)));

        _D(("\n"));
#endif

} // FrameBufferTurtleSpaceRenderer::constructPixelValue


void FrameBufferTurtleSpaceRenderer::createSurface(int _w, int _h)
{
    defaultSurface = new toby_uint32[_w * _h];
} // FrameBufferTurtleSpaceRenderer::createSurface


void FrameBufferTurtleSpaceRenderer::deleteSurface(void)
{
    delete[] defaultSurface;
} // FrameBufferTurtleSpaceRenderer::deleteSurface


toby_uint32 *FrameBufferTurtleSpaceRenderer::getSurface(void)
{
    return(defaultSurface);
} // FrameBufferTurtleSpaceRenderer::getSurface


void FrameBufferTurtleSpaceRenderer::putMultToScreen(toby_uint32 *pix,
                                                     int rectCount,
                                                     FrameBufferRect *rects)
{
    for (int i = 0; i < rectCount; i++)
       putToScreen(pix, rects[i].x, rects[i].y, rects[i].w, rects[i].h);
} // FrameBufferTurtleSpaceRenderer::putMultToScreen

// end of FrameBufferTurtleSpaceRenderer.cpp ...

