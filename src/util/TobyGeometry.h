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

#ifndef _INCLUDE_TOBYGEOMETRY_H_
#define _INCLUDE_TOBYGEOMETRY_H_

#include <math.h>
#include "util/TobyObject.h"


// These are UNSAFE macros! Do NOT use increment operators, etc with them!
//  tobyclamp expects (low) < (high).
#define tobymin(x, y) ((x) < (y) ? (x) : (y))
#define tobymax(x, y) ((x) > (y) ? (x) : (y))
#define tobyclamp(low, x, high) ( (x) > (high) ? (high) : ((x) < (low) ? (low) : (x)) )



/*
 * Some needed math routines.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class TobyGeometry
{
public:
    static inline double degreesToRadians(double degrees)
    {
        return(degrees * (M_PI / 180.0));
    } // TobyGeometry::degreesToRadians


    static inline double radiansToDegrees(double radians)
    {
        return(radians * (180.0 / M_PI));
    } // TobyGeometry::degreesToRadians


    static inline int roundDoubleToInt(double dbl)
    {
        return((int) (dbl + 0.5));
    } // TobyGeometry::roundDoubleToInt


    static inline void calculateLine(double heading, double distance,
                              double startX, double startY,
                              double *endX, double *endY)
    /*
     * This procedure calculates coordinates for a line. No line is actually
     *  drawn by this procedure, though.
     *
     *   params : heading  == 0 - 360 degree direction line goes.
     *            distance == total space line should cover.
     *            startX   == Starting x coordinate for line.
     *            startY   == Starting y coordinate for line.
     *            *endX    == filled with end point on X-axis.
     *            *endY    == filled with end point on Y-axis.
     *   returns : void. Data is return in (endX) and (endY).
     */
    {
        assert(endX != NULL);
        assert(endY != NULL);

        double rad = degreesToRadians(heading);
        *endX = (cos(rad) * (double) distance) + startX;
        *endY = (sin(rad) * (double) distance) + startY;
    } // TobyGeometry::calculateLine

    /*
     * This converts a float in the range of 0.0 to 1.0 to an 8-bit integer.
     *  A value of 0.0 (zero percent) yields a return of 0. A value of 1.0
     *  (one hundred percent) yields a return of 255. Everything else is
     *  somewhere inbetween. Values not in the range of 0.0 to 1.0 are clamped
     *  before conversion.
     */
    static inline toby_uint8 floatTo8Bit(float val)
    {
        if (val > 1.0)
            val = 1.0;
        else if (val < 0.0)
            val = 0.0;

        return((toby_uint8) (val * 255.0));
    } // TobyGeometry::floatTo8Bit


    static inline double pythagorian(double s1, double hypotenuse)
    /*
     *  The Pythagorian Theorem: Figure out the length of the third side
     *   of a right triangle, based on the lengths of the other two.
     *
     *  The formula is like this: The sum of the squares of the lengths of
     *   the two non-hypotenuse sides of a right triangle will equal the
     *   square of the hypotenuse. So, if we know two sides of a right
     *   triangle, we can get the third. (In this function, you need to know
     *   which is the hypotenuse, but you can easily write a version of
     *   this that figures out the hypotenuse based on the other two sides.
     *
     *  To find a non-hypotenuse side:
     *    (H2 represents hypotenuse, squared.)
     *    (X2 represents 1st non-hypotenuse side, squared.)
     *    (Y2 represents 2nd non-hypotenuse side, squared.)
     *
     *       H2 = X2 + Y2
     *       H2 - X2 = Y2 + X2 - X2
     *       H2 - X2 = Y2
     *       H2 - H2 - X2 = Y2 - H2
     *       -X2 = Y2 - H2
     *       X2 = - Y2 + H2
     *       X2 = H2 - Y2
     *
     *      params : s1 == length of known non-hypotenuse side of rt. triangle.
     *               hypotenuse == length of hypotenuse of right triangle.
     *     returns : length of third side of right triangle.
     */
    {
        return(sqrt((hypotenuse * hypotenuse) - (s1 * s1)));
    } // TobyGeometry::pythagorian

}; // class TobyGeometry

#endif // !defined _INCLUDE_TOBYGEOMETRY_H_

// end of TobyGeometry.h ...


