/*
 * TOBY -- A LOGO-like interpreted language, written in 100% pure Java.
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

/**
 * Extension to java.lang.math to allow calculations in TurtleSpace.
 *
 *    Written by Ryan C. Gordon.
 */

import java.awt.Point;

public class TobyGeometry
{

    public static double degreesToRadians(double degrees)
    {
        return(degrees * (Math.PI / 180));
    } // degreesToRadians


    public static double radiansToDegrees(double radians)
    {
        return(radians * (180 / Math.PI));
    } // degreesToRadians


    public static int roundDoubleToInt(double dbl)
    {
        return((int) Math.round(dbl));
    } // roundDoubleToInt


    public static DoublePoint calculateLine(double heading, double distance,
                                            double startX, double startY)
    /**
     * This procedure calculates coordinates for a line. No line is actually
     *  drawn by this procedure, though.
     *
     *   params : heading  == 0 - 360 degree direction line goes.
     *            distance == total space line should cover.
     *            startX   == Starting x coordinate for line.
     *            startY   == Starting y coordinate for line.
     *   returns : Point object with calculated ending point.
     */
    {
        DoublePoint retVal = new DoublePoint();
        double rad;

        heading -= 90.0;      // Make 0 degrees point north.
                              // In mathematics, 0 points due east.
                              //  in TobySpace, 0 points due north.
                              //  Yeah, I know...it's dumb.

        rad = degreesToRadians(heading);
        retVal.x = Math.cos(rad) * (double) distance;
        retVal.y = Math.sin(rad) * (double) distance;
    
        retVal.x += startX;     // offset for starting point considerations.
        retVal.y += startY;

        return(retVal);
    } // calculateLine



    public static DoublePoint calculateLine(double heading, double distance,
                                            DoublePoint startPoint)
    /**
     *  Overloaded calculateLine(), same as above, but takes a DoublePoint arg.
     */
    {
        return(calculateLine(heading, distance, startPoint.x, startPoint.y));
    } // calculateLine (this one takes a DoublePoint obj, instead of 2 nums...)



    public static double pythagorian(double s1, double hypotenuse)
    /**
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
        return(Math.sqrt((hypotenuse * hypotenuse) - (s1 * s1)));
    } // pythagorian

} // TobyGeometry


// end of TobyGeometry.java ...


