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

package last.toby.turtlespace.swing;

import java.awt.*;
import java.awt.image.*;
import javax.swing.*;
import last.toby.util.*;

    /**
     *  The Turtle class.
     *
     *  This contains all the functionality needed for the graphical "cursor"
     *   in TurtleSpace. Derive paintImpl() to make your own Turtle. For
     *   example, the basic Turtle is a green triangle, but maybe a bitmapped
     *   image of a turtle class could be made, etc...
     *
     *  This is NOT a component. We have more than one place this needs to
     *   draw to: Icons, TurtleSpace, etc...plus, we need to be able to see the
     *   TurtleSpace under the turtle (i.e. - TriangleTurtle is NOT a square,
     *   and we'll need to see the uncovered parts underneath him...this gets
     *   harder as a component than it would be as just a drawer...Also, the
     *   line drawing and coloring routines are in here, which would either
     *   need to be moved to TurtleSpace.java (complicates things), or kept
     *   in a non-component Turtle...not to mention that JComponents add massive
     *   overhead: it would be unbearable if you had to continually setLocation()
     *   to move the Turtle around TurtleSpace. Ugh.
     *
     *     @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public abstract class Turtle implements ImageObserver
{

        /*
         * Class variables and constants...
         */

    public static final double DUE_NORTH = 270.0;

    private double  sideLength = 20.0;    // Size of one side of Turtle.
    private double  angle = DUE_NORTH;    // 0 - 360 degrees. Direction faced.
    private double  turtleX = 0.0;        // X location of Turtle.
    private double  turtleY = 0.0;        // Y location of Turtle.
    private boolean isVisible = true;     // Should we even paint this guy?
    private boolean shouldDraw = true;    // Should we leave trails?
    private Color   penColor = null;      // Current pen color.


        /**
         * Build a turtle. Set internal variables to defaults...
         */
    public Turtle()
    {
        initTurtle();
        visibleNotify(false);   // stops premature calculation.
        sizeNotify(sideLength);
        angleNotify(DUE_NORTH);      // due north in TobySpace.
        positionNotify(turtleX, turtleY);
    } // Constructor


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
        sizeNotify(newSize);
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
         * Returns the greater of two values.
         *
         *   @param x1 first value to compare.
         *   @param x2 second value to compare.
         *  @return Greater value of <em>x1</em> and <em>x2</em>.
         */
    public static int max(int x1, int x2)
    {
        return((x1 > x2) ? x1 : x2);
    } // max


        /**
         * Replace the current location of the turtle with
         *  what resides in that location at copyImage.
         *
         *    @param g Context to blank turtle on.
         *    @param copyImage image with non-turtle graphics.
         */
    private void blankTurtle(Graphics g, Image copyImage)
    {
        if (isVisible)
        {
            int startX = max((int) (turtleX - sideLength), 0);
            int startY = max((int) (turtleY - sideLength), 0);
            int endX   = max((int) (turtleX + sideLength), 0);
            int endY   = max((int) (turtleY + sideLength), 0);

            g.drawImage(copyImage,
                        startX, startY, endX, endY,
                        startX, startY, endX, endY,
                        this);
        } // if
    } // blankTurtle


        /**
         * Paint the turtle.
         */
    public final void paint(Graphics g)
    {
        if (isVisible)
            paintImpl(g);
    } // paint


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
    public final void setVisible(boolean visibility, Graphics g, Image copyImg)
    {
        if (isVisible != visibility)
        {
            if ((g != null) && (copyImg != null))
            {
                if (isVisible)
                    blankTurtle(g, copyImg);
                else
                    paint(g);
            } // if
            isVisible = visibility;
            visibleNotify(visibility);
        } // if
    } // setVisible


        /**
         *  Rotate our little virtual friend. Specifying a negative
         *   number spins him left, positive numbers spin him right.
         *   Just like in geometry.
         *
         *    @param degrees Degrees to rotate turtle.
         *    @param g Graphics context to paint to.
         *    @param copyImage where to get blanking image from.
         */
    public final void rotate(double degrees, Graphics g, Image copyImage)
    {
        if (isVisible)
            blankTurtle(g, copyImage);

        angle += degrees;
        angleNotify(angle);
        paint(g);
    } // rotate


        /**
         * Explicitly set the turtle to a specified location.
         *
         *    @param x X coordinate.
         *    @param y Y coordinate.
         *    @param g Graphics context to paint to.
         *    @param copyImage where to get blanking image from.
         */
    public final void setXY(double x, double y, Graphics g, Image copyImage)
    {
        if (isVisible)
            blankTurtle(g, copyImage);

        turtleX = x;
        turtleY = y;
        positionNotify(x, y);

        if (isVisible)
            paintImpl(g);
    } // setXY


    public final void setX(double x, Graphics g, Image copyImage)
    {
        setXY(x, this.turtleY, g, copyImage);
    } // setX


    public final void setY(double y, Graphics g, Image copyImage)
    {
        setXY(this.turtleX, y, g, copyImage);
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
    public final void setAngle(double newAngle, Graphics g, Image copyImage)
    {
        if (isVisible)
            blankTurtle(g, copyImage);

        angle = newAngle;

        while (angle < 0)
            angle += 360.0;

        angle %= 360.0;
        angleNotify(angle);

        if (isVisible)
            paint(g);
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


        /**
         * This gives us a chance to create our subclasses' arrays before
         *  this superclass's constructor forces us to use them. This is
         *  the first method the superclass calls.
         */
    protected abstract void initTurtle();


        /**
         * Notify Turtle subclasses that the Turtle is now facing a new angle.
         *  @param newAngle new angle Turtle is facing.
         */
    protected abstract void angleNotify(double newAngle);


        /**
         * Notify Turtle subclasses that the Turtle is now (in)visible.
         *  @param isVis (true) if Turtle is now visible, (false) otherwise.
         */
    protected abstract void visibleNotify(boolean isVis);


        /**
         * Notify Turtle subclasses that the Turtle is now a new size.
         *  @param newSize new size of Turtle.
         */
    protected abstract void sizeNotify(double newAngle);


        /**
         * Notify Turtle subclasses that the Turtle has moved,
         *  but not changed angle.
         *
         *  @param newSize new size of Turtle.
         */
    protected abstract void positionNotify(double x, double y);


        /**
         * This method must take into consideration the size and angle
         *  by calling getSize() and getAngle(), respectively. This is
         *  only called if Turtle.paint() is called, and isVisible == true.
         *
         * Have fun.
         */
    protected abstract void paintImpl(Graphics g);



        // ImageObserver implementation ...

        /**
         * We just need a default imageUpdate() that uniformly returns
         *  (false), so we have SOMETHING to pass to Graphics.drawImage()...
         */
    public boolean imageUpdate(Image img, int infoflags,
                               int x, int y, int width, int height)
    {
        return(false);
    } // imageUpdate

} // Turtle


// end of Turtle.java ...


