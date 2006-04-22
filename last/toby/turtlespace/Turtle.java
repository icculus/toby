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

package last.toby.turtlespace;

import java.awt.*;
import java.awt.image.*;
import javax.swing.*;
import last.toby.util.*;

    /**
     *  The Turtle class.
     *
     *     @author Ryan C. Gordon. (icculus@icculus.org)
     */
public class Turtle
{
    public static final double DUE_NORTH = 270.0;

    private double  sideLength = 20.0;    // Size of one side of Turtle.
    private double  angle = DUE_NORTH;    // 0 - 360 degrees. Direction faced.
    private double  turtleX = 0.0;        // X location of Turtle.
    private double  turtleY = 0.0;        // Y location of Turtle.
    private boolean isVisible = true;     // Should we even paint this guy?
    private boolean shouldDraw = true;    // Should we leave trails?
    private Color   penColor = null;      // Current pen color.


        /**
         * Set the turtle size. This is the maximum square a
         *  turtle can inhabit. <em>x</em> represents both the
         *  width and height; hence, a square.
         *
         *    @param newSize width and height to set turtle to be.
         */
    private final void setSize(double newSize)
    {
        sideLength = newSize;
    } // setSize


    public final void setWidth(double newSize)
    {
        setSize(newSize);
    } // setWidth


    public final void setHeight(double newSize)
    {
        setSize(newSize);
    } // setHeight


    public final double getWidth()
    {
        return(this.sideLength);
    } // getWidth


    public final double getHeight()
    {
        return(this.sideLength);
    } // getHeight


        /**
         * Toggle this turtle's visible flag. If it is made invisible,
         *  we blank the turtle image, and will not redraw the li'l guy
         *  until he's set visible again. If he's made visible, we draw him
         *  right now.
         *
         *    @param visibility Should we show the turtle?
         *    @param g Graphics context to paint to.
         *    @param copyImg where to get blanking image from.
         */
    public final void setVisible(boolean visibility)
    {
        isVisible = visibility;
    } // setVisible


    public final boolean getVisible()
    {
        return(isVisible);
    } // setVisible


        /**
         * Explicitly set the turtle to a specified location.
         *
         *    @param x X coordinate.
         *    @param y Y coordinate.
         *    @param g Graphics context to paint to.
         *    @param copyImage where to get blanking image from.
         */
    public final void setXY(double x, double y)
    {
        turtleX = x;
        turtleY = y;
    } // setXY


    public final void setX(double x)
    {
        turtleX = x;
    } // setX


    public final void setY(double y)
    {
        turtleY = y;
    } // setX


        /**
         * Get the turtle's current X coordinate.
         *
         *   @return Current X coordinate.
         */
    public final double getX()
    {
        return(turtleX);
    } // getX


        /**
         * Get the turtle's current Y coordinate.
         *
         *   @return Current Y coordinate.
         */
    public final double getY()
    {
        return(turtleY);
    } // getX


        /**
         * Get the turtle's current X <i>and</i> Y coordinates.
         *
         *   @return Current X and Y coordinate, in a DoublePoint object.
         */
    public final DoublePoint getXY()
    {
        DoublePoint retVal = new DoublePoint();

        retVal.x = turtleX;
        retVal.y = turtleY;

        return(retVal);
    } // getXY


        /**
         *  Rotate our little virtual friend. Specifying a negative
         *   number spins him left, positive numbers spin him right.
         *   Just like in geometry.
         *
         *    @param degrees Degrees to rotate turtle.
         *    @param g Graphics context to paint to.
         *    @param copyImage where to get blanking image from.
         */
    public final void rotate(double degrees)
    {
        setAngle(angle + degrees);
    } // rotate



        /**
         * Get the angle the Turtle is facing. This angle will always be from
         *  0.0 to ~359.999999999...remember that 360 degrees is just 0.0 ...
         *
         *     @return current Turtle angle.
         */
    public final double getAngle()
    {
        return(angle);
    } // getAngle


        /**
         * Set the turtle's angle. The angle may be negative, or > 360, and
         *  the angle will just be "wrapped."
         *
         *    @param newAngle New angle for turtle to face.
         *    @param g Graphics context to paint to.
         *    @param copyImage where to get blanking image from.
         */
    public final void setAngle(double newAngle)
    {
        angle = newAngle;
        while (angle < 0)
            angle += 360.0;

        angle %= 360.0;
    } // setAngle


        /**
         * Turn the Turtle's pen on or off. If set on, future movement of
         *  the Turtle will leave lines on TurtleSpace.
         *
         *   @param newState <em>true</em> if pen should draw,
         *                   <em>false</em> otherwise.
         */
    public final void setPenDown(boolean newState)
    {
        shouldDraw = newState;
    } // setPenDown


        /**
         * Determine if the turtle's pen is set for drawing, currently.
         *
         *  @return <em>true</em> if pen is drawing, <em>false</em> otherwise.
         */
    public final boolean isPenDown()
    {
        return(shouldDraw);
    } // isPenDown


        /**
         * Return the current pen color, as a java.awt.Color object.
         *
         *   @return Current pen color.
         */
    public final Color getPenColor()
    {
        return(penColor);
    } // getPenColor


        /**
         * Set the pen color. This is just stored in the Turtle object; actual
         *  drawing of lines in this color is done in TurtleSpace.
         *
         *    @param r Red element of new color (between 0.0 and 1.0).
         *    @param g Green element of new color (between 0.0 and 1.0).
         *    @param b Blue element of new color (between 0.0 and 1.0).
         */
    public final void setPenColor(float r, float g, float b)
    {
        penColor = new Color(r, g, b);   // Pooling these would be nice. !!!
    } // setPenColor

} // Turtle

// end of Turtle.java ...

