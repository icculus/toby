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

#include "turtlespace/Turtle.h"

void Turtle::setWidth(double newSize)
{
    width = newSize;
    sizeAdjust();
} // Turtle::setWidth


void Turtle::setHeight(double newSize)
{
    height = newSize;
    sizeAdjust();
} // Turtle::setHeight


void Turtle::setSize(double newSize)
{
    width = height = newSize;
    sizeAdjust();
} // Turtle::setSize


inline void Turtle::sizeAdjust(void)
{
    if (isVisible)
        calcTriangle();
} // Turtle::sizeAdjust


double Turtle::getWidth(void)
{
    return(width);
} // Turtle::getWidth


double Turtle::getHeight(void)
{
    return(height);
} // Turtle::getHeight


void Turtle::setVisible(bool visibility)
{
    isVisible = visibility;
    if (isVisible)
        calcTriangle();
} // Turtle::setVisible


bool Turtle::getVisible(void)
{
    return(isVisible);
} // Turtle::setVisible


void Turtle::setXY(double x, double y)
{
    turtleX = x;
    turtleY = y;
    positionAdjust();
} // Turtle::setXY


void Turtle::setX(double x)
{
    turtleX = x;
    positionAdjust();
} // Turtle::setX


void Turtle::setY(double y)
{
    turtleY = y;
    positionAdjust();
} // Turtle::setY


double Turtle::getX()
{
    return(turtleX);
} // Turtle::getX


double Turtle::getY()
{
    return(turtleY);
} // getY


inline void Turtle::positionAdjust(void)
{
    if (isVisible)
        calcTriangleLocation();
} // Turtle::positionAdjust


void Turtle::getXY(double *x, double *y)
{
    if (x != NULL)
        *x = turtleX;

    if (y != NULL)
        *y = turtleY;
} // Turtle::getXY


void Turtle::rotate(double degrees)
{
    setAngle(angle + degrees);
} // Turtle::rotate


double Turtle::getAngle(void)
{
    return(angle);
} // getAngle


void Turtle::setAngle(double newAngle)
{
    angle = newAngle;
    while (angle < 0)
        angle += 360.0;

    // !!! can't do mod with floating point?   angle %= 360.0;
    while (angle >= 360.0)
        angle -= 360.0;

    if (isVisible)
        calcTriangle();
} // Turtle::setAngle


void Turtle::setPenDown(bool newState)
{
    shouldDraw = newState;
} // Turtle::setPenDown


bool Turtle::isPenDown(void)
{
    return(shouldDraw);
} // Turtle::isPenDown


void Turtle::getPenColor(float *r, float *g, float *b, float *a)
{
    if (r != NULL)
        *r = penRed;

    if (g != NULL)
        *g = penGreen;

    if (b != NULL)
        *b = penBlue;

    if (a != NULL)
        *a = penAlpha;
} // Turtle::getPenColor


void Turtle::setPenColor(float r, float g, float b, float a)
{
    penRed = r;
    penGreen = g;
    penBlue = b;
    penAlpha = a;
} // Turtle::setPenColor


int *Turtle::getRenderingIntsX(void)
{
    return(intsX);
} // Turtle::getRenderingIntsX


int *Turtle::getRenderingIntsY(void)
{
    return(intsY);
} // Turtle::getRenderingIntsY



inline void Turtle::calcTriangle(void)
/*
 * We need to take into account the direction that the turtle is
 *  facing when we draw him. This complicates the drawing process
 *  considerably, but makes me grateful that I occasionally picked
 *  up on what my high school math teachers were trying to explain.
 *  (Thanks Mrs. Strohm and Mrs. Tannery!)
 *
 *                                               0
 *                                               |    <-- Front of turtle.
 *                                              /|\
 *        Follow this diagram                  / | \
 *         if you get lost in                 /  |  \
 *         the code:                         /   |   \
 *                                          /    X0,0 \
 *            'X' == start point.          /     |     \
 *       0 thru 3 == calculated points.   /      |      \
 *                                       /     90|90     \
 *                                     2---------3---------1
 *                                              180
 */
{
    double halfWidth = width / 2;
    double tailX;
    double tailY;

        // initialize the start point. We consider dead center of the turtle
        //  to be point (0, 0) during this function, so we just need to offset
        //  a little. To face the rear of the Turtle, we need to swing around
        //  180.0 degrees for this calculation. We'll call this result the
        //  Turtle's "tail", since it's in the middle of his backside.  :)
    TobyGeometry::calculateLine(angle + 180.0, height / 2, 0, 0,
                                &tailX, &tailY);

        // Calculate front-facing point of triangle...
        //  We start from center of the turtle's rear side, which we're
        //  considering (0, 0) to be, and head out the full height of the
        //  Turtle. This gives us both points of the centerline segment, and
        //  the front-facing point. (point #1).
    TobyGeometry::calculateLine(angle, height, tailX, tailY,
                                &pointsX[0], &pointsY[0]);


        // Now, rotate 90 degrees on point (0, 0), which will angle us
        //  towards point #2. Since we are in the center of the rear line,
        //  we go out half the width of the turtle.
    TobyGeometry::calculateLine(angle + 90.0, halfWidth, tailX, tailY,
                                &pointsX[1], &pointsY[1]);

        // A final rotation on the start point, -90 degrees will point us
        //  in the opposite direction, towards the last point, #3. Again,
        //  go half the turtle's width to hit this point.
    TobyGeometry::calculateLine(angle - 90.0, halfWidth, tailX, tailY,
                                &pointsX[2], &pointsY[2]);

        // Fill these in, so they are available to calcTriangleLocation()...
    pointsX[3] = tailX;
    pointsY[3] = tailY;

        // Now, call calcTriangleLocation() to update the actual
        //  screen coordinates for the turtle, which, by changing the
        //  angle, have guaranteed to have changed.
    calcTriangleLocation();

        // Wow, wasn't that fun? Of course it was.
} // Turtle::calcTriangle


inline void Turtle::calcTriangleLocation(void)
{
    for (unsigned int i = 0; i < (sizeof (intsX) / sizeof (int)); i++)
    {
        intsX[i] = TobyGeometry::roundDoubleToInt(pointsX[i] + turtleX);
        intsY[i] = TobyGeometry::roundDoubleToInt(pointsY[i] + turtleY);
    } // for
} // Turtle::calcTriangleLocation


// end of Turtle.cpp ...

