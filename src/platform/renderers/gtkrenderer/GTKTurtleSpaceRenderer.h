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

#ifndef _INCLUDE_GTKTURTLESPACERENDERER_H_
#define _INCLUDE_GTKTURTLESPACERENDERER_H_

#include <gtk/gtk.h>
#include "turtlespace/TurtleSpaceRenderer.h"
#include "util/TobyGeometry.h"

/*
 * A GTK+-based TurtleSpaceRenderer. GTK+ and related libraries can be
 *  found at http://www.gtk.org/.
 *
 *     Written by Ryan C. Gordon. (icculus@linuxgames.com)
 */
class GTKTurtleSpaceRenderer : public TurtleSpaceRenderer
{
public:
    GTKTurtleSpaceRenderer(void);
    virtual ~GTKTurtleSpaceRenderer(void);

    virtual bool resize(int _w, int _h);
    virtual void notifyGrabbed(void);
    virtual void notifyUngrabbed(void);
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

    virtual GtkWidget *getDrawingAreaWidget(void);
    virtual GdkPixmap *getOffscreenPixmap(void);

protected:
        // This returns a 32-bit value based on four floats that represent
        //  intensity of the color channels in the range of 0.0 to 1.0.
    inline guint32 constructPixelValue(float r, float g, float b, float a);

private:
    GdkGC *gc;
    GdkPixmap *offscreen_pixmap;
    GtkWidget *canvas;
}; // class GTKTurtleSpaceRenderer

#endif // !defined _INCLUDE_GTKTURTLESPACERENDERER_H_

// end of GTKTurtleSpaceRenderer.h ...

