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
 *   TurtleSpace under the turtle (i.e. - TriangleTurtle is NOT a square, and
 *   we'll need to see the uncovered parts underneath him...this gets harder
 *   as a component than it would be as just a drawer...Also, the line
 *   drawing and coloring routines are in here, which would either need to
 *   be moved to TurtleSpace.java (complicates things), or kept in a
 *   non-component Turtle...not to mention that JComponents add massive
 *   overhead: it would be unbearable if you had to continually setLocation()
 *   to move the Turtle around TurtleSpace. Ugh.
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

    private double  sideLength = 20.0;    // Size of one side of Turtle.
    private double  angle = 0.0;          // 0 - 360 degrees. Direction faced.
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
        penColor = colorByNumber(defaultPenColor());

        visibleNotify(false);   // stops premature calculation.
        sizeNotify(sideLength);
        angleNotify(angle);      // due north in TobySpace. !!! for now.
        positionNotify(turtleX, turtleY);
        visibleNotify(true);
    } // Constructor


        /**
         * Return the default pen color, as a Toby "number" type.
         * This should probably be a White or gray color...
         *
         *   @return default pen color.
         */
    public double defaultPenColor()
    {
        return(11.0);
    } // defaultPenColor


        /**
         * Set the turtle size. This is the maximum square a
         *  turtle can inhabit. <em>x</em> represents both the
         *  width and height; hence, a square.
         *
         *    @param newSize width and height to set turtle to be.
         */
    public final void setTurtleSize(double newSize)
    {
        sideLength = newSize;
        sizeNotify(newSize);
    } // setTurtleSize


        /**
         * Get the turtle size. This is the maximum square a
         *  turtle can inhabit. <em>x</em> represents both the
         *  width and height; hence, a square.
         *
         *    @return x width and height of turtle.
         */
    public final double getTurtleSize()
    {
        return(sideLength);
    } // getSize


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
         * Paint the turtle. If <em>copyImage</em> is not <em>null</em>,
         *  we'll blank the turtle out first. This is really useful if we
         *  are rotating the turtle on a given axis.
         */
    public final void paint(Graphics g, Image copyImage)
    {
        if (isVisible)
        {
            if (copyImage != null)
                blankTurtle(g, copyImage);
            paintImpl(g);
        } // if
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
            if (isVisible)
                blankTurtle(g, copyImg);
            else
                paint(g, copyImg);
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
        angle += degrees;
        angleNotify(angle);
        paint(g, copyImage);
    } // rotate


        /**
         *  Moves the Turtle back to the center of turtlespace,
         *   and faces it north.
         *
         *    @param comp JComponent that we are centering in. Usually
         *                a TurtleSpace object.
         *    @param g Graphics context to paint to.
         *    @param copyImage where to get blanking image from.
         */
    public final void homeTurtle(JComponent comp, Graphics g, Image copyImage)
    {
        blankTurtle(g, copyImage);

        turtleX = (double) ((comp.getWidth()  - sideLength) / 2);
        turtleY = (double) ((comp.getHeight() - sideLength) / 2);

        angle = 0.0;

        angleNotify(0.0);
        positionNotify(turtleX, turtleY);

        if (isVisible)
            paintImpl(g);
    } // homeTurtle


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
        blankTurtle(g, copyImage);
        turtleX = x;
        turtleY = y;
        positionNotify(x, y);

        if (isVisible)
            paintImpl(g);
    } // setXY


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
         * Move turtle forward (distance) length in whatever direction he's
         *  facing. If the "pen" is "down", then draw the appropriate lines
         *  in every Graphics object passed. Typically, this is the
         *  screen context, and the copyImage context. The turtle will only
         *  be drawn (and blanked) on the single Graphics context, which
         *  can be, and usually is, one of the objects in the graphics array.
         *
         *    @param distance How far to move the turtle.
         *    @param gr Array of Graphics to draw lines to.
         *    @param g Graphics context to paint TURTLE to.
         *    @param copyImage where to get blanking image from.
         */
    public final void forwardTurtle(double distance,
                                    Graphics gr[], Graphics g,
                                    Image copyImage)
    {
        DoublePoint point;
        int i;

        point = TobyGeometry.calculateLine(angle, distance, turtleX, turtleY);

        if (shouldDraw)   // draw the line covering path turtle took?
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
        positionNotify(turtleX, turtleY);

        if (isVisible)
            paintImpl(g);
    } // forwardTurtle


        /**
         * Get the angle the Turtle is facing. 0.0 is due north (which is
         *  contrary to Geometric theory)...this angle will always be from
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
        angle = newAngle;
        angle %= 360.0;       // check this !!!
        angleNotify(angle);
        paint(g, copyImage);
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
         * Return the current pen color, as a Java Color object.
         *
         *   @return Current pen color.
         */
    public final Color getPenColor()
    {
        return(penColor);
    } // getPenColor


        /**
         * Set the pen color. The parameter must be valid for the color to
         *  actually change. Future turtle movement will cause lines in the
         *  new color.
         *
         *    @param colorNum new pen color.
         *   @return <em>true</em> if new color was valid,
         *           <em>false</em> otherwise.
         */
    public final boolean setPenColor(double colorNum)
    {
        boolean retVal = false;
        Color color = colorByNumber(colorNum);

        if (color != null)
        {
            penColor = color;
            retVal = true;
        } // if

        return(retVal);
    } // setPenColor


        /**
         * Converts the current pen color to its numeric equivalent, then
         *  converts that to a string. Yeah.
         *
         *    @return Current pen color number, in String form.
         */
    public final String getPenColorNumStr()
    {
        double penColNum = numberByColor(penColor);
        return(Double.toString(penColNum));
    } // getPenColorNumStr


        /**
         * Convert a number into a color. Each of the integers from 0 to 11
         *  represent a standard color (java.awt.Color.red, for example.), and
         *  I've tried to have them match QuickBASIC's color scheme. Sorta.
         *  The parameter is a double, even though only whole numbers are
         *  accepted, since Toby's NUMBER intrinsic is equivalent to Java's
         *  double. That's what they'll specify for standard functions such
         *  as setPenColor()...
         *
         *    @param number whole number from 0-11.
         *   @return Color based on number passed as argument. <em>null</em>
         *           if <em>number</em> is not a whole number from 0 to 11.
         */
    public final Color colorByNumber(double number)
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


        /**
         * Refer to colorByNumber(). Returns appropriate number for standard
         *  color, or (-1.0) if not a standard color.
         *
         *   @param color standard Java Color object to get Toby equivalent.
         *  @return Toby equivalent number of Java color.
         */
    public final double numberByColor(Color color)
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


        /**
         * This gives us a change to create our subclasses' arrays before
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


