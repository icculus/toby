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

#ifndef _INCLUDE_TURTLE_H_
#define _INCLUDE_TURTLE_H_

#include "util/TobyObject.h"

/*
 *  The Turtle class. This is all the state related to one of those little
 *   line-drawing cursors that roams around TurtleSpace. Note that changing
 *   state like visibility or size doesn't affect the visual representation
 *   by default; these are just data structures. The TurtleSpace and
 *   TurtleSpaceRenderer classes handle getting those changes to the screen.
 *
 *     Written by Ryan C. Gordon. (icculus@linuxgames.com)
 */
class Turtle : public TobyObject
{
public:
    Turtle(void) : sideLength(0.0), angle(270.0),
                   turtleX(0.0), turtleY(0.0),
                   isVisible(false), shouldDraw(true),
                   penRed(1.0), penGreen(1.0), penBlue(1.0), penAlpha(1.0) {}

    virtual ~Turtle(void) {}

    void setWidth(double newSize);
    void setHeight(double newSize);
    double getWidth(void);
    double getHeight(void);
    void setVisible(bool visibility);
    bool getVisible(void);
    void setXY(double x, double y);
    void setX(double x);
    void setY(double y);
    double getX(void);
    double getY(void);
    void getXY(double *x, double *y);
    void rotate(double degrees);
    double getAngle(void);
    void setAngle(double newAngle);
    void setPenDown(bool newState);
    bool isPenDown(void);
    void getPenColor(float *r, float *g, float *b, float *a);
    void setPenColor(float r, float g, float b, float a);

private:
    double sideLength;     // Size of one side of Turtle.
    double angle;          // 0 - 360 degrees. Direction faced.
    double turtleX;        // X location of Turtle.
    double turtleY;        // Y location of Turtle.
    bool isVisible;        // Should we even paint this guy?
    bool shouldDraw;       // Should we leave trails?
    float penRed;          // Turtle's pen's red element.
    float penGreen;        // Turtle's pen's green element.
    float penBlue;         // Turtle's pen's blue element.
    float penAlpha;        // Turtle's pen's alpha element.

    void setSize(double newSize);   // !!! remove. Turtle is not necessarily square.
}; // class Turtle

#endif  // !defined _INCLUDE_TURTLE_H_

// end of Turtle.h ...

