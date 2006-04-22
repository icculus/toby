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

#ifndef _INCLUDE_NULLTURTLESPACERENDERER_H_
#define _INCLUDE_NULLTURTLESPACERENDERER_H_

#include "turtlespace/TurtleSpaceRenderer.h"

/*
 * A Null TurtleSpaceRenderer. Use this for profiling the rest of the
 *  program without graphic rendering overhead. Also good for getting
 *  the rest of Toby up on a system before you've written a proper
 *  platform driver, or for systems where you want the interpreter engine
 *  without graphic output.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class NullTurtleSpaceRenderer : public TurtleSpaceRenderer
{
public:
    NullTurtleSpaceRenderer(void);
    virtual ~NullTurtleSpaceRenderer(void);

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
    virtual void renderToOffscreen(void);
    virtual void renderToScreen(void);

    virtual bool doEvents(void);

private:
    double w;
    double h;
}; // class NullTurtleSpaceRenderer

#endif // !defined _INCLUDE_NULLTURTLESPACERENDERER_H_

// end of NullTurtleSpaceRenderer.h ...

