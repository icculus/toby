/**
 *  The Turtle class.
 *
 *  This contains all the functionality needed for the graphical "cursor"
 *   in TurtleSpace. Derive paintImpl() to make your own Turtle. For
 *   example, the basic Turtle is a green triangle, but maybe a bitmapped
 *   image of a turtle class could be made, etc...
 *
 *  !!! Justify why this is NOT a component.
 *
 *    Copyright (c) Lighting and Sound Technologies, 1997.
 *     Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.awt.image.*;
import javax.swing.*;

public abstract class Turtle implements ImageObserver
{

        /*
         * Class variables and constants...
         */

    private double  sideLength;       // Size of one side of Turtle.
    private double  angle;            // 0 - 360 degrees. Direction faced.
    private double  turtleX;          // X location of Turtle.
    private double  turtleY;          // Y location of Turtle.
    private boolean isVisible;        // Should we even paint this guy?
    private boolean isPenUp;          // Should we leave trails?
    private Color   penColor;

    public Turtle()
    {
        penColor = colorByNumber(defaultPenColor());
        isVisible = true;
        isPenUp = false;
        angle = 0.0;   // due north in TobySpace.
    } // Constructor


    public double defaultPenColor()
    {
        return(11.0);
    } // defaultPenColor


    public void setTurtleSize(double x)
    {
        sideLength = x;
    } // setSize


    public final double getTurtleSize()
    {
        return(sideLength);
    } // getSize


    private void blankTurtle(Graphics g, Image copyImage)
    {
        if (isVisible)
        {
            g.drawImage(copyImage,
                        (int) (turtleX - sideLength),
                        (int) (turtleY - sideLength),
                        (int) (turtleX + sideLength),
                        (int) (turtleY + sideLength),
                        (int) (turtleX - sideLength),
                        (int) (turtleY - sideLength),
                        (int) (turtleX + sideLength),
                        (int) (turtleY + sideLength),
                        this);
        } // if
    } // blankTurtle


    public final void paint(Graphics g, Image copyImage)
    {
        if (isVisible)
        {
            blankTurtle(g, copyImage);
            paintImpl(g);
        } // if
    } // paint


    protected abstract void paintImpl(Graphics g);
    /**
     * This method must take into consideration the size and angle
     *  by calling getSize() and getAngle(), respectively. This is
     *  only called if Turtle.paint() is called, and isVisible == true.
     *
     * Also, be sure to call all Graphics methods in a for loop, like so :
     *
     *         for (int i = 0; i < g.length; i++)
     *             g[i].drawLine(x1, y1, x2, y2);
     *
     * Have fun.
     */


    public final void setVisible(boolean visibility, Graphics g, Image copyImg)
    {
        if (isVisible != visibility)
        {
            if (isVisible)
                blankTurtle(g, copyImg);
            else
                paint(g, copyImg);
            isVisible = visibility;
        } // if
    } // setVisible


    public final void rotate(double degrees, Graphics g, Image copyImage)
    /**
     *  Rotate our little virtual friend. Specifying a negative number spins
     *   him left, positive numbers spin him right. Just like in geometry.
     *
     *     params : see above.
     *    returns : void.
     */
    {
        angle += degrees;
        angle %= 360.0;       // check this !!!
        paint(g, copyImage);
    } // rotate


    public final void homeTurtle(JComponent comp, Graphics g, Image copyImage)
    /**
     *  Moves the Turtle back to the center of turtlespace, and face it north.
     *
     *     params : component that we are centering in.
     *    returns : void.
     */
    {
        blankTurtle(g, copyImage);

        turtleX = (double) ((comp.getWidth()  - sideLength) / 2);
        turtleY = (double) ((comp.getHeight() - sideLength) / 2);

        angle = 0.0;
        if (isVisible)
            paintImpl(g);
    } // homeTurtle


    public final void setXY(double x, double y, Graphics g, Image copyImage)
    /** !!! comment !!!
     *
     */
    {
        blankTurtle(g, copyImage);
        turtleX = x;
        turtleY = y;

        if (isVisible)
            paintImpl(g);
    } // moveTurtle


    public final double getX()
    {
        return(turtleX);
    } // getX


    public final double getY()
    {
        return(turtleY);
    } // getX


    public final DoublePoint getXY()
    {
        DoublePoint retVal = new DoublePoint();

        retVal.x = turtleX;
        retVal.y = turtleY;

        return(retVal);
    } // getXY


    public final void forwardTurtle(double distance,
                                    Graphics gr[], Graphics g, Image copyImage)
    /**
     * Move turtle forward (distance) length in whatever direction he's
     *  facing.
     *  !!! comment this! It's out of date.
     *    params : distance == how far to move the turtle.
     *   returns : void.
     */
    {
        DoublePoint point;
        int i;

        point = TobyGeometry.calculateLine(angle, distance, turtleX, turtleY);

        if (!isPenUp)   // draw the line covering path turtle took?
        {
            for (i = 0; i < gr.length; i++)
            {
                gr[i].setColor(penColor);
                gr[i].drawLine(TobyGeometry.roundDoubleToInt(turtleX),
                               TobyGeometry.roundDoubleToInt(turtleY),
                               TobyGeometry.roundDoubleToInt(point.x),
                               TobyGeometry.roundDoubleToInt(point.y));
            } // for
        } // if

        blankTurtle(g, copyImage);
        turtleX = point.x;    // update internals...
        turtleY = point.y;
        if (isVisible)
            paintImpl(g);
    } // forwardTurtle


    public final double getAngle()
    {
        return(angle);
    } // getAngle


    public final void setAngle(double newAngle, Graphics g, Image copyImage)
    {
        angle = newAngle;
        angle %= 360.0;       // check this !!!
        paint(g, copyImage);
    } // setAngle


    public final void setPenUp(boolean isIt)
    {
        isPenUp = isIt;
    } // penUp


    public final boolean isPenUp()
    {
        return(isPenUp);
    } // isPenUp


    public final Color getPenColor()
    {
        return(penColor);
    } // getPenColor


    public final void setPenColor(double colorNum)
    {
        Color color = colorByNumber(colorNum);

        if (color != null)
            penColor = color;
    } // setPenColor


    public final String getPenColorNumStr()
    /**
     * Converts the current pen color to its numeric equivalent, then
     *  converts that to a string. Yeah.
     */
    {
        double penColNum = numberByColor(penColor);
        return(Double.toString(penColNum));
    } // getPenColorNumStr


    public final Color colorByNumber(double number)
    /**
     * Convert a number into a color. Each of the integers from 0 to 11
     *  represent a standard color (java.awt.Color.red, for example.), and
     *  I've tried to have them match QuickBASIC's color scheme. Sorta. The
     *  parameter is a double, even though only whole numbers are accepted,
     *  since Toby's NUMBER intrinsic is equivalent to Java's double. That's
     *  what they'll specify for standard functions such as setPenColor()...
     *
     *       params : number == whole number from 0-11.
     *      returns : color based on number passed as argument. (null) if
     *                (number) is not a whole number from 0 to 11.
     */
    {
        Color retVal = null;

        if (number == 0.0)
            retVal = Color.black;
        else if (number == 1.0)
            retVal = Color.blue;
        else if (number == 2.0)
            retVal = Color.green;
        else if (number == 3.0)
            retVal = Color.cyan;
        else if (number == 4.0)
            retVal = Color.red;
        else if (number == 5.0)
            retVal = Color.magenta;
        else if (number == 6.0)
            retVal = Color.yellow;
        else if (number == 7.0)
            retVal = Color.lightGray;
        else if (number == 8.0)
            retVal = Color.gray;
        else if (number == 9.0)
            retVal = Color.orange;
        else if (number == 10.0)
            retVal = Color.pink;
        else if (number == 11.0)
            retVal = Color.white;
       
        return(retVal);
    } // colorByNumber


    public final double numberByColor(Color color)
    /**
     * Refer to colorByNumber(). Returns appropriate number for standard
     *  color, or (-1.0) if not a standard color.
     */
    {
        double retVal = -1.0;

        if (color.equals(Color.black))
            retVal = 0.0;
        else if (color.equals(Color.blue))
            retVal = 1.0;
        else if (color.equals(Color.green))
            retVal = 2.0;
        else if (color.equals(Color.cyan))
            retVal = 3.0;
        else if (color.equals(Color.red))
            retVal = 4.0;
        else if (color.equals(Color.magenta))
            retVal = 5.0;
        else if (color.equals(Color.yellow))
            retVal = 6.0;
        else if (color.equals(Color.lightGray))
            retVal = 7.0;
        else if (color.equals(Color.gray))
            retVal = 8.0;
        else if (color.equals(Color.orange))
            retVal = 9.0;
        else if (color.equals(Color.pink))
            retVal = 10.0;
        else if (color.equals(Color.white))
            retVal = 11.0;
        return(retVal);
    } // numberByColor


        // ImageObserver implementation ...
    public boolean imageUpdate(Image img, int infoflags,
                               int x, int y, int width, int height)
    {
        return(false);
    } // imageUpdate

} // Turtle


// end of Turtle.java ...


