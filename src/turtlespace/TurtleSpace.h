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

#ifndef _INCLUDE_TURTLESPACE_H_
#define _INCLUDE_TURTLESPACE_H_

#include "util/TobyObject.h"
#include "util/TobyLanguage.h"
#include "util/TobyMutex.h"
#include "util/TobyStack.h"
#include "util/TobyGeometry.h"
#include "turtlespace/Turtle.h"
#include "turtlespace/TurtleSpaceRenderer.h"

/*
 *  This is TurtleSpace, the interpreted program's interface to the
 *   screen. This specific class has no physical representation; the actual
 *   widget/window/whatever is managed by an implementation of the
 *   TurtleSpaceRenderer class, which takes commands from this class and
 *   gets the bits to the screen. Besides being the abstract link between
 *   the renderer and the interpreted program, TurtleSpace handles Turtle
 *   maintenance and other TurtleGraphics-related state.
 *
 *   Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class TurtleSpace : public TobyObject
{
public:
    TurtleSpace(TurtleSpaceRenderer *_renderer);
    virtual ~TurtleSpace(void);

        // Basic TurtleSpace information queries.
    TurtleSpaceRenderer *getTurtleSpaceRenderer(void);
    double getTurtleSpaceWidth(void);
    double getTurtleSpaceHeight(void);
    double getDueNorth(void);
    void   setDueNorth(double angle);
    static float getDefaultPenRed(void)    { return(1.0); }
    static float getDefaultPenGreen(void)  { return(1.0); }
    static float getDefaultPenBlue(void)   { return(1.0); }
    static float getDefaultPenAlpha(void)  { return(1.0); }

        // Turtle pool manipulation.
    int getTurtleCount(void);
    int addTurtle(void) throw (ExecException *);
    bool removeTurtle(int turtleIndex) throw (ExecException *);
    void removeAllTurtles(void) throw (ExecException *);

        // Rendering state-machine interface.
    void grabTurtleSpace(void);
    void releaseTurtleSpace(void);

        // Fence state-machine interface.
    void enableFence(void) throw (ExecException *);
    void disableFence(void) throw (ExecException *);
    void setFence(bool buildFence) throw (ExecException *);

        // Turtle state-machine interface.
    void useTurtle(int turtleIndex) throw (ExecException *);
    double getTurtleX(void) throw (ExecException *);
    double getTurtleY(void) throw (ExecException *);
    double getTurtleWidth(void) throw (ExecException *);
    double getTurtleHeight(void) throw (ExecException *);
    void setTurtleX(double x) throw (ExecException *);
    void setTurtleY(double y) throw (ExecException *);
    void setTurtleXY(double x, double y) throw (ExecException *);
    void setTurtleWidth(double newWidth) throw (ExecException *);
    void setTurtleHeight(double newHeight) throw (ExecException *);
    void setTurtleVisibility(bool isVis) throw (ExecException *);
    void showTurtle(void) throw (ExecException *);
    void showAllTurtles(void) throw (ExecException *);
    void hideTurtle(void) throw (ExecException *);
    void hideAllTurtles(void) throw (ExecException *);
    void advanceTurtle(double distance) throw (ExecException *);
    void setTurtleAngle(double angle) throw (ExecException *);
    void rotateTurtle(double degree) throw (ExecException *);
    void homeTurtle(void) throw (ExecException *);
    void homeAllTurtles(void) throw (ExecException *);
    void defaultTurtle(void) throw (ExecException *);
    void defaultAllTurtles(void) throw (ExecException *);
    void setPenColor(float r, float g, float b, float a) throw (ExecException *);
    void setPen(bool drawing) throw (ExecException *);
    void setPenUp(void) throw (ExecException *);
    void setPenDown(void) throw (ExecException *);

        // miscellaneous rendering stuff.
    void drawString(const char *str) throw (ExecException *);
    void cleanup(void) throw (ExecException *);

        // When this is called, all rendering is done to the offscreen buffer,
        //  and never to the screen. The turtle should never be rendered to
        //  the offscreen buffer. Screen rendering should commence again when
        //  renderToScreen() is called.
    virtual void renderToOffscreen(void);

        // When this is called, the offscreen buffer should be blitted/flipped
        //  to the screen, and further rendering (at least, until the next
        //  renderToOffscreen() call) should be done to the screen and,
        //  optionally, the offscreen buffer as well.
    void renderToScreen(void);

        // returns (true) if rendering is only being done in the offscreen
        //  buffer. Returns (false) if rendering is also going to the screen.
    virtual bool isRenderingToOffscreen(void);

        // returns (true) if rendering is being done on the video device.
    virtual bool isRenderingToScreen(void);

protected:
    Turtle *getTurtleByIndex(int turtleIndex);
    Turtle *buildNewTurtle(void) throw (ExecException *);
    void homeTurtle(Turtle *t) throw (ExecException *);
    void defaultTurtle(Turtle *t) throw (ExecException *);
    void verifyPointInsideFence(double x, double y) throw (ExecException *);
    void setTurtleVisibility(Turtle *t, bool isVis) throw (ExecException *);

private:
    double dueNorth;
    Turtle **turtles;
    int turtlesArraySize;
    TobyStack *turtlePool;
    bool fenceEnabled;
    Turtle *turtle;
    TurtleSpaceRenderer *renderer;
    TobyMutex *turtleSync;
    bool renderingToOffscreen;
}; // class TurtleSpace

#endif // !defined _INCLUDE_TURTLESPACE_H_

// end of TurtleSpace.h ...

