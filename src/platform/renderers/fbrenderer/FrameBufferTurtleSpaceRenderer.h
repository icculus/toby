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

#ifndef _INCLUDE_FRAMEBUFFERTURTLESPACERENDERER_H_
#define _INCLUDE_FRAMEBUFFERTURTLESPACERENDERER_H_

#include "turtlespace/TurtleSpaceRenderer.h"
#include "util/TobyGeometry.h"


typedef struct __STRUCT_FrameBufferRect
{
    int x;
    int y;
    int w;
    int h;
} FrameBufferRect;

/*
 * An implementation of TurtleSpaceRenderer that writes to a linear
 *  framebuffer in memory. Internally, this framebuffer is 32-bits per
 *  pixel: 8 bits each for red, green, blue, and alpha. Rendering into
 *  this framebuffer is done within this class, which expects a subclass
 *  to take responsibility for getting the bits to the screen. The subclass
 *  is also responsible for converting the 32-bit format to their visual's
 *  format, including alpha-blending.
 *
 *     Written by Ryan C. Gordon. (icculus@linuxgames.com)
 */
class FrameBufferTurtleSpaceRenderer : public TurtleSpaceRenderer
{
public:
    FrameBufferTurtleSpaceRenderer(void);
    virtual ~FrameBufferTurtleSpaceRenderer(void);

        // Do NOT override this. This base class will call screenResize(),
        //  where you can perform your own setup.
    virtual bool resize(int _w, int _h);

        // Grab notifications are currently no-ops in this class, but if you
        //  override them, please call the superclass's version first, just
        //  in case.
    virtual void notifyGrabbed(void);
    virtual void notifyUngrabbed(void);

        // These will probably not need overriding.
    virtual double getTurtleSpaceWidth(void);
    virtual double getTurtleSpaceHeight(void);
    virtual double getDesiredTurtleWidth(void);
    virtual double getDesiredTurtleHeight(void);
    virtual void renderString(double x, double y, double angle,
                                float r, float b, float g, float a,
                                const char *str);
    virtual void renderLine(double x1, double y1, double x2, double y2,
                                float r, float b, float g, float a);
    virtual void renderTurtle(Turtle *turtle) throw (ExecException *);
    virtual void blankTurtle(Turtle *turtle) throw (ExecException *);
    virtual void cleanup(void) throw (ExecException *);


protected:

        // This returns a 32-bit value based on four floats that represent
        //  intensity of the color channels in the range of 0.0 to 1.0.
    inline toby_uint32 constructPixelValue(float r, float g, float b, float a);

        // Set the screen surface (the window, the fullscreen resolution,
        //  whatever) to (_w) by (_h) pixels. Returns (true) if successful,
        //  (false) otherwise. If this succeeds, this base class will then
        //  attempt to build an offscreen surface (via createSurface()).
        //  You must implement this.
    virtual bool screenResize(int _w, int _h) = 0;

        // Some toolkits can accelerate blits to the screen if they manage
        //  their own offscreen memory surface. In such a case, this method
        //  should be overridden to build a platform-specific surface. The
        //  offscreen surface must be a linear framebuffer, 32-bits per pixel,
        //  in format 0xAARRBBGG (Alpha, Red, Blue, Green). If not overridden,
        //  this base class will allocate a block of memory for itself.
    virtual void createSurface(int _w, int _h);

        // Free resources allocated by createSurface(). The base class will
        //  always call deleteSurface() before calling createSurface() again
        //  (in case of resizing, etc). This, by default, cleans up an
        //  internal buffer created by this base class. You must override
        //  this if you overrode createSurface()!
    virtual void deleteSurface(void);

        // Get the offscreen surface's linear framebuffer. This, by default,
        //  returns the memory buffer allocated by this base class. If
        //  createSurface() could not build an offscreen surface, this method
        //  should return NULL. You must override this if you overrode
        //  createSurface()!
    virtual toby_uint32 *getSurface(void);

        // Take the framebuffer pointed to by (pix) and display the pixels
        //  within the rectangle specified by (x), (y), (w), and (h).
        // You must implement this!
    virtual void putToScreen(toby_uint32 *pix, int x, int y, int w, int h) = 0;

        // If your renderer can accelerate putting multiple rectangles to
        //  the screen at the same time, then override this.
        // The default implementation just calls putToScreen() on each
        //  rectangle in the array.
    virtual void putMultToScreen(toby_uint32 *pix, int rectCount,
                                    FrameBufferRect *rects);

private:
    toby_uint32 *defaultSurface;
    int w;
    int h;

    void blankPixels(toby_uint32 *p, int _w, int _h);
}; // class FrameBufferTurtleSpaceRenderer

#endif // !defined _INCLUDE_FRAMEBUFFERTURTLESPACERENDERER_H_

// end of FrameBufferTurtleSpaceRenderer.h ...

