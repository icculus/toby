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

void Turtle::setSize(double newSize)
{
    sideLength = newSize;
} // Turtle::setSize


void Turtle::setWidth(double newSize)
{
    setSize(newSize);
} // Turtle::setWidth


void Turtle::setHeight(double newSize)
{
    setSize(newSize);
} // Turtle::setHeight


double Turtle::getWidth(void)
{
    return(sideLength);
} // Turtle::getWidth


double Turtle::getHeight(void)
{
    return(sideLength);
} // Turtle::getHeight


void Turtle::setVisible(bool visibility)
{
    isVisible = visibility;
} // Turtle::setVisible


bool Turtle::getVisible(void)
{
    return(isVisible);
} // Turtle::setVisible


void Turtle::setXY(double x, double y)
{
    turtleX = x;
    turtleY = y;
} // Turtle::setXY


void Turtle::setX(double x)
{
    turtleX = x;
} // Turtle::setX


void Turtle::setY(double y)
{
        turtleY = y;
} // Turtle::setY


double Turtle::getX()
{
    return(turtleX);
} // Turtle::getX


double Turtle::getY()
{
    return(turtleY);
} // getY


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


// end of Turtle.cpp ...

