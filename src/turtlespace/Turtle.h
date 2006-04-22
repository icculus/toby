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
#include "util/TobyGeometry.h"

/*
 *  The Turtle class. This is all the state related to one of those little
 *   line-drawing cursors that roams around TurtleSpace. Note that changing
 *   state like visibility or size doesn't affect the visual representation
 *   by default; these are just data structures. The TurtleSpace and
 *   TurtleSpaceRenderer classes handle getting those changes to the screen.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class Turtle : public TobyObject
{
public:
    Turtle(void) : width(0.0), height(0.0), angle(270.0),
                   turtleX(0.0), turtleY(0.0), centerLineHalf(0.0),
                   isVisible(false), shouldDraw(true),
                   penRed(1.0), penGreen(1.0), penBlue(1.0), penAlpha(1.0) {}

    virtual ~Turtle(void) {}

    void setWidth(double newSize);
    void setHeight(double newSize);
    void setSize(double newSize);
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


        // Get an array of points, rounded for raster displays, that
        //  represent the turtle on screen. The array should be considered
        //  READ ONLY, and not free()'d or delete[]'d. The first three
        //  elements of the array are the points that make up the Turtle's
        //  triangle, and the fourth element is the center of the rear line
        //  segment (which, when used with element 0 in the array, gives you
        //  a line segment that runs vertically down the center of the
        //  triangle.
    int *getRenderingIntsX(void);
    int *getRenderingIntsY(void);


private:
    double width;          // Size of left and right sides of Turtle.
    double height;         // Size from back side to front vertex.
    double angle;          // 0 - 360 degrees. Direction faced.
    double turtleX;        // X location of Turtle.
    double turtleY;        // Y location of Turtle.
    double centerLineHalf; // .5 of the center line.
    bool isVisible;        // Should we even paint this guy?
    bool shouldDraw;       // Should we leave trails?
    float penRed;          // Turtle's pen's red element.
    float penGreen;        // Turtle's pen's green element.
    float penBlue;         // Turtle's pen's blue element.
    float penAlpha;        // Turtle's pen's alpha element.
    double pointsX[4];
    double pointsY[4];
    int intsX[4];
    int intsY[4];


        // We use this method to readjust our size-related members, which are
        //  used to draw the Turtle. Ignored if Turtle is not visible.
    inline void sizeAdjust(void);


        // Notifies us that the Turtle has changed positions, but not angle.
        //  We update our screen coordinates accordingly, if turtle visible.
    inline void positionAdjust(void);


        // This function is called whenever the coordinates that make up
        //  the 3 points on a triangle need to be recalculated. Specifically,
        //  this is done when the angle or size of the turtle are changed.
        //
        // The calculations are stored in pointsX, pointsY, and centerDbl
        //  for later use by paintImpl().
        //
        // The turtle is calculated to be at (0, 0) in TurtleSpace. We will
        //  add the correct coordinates (and round to ints) before painting.
    inline void calcTriangle(void);


        // This function calculates the screen coordinates of the Turtle.
        //  The values stored in pointsX and pointsY are added to
        //  turtleX, and turtleY, respectively, and rounded to ints.
        //
        // The results are stored in intsX and intsY, respectively.
        //
        // This function must be called anytime the size, angle, or
        //  location of the turtle has changed. Note that calcTriangle()
        //  calls this, so you may not have to.
    inline void calcTriangleLocation(void);

}; // class Turtle

#endif  // !defined _INCLUDE_TURTLE_H_

// end of Turtle.h ...

