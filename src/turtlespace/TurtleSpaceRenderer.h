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

#ifndef _INCLUDE_TURTLESPACERENDERER_H_
#define _INCLUDE_TURTLESPACERENDERER_H_

#include "util/TobyObject.h"
#include "exceptions/ExecException.h"
#include "turtlespace/Turtle.h"

/*
 * The TurtleSpaceRenderer interface. This is a pure virtual class, so
 *  you'll need to implement it for your given platform. This class has the
 *  physical representation of TurtleSpace, and gets the bits to the screen.
 *
 * Examples of subclasses would be a Qt implementation, an SDL
 *  implementation, a Win32 implementation, etc.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class TurtleSpaceRenderer : public TobyObject
{
public:
    TurtleSpaceRenderer(void) {}
    virtual ~TurtleSpaceRenderer(void) {}

        // Create a screen surface: windows should be created, or resolutions
        //  should be changed in this method. If the screen surface can not
        //  be created to satisfaction, this should return (false). If the
        //  screen initialization was successful, this should return (true).
        // Note that this method may be called multiple times during the
        //  existance of the object, whether previous calls failed or not.
        //  If there is a resize call after/during rendering (even while
        //  TurtleSpace is "grabbed"), then the renderer should strive to
        //  maintain the currently rendered image in the new size.
    virtual bool resize(int w, int h) = 0;

        // These notification methods are called when TurtleSpace rendering
        //  begins and ends, presumably at program interpretation start and
        //  finish. This is for your renderer's informational purposes only,
        //  and you aren't required to do anything at all in these methods.
    virtual void notifyGrabbed(void) = 0;
    virtual void notifyUngrabbed(void) = 0;

        // Get the dimensions of the current screen surface in pixels.
    virtual double getTurtleSpaceWidth(void) = 0;
    virtual double getTurtleSpaceHeight(void) = 0;

        // Get the preferred default dimensions for a turtle in pixels, which
        //  may or may not be ignored completely. Note the turtle may also be
        //  arbitrarily resized during program interpretation.
    virtual double getDesiredTurtleWidth(void) = 0;
    virtual double getDesiredTurtleHeight(void) = 0;

        // Render a text string to the screen, starting at coordinate (x), (y),
        //  and tilted at (angle), where 270.0 is due north. The color
        //  channels, (r), (b), (g), and (a), are the red, blue, green, and
        //  alpha, are specified in a range of 0.0 (no intensity on that
        //  channel) to 1.0 (full intensity on that channel). (str) is, of
        //  course, the string to render. Extra credit for antialiasing.
    virtual void renderString(double x, double y, double angle,
                                float r, float b, float g, float a,
                                const char *str) = 0;


        // Render a line segment between 2 points, in the specified color.
        //  The color values will be clamped between 0.0 and 1.0, and the
        //  line coordinates will be clamped to be inside the dimensions of
        //  TurtleSpace.
    virtual void renderLine(double x1, double y1, double x2, double y2,
                                float r, float b, float g, float a) = 0;


        // Draw the turtle to the screen. It is the responsibility of the
        //  renderer implementation to maintain the pixels underneath the
        //  turtle, so they can be replaced during the blankTurtle call.
        //
        // The turtle is a triangle, which should be rendered with the points
        //  returned from turtle->getRenderingInts(). See
        //  src/turtlespace/Turtle.h for details. The turtle should never
        //  be rendered to the offscreen image.
    virtual void renderTurtle(Turtle *turtle) throw (ExecException) = 0;

        // Remove the turtle from the screen, and return the pixels that
        //  the turtle had overwritten.
    virtual void blankTurtle(Turtle *turtle) throw (ExecException) = 0;

        // Blank TurtleSpace to full alpha, and zero red, green, and blue.
        //  Turtles that need rerendering will be handled by calls to
        //  the renderTurtle() method after this call.
    virtual void cleanup(void) throw (ExecException) = 0;

        // When this is called, all rendering is done to the offscreen buffer,
        //  and never to the screen. The turtle should never be rendered to
        //  the offscreen buffer. Screen rendering should commence again when
        //  renderToScreen() is called.
    virtual void renderToOffscreen(void) = 0;

        // When this is called, the offscreen buffer should be blitted/flipped
        //  to the screen, and further rendering (at least, until the next
        //  renderToOffscreen() call) should be done to the screen and,
        //  optionally, the offscreen buffer as well.
    virtual void renderToScreen(void) = 0;

        // This gets called to give you a chance to run a toolkit-specific
        //  event queue or other idle-time upkeep you may need. If a quit
        //  event or some other reason to abort the program arises, return
        //  false, otherwise, return true.
    virtual bool doEvents(void) = 0;

        // This gets filled in along with the platform-specific subclass.
        //  It should be a standalone TurtleSpace, or return NULL if it can't.
        //  For example, a GTK+ widget-based TurtleSpace should build a
        //  GtkWindow around it so that it needs nothing else to display.
        //  Width and height are specified in pixels.
        //  Note that various GUI interfaces will just instantiate their
        //  rendering subclass directly, so they can have an embedded renderer.
    static TurtleSpaceRenderer *buildStandalone(const char *winTitle,
                                                int *argc, char ***argv);
}; // class TurtleSpaceRenderer

#endif // !defined _INCLUDE_TURTLESPACERENDERER_H_

// end of TurtleSpaceRenderer.h ...

