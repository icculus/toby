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
import last.toby.turtlespace.TurtleSpace;
import last.toby.turtlespace.TurtleSpaceRenderer;
import last.toby.turtlespace.Turtle;
import last.toby.exceptions.ExecException;
import last.toby.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import javax.swing.*;

    /**
     *  !!! comment.
     *   @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public class SwingRenderer extends JComponent implements TurtleSpaceRenderer
{
    public final static double TURTLE_RATIO = 0.02;  // 2 percent of screen.
    public final static int MODE_SCALED   = 0;
    public final static int MODE_CENTERED = 1;
    public final static int MODE_TILED    = 2;
    public final static int MODE_REMOVE   = 3;
    public final static int HIGHEST_MODE  = 3;

    protected Image bgImg = null;
    protected int bgDrawMode = 0;
    protected Image backBuffer = null;
    protected Graphics2D imgGraphics = null;
    protected Graphics2D screenGraphics = null;
    protected int backBufferWidth = -1;
    protected int backBufferHeight = -1;

    public SwingRenderer()
    {
        super();
        setBackground(Color.black);

        addComponentListener(
            new ComponentAdapter()
            {
                public synchronized void componentResized(ComponentEvent e)
                {
                    if (screenGraphics == null)
                        resizeBackBuffer(getWidth(), getHeight());
                    else
                    {
                        screenGraphics.setClip(0, 0,
                                               backBufferWidth,
                                               backBufferHeight);
                    } // else
                } // componentResized
            } // ComponentAdapter
        );
    } // Constructor


    public void finalize() throws Throwable
    {
        notifyUngrabbed();

        if (imgGraphics != null)
        {
            imgGraphics.dispose();
            imgGraphics = null;
        } // if

        if (screenGraphics != null)
        {
            screenGraphics.dispose();
            screenGraphics = null;
        } // if

        if (backBuffer != null)
        {
            backBuffer.flush();
            backBuffer = null;
        } // if

        if (bgImg != null)
        {
            bgImg.flush();
            bgImg = null;
        } // if

        super.finalize();
    } // finalize


    public void update(Graphics g)
    {
        paint(g);  // prevent flicker.
    } // update


    public void renderLine(double x1, double y1, double x2, double y2, Color c)
    {
        int _x1 = TobyGeometry.roundDoubleToInt(x1);
        int _y1 = TobyGeometry.roundDoubleToInt(y1);
        int _x2 = TobyGeometry.roundDoubleToInt(x2);
        int _y2 = TobyGeometry.roundDoubleToInt(y2);

        screenGraphics.setColor(c);
        imgGraphics.setColor(c);
        screenGraphics.drawLine(_x1, _y1, _x2, _y2);
        imgGraphics.drawLine(_x1, _y1, _x2, _y2);
    } // renderLine


    public synchronized void paint(Graphics g)
    {
        if (backBuffer != null)
            g.drawImage(backBuffer, 0, 0, this);
        else
        {
            g.setColor(Color.black);
            g.fillRect(0, 0, getWidth(), getHeight());
        } // else

        /* !!! Repaint the turtles!
        if (turtles != null)
        {
            for (int i = 0; i < turtles.length; i++)
            {
                if (turtles[i] != null)
                    turtles[i].paint(g);
            } // for
        } // if
        */
    } // paint


    protected void resizeBackBuffer(int w, int h)
    {
        if ((backBufferWidth == w) && (backBufferHeight == h))
            return;

        Image newImg = createImage(w, h);
        Graphics g = newImg.getGraphics();
        g.setColor(Color.black);
        g.fillRect(0, 0, w, h);

        if (backBuffer != null)
        {
            g.drawImage(backBuffer, 0, 0, this);
            backBuffer.flush();
        } // if

        g.dispose();

        backBuffer = newImg;
        backBufferWidth = w;
        backBufferHeight = h;
    } // resizeBackBuffer


        // TurtleSpace implementation...

    public synchronized void notifyGrabbed()
    {
        int w = getWidth();
        int h = getHeight();

        resizeBackBuffer(w, h);

        imgGraphics = (Graphics2D) backBuffer.getGraphics();
        imgGraphics.setColor(Color.black);
        imgGraphics.fillRect(0, 0, w, h);

        screenGraphics = (Graphics2D) getGraphics();
    } // grabTurtleSpace


    public synchronized void notifyUngrabbed()
    {
        int w = getWidth();
        int h = getHeight();

        if (imgGraphics != null)
        {
            imgGraphics.dispose();
            imgGraphics = null;
        } // if

            // in case there was a component resize during execution...
        resizeBackBuffer(w, h);
    } // releaseTurtleSpace


    public synchronized double getTurtleSpaceWidth()
    {
        return((double) backBufferWidth);
    } // getTurtleSpaceWidth


    public synchronized double getTurtleSpaceHeight()
    {
        return((double) backBufferHeight);
    } // getTurtleSpaceHeight


    public double getDesiredTurtleWidth()
    {
        return(getTurtleSpaceWidth() * TURTLE_RATIO);
    } // getDesiredTurtleWidth

    public double getDesiredTurtleHeight()
    {
        return(getTurtleSpaceWidth() * TURTLE_RATIO);  // yes, that SHOULD say width. Deal.
    } // getDesiredTurtleHeight


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
         */
    public void blankTurtle(Turtle turtle) throws ExecException
    {
        if (turtle.getVisible())
        {
            int x = (int) turtle.getX();
            int y = (int) turtle.getY();
            int w = (int) turtle.getWidth();
            int h = (int) turtle.getHeight();
            int startX = max((int) (x - w), 0);
            int startY = max((int) (y - h), 0);
            int endX   = max((int) (x + w), 0);
            int endY   = max((int) (y + h), 0);

            screenGraphics.drawImage(backBuffer,
                        startX, startY, endX, endY,
                        startX, startY, endX, endY,
                        this);
        } // if
    } // blankTurtle


        /*
         * These store the results of calculating the dimensions of the Turtle.
         *
         * The Triangle these doubles define would draw in at position (0, 0)
         *  in TurtleSpace. They are offset and stored in (intsX) and (intsY)
         *  during paintImpl() for correct placement.
         */
    private double[] pointsX = new double[3];
    private double[] pointsY = new double[3];
    private DoublePoint centerDbl = new DoublePoint();
    private double centerLineHalf = 0.0;      // ((Size of center line) / 2).

        /*
         * These store the less-precise screen coordinates for the Turtle's
         *  polygon points, but we keep them as instance variables,
         *  so we don't make a new array every time we call paintImpl() (which
         *  can be several thousand times even in a simple TOBY program...)
         */
    private int[] intsX = new int[3];
    private int[] intsY = new int[3];
    private Point centerInt = new Point();


    public void renderTurtle(Turtle turtle) throws ExecException
    {
        if (turtle.getVisible() == true)
        {
                // !!! loses speed to calculate this everytime. Oh well.

            // We'll use the Pythagorian Theorem to figure out how long
            //  the center line of the triangle is. Then, divided by two,
            //  we'll have half the center line. This is needed for the
            //  calcTriangle() method. Splitting an equilateral triangle down
            //  the center gives us a right triangle of which we know the
            //  length of two of the sides, (sideLength, and sideLength / 2)
            //  so we can use the Pythagorian Theorem...have you hugged your
            //  math teacher today?  :)

            double newSize = turtle.getWidth();
            centerLineHalf = TobyGeometry.pythagorian(newSize / 2, newSize) / 2;

                // !!! loses speed to calculate this everytime. Oh well.
            calcTriangle(turtle);

            screenGraphics.setColor(Color.green);              // Color and...

                //  ...draw the triangle.
            screenGraphics.fillPolygon(intsX, intsY, intsX.length);

            screenGraphics.setColor(Color.blue);               // draw center line.
            screenGraphics.drawLine(intsX[0], intsY[0], centerInt.x, centerInt.y);
        } // if
    } // renderTurtle


        /**
         * We need to take into account the direction that the turtle is
         *  facing when we draw him. This complicates the drawing process
         *  considerably, but makes me grateful that I occasionally picked
         *  up on what my high school math teachers were trying to explain.
         *  (Thanks Mrs. Strohm and Mrs. Tannery!)
         *
         * This function is called whenever the coordinates that make up
         *  the 3 points on a triangle need to be recalculated. Specifically,
         *  this is done when the angle or size of the turtle are changed.
         *
         * The calculations are stored in pointsX, pointsY, and centerDbl
         *  for later use by paintImpl().
         *
         * The turtle is calculated to be at (0, 0) in TurtleSpace. We will
         *  add the correct coordinates (and round to ints) before painting.
         *
         *       @param angle Angle Turtle is facing.
         *       @param sideLength size of Turtle, as given by getTurtleSize().
         */
    private void calcTriangle(Turtle t)
    /*
     *                                               |
     *                                            150|150 <-- Front of turtle.
     *                                              /|\
     *        Follow this diagram                  /3|3\
     *         if you get lost in                 / 0|0 \
     *         the code:                         /   |   \
     *                                          /    X    \
     *            'X' == center point.         /     |     \
     *                                        /      |      \
     *                                       /60     |     60\
     *                                      ---------|---------
     *                                     /120
     */
    {

            // !!! Can we speed this up with an Affine Transform?


        double angle = t.getAngle();
        double sideLength = t.getWidth();   // !!! don't assume equilateral?
        DoublePoint point;
        double halfSide = sideLength / 2;

            // Calculate front-facing point of triangle...
            //  We start from dead center of turtle, which we're considering
            //  (0, 0) to be. We head out half the length of the centerline,
            //  which we calculated when we sized this component, and stored
            //  in centerLineHalf.
            // We assign the calculated point to the arrays.

        point = TobyGeometry.calculateLine(angle, centerLineHalf, 0, 0);

        pointsX[0] = point.x;
        pointsY[0] = point.y;


            // Now, turn 150 degrees, which will angle us, from the
            //  previously-calculated point, towards the second point.
            //  (Making a 30 degree angle with the first calc'ed line.
            //  From the center point, this is half of the 60 degrees
            //  we need for each angle of an equilateral triangle.)
            //  the calculateLine() call with give us Point #2 and our
            //  first official side (line segment) of the Turtle.

        angle += 150.0;

        point = TobyGeometry.calculateLine(angle, sideLength,
                                           pointsX[0], pointsY[0]);
        pointsX[1] = point.x;
        pointsY[1] = point.y;

            // A final spin of 120 degrees will point us
            //  towards the last point.

        angle += 120.0;
        point = TobyGeometry.calculateLine(angle, sideLength,
                                           pointsX[1], pointsY[1]);
        pointsX[2] = point.x;
        pointsY[2] = point.y;


            // Finally, we'll draw a line down the center of the
            //  turtle, so we know which of these three equal sides is
            //  designated as the front. To do so, we'll step back to
            //  our second point, and calculate another line towards the
            //  third that's half the size of a side of the turtle. Combined
            //  with our first point (the front point), we have a line
            //  segment that splits the turtle exactly down the center.
            //  We'll store this point in our instance variable, (centerDbl).

        centerDbl = TobyGeometry.calculateLine(angle, halfSide,
                                               pointsX[1], pointsY[1]);

            // Now, call calcTriangleLocation() to update the actual
            //  screen coordinates for the turtle, which, by changing the
            //  angle, have guaranteed to have changed.
        calcTriangleLocation(t.getX(), t.getY());

                // Wow, wasn't that fun? Of course it was.
    } // calcTriangle


        /**
         * This function calculates the screen coordinates of the Turtle.
         *  The values stored in pointsX and pointsY are added to
         *  turtleX, and turtleY, respectively, and rounded to ints.
         * This is also done for centerDbl, which is calculated and
         *  stored in centerInt.
         *
         * This function must be called anytime the size, angle, or
         *  location of the turtle has changed. Note that calcTriangle()
         *  calls this, so you may not have to.
         *
         *     @param turtleX X coordinate of the Turtle in TurtleSpace.
         *     @param turtleY Y coordinate of the Turtle in TurtleSpace.
         */
    private void calcTriangleLocation(double turtleX, double turtleY)
    {
        int i;

        for (i = 0; i < intsX.length; i++)
        {
            intsX[i] = TobyGeometry.roundDoubleToInt(pointsX[i] + turtleX);
            intsY[i] = TobyGeometry.roundDoubleToInt(pointsY[i] + turtleY);
        } // for

        centerInt.x = TobyGeometry.roundDoubleToInt(centerDbl.x + turtleX);
        centerInt.y = TobyGeometry.roundDoubleToInt(centerDbl.y + turtleY);
    } // calcTriangleLocation


    public void renderString(double x, double y, double angle,
                             Color c, String str)
    {
            // this isn't very efficient, I bet.
        AffineTransform origScreenTransform = screenGraphics.getTransform();
        AffineTransform origImageTransform = imgGraphics.getTransform();

        double radians = TobyGeometry.degreesToRadians(angle);
        int _x = (int) x;
        int _y = (int) y;

        screenGraphics.rotate(radians, _x, _y);
        imgGraphics.rotate(radians, _x, _y);

        screenGraphics.setColor(c);
        imgGraphics.setColor(c);

            // since rotating text is a performance hit anyhow, we might as
            //  well swallow the cost of antialiasing it, too. Plus, the
            //  improvement to the output quality is VERY noticible here,
            //  while it isn't so much on the average lines drawn, since they
            //  are usually being done pixel-by-pixel, or are
            //  vertical/horizontal.
        screenGraphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                                        RenderingHints.VALUE_ANTIALIAS_ON);
        imgGraphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                                     RenderingHints.VALUE_ANTIALIAS_ON);

        screenGraphics.drawString(str, _x, _y);
        imgGraphics.drawString(str, _x, _y);

        screenGraphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                                        RenderingHints.VALUE_ANTIALIAS_OFF);
        imgGraphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                                     RenderingHints.VALUE_ANTIALIAS_OFF);

        screenGraphics.setTransform(origScreenTransform);
        imgGraphics.setTransform(origImageTransform);
    } // renderString


    public synchronized void cleanup() throws ExecException
    {
        Graphics g = null;

        if (backBuffer != null)
        {
            g = imgGraphics;
            if (g == null)
                g = backBuffer.getGraphics();

            if (this.bgImg != null)
                drawBackgroundImage(g);
            else
            {
                g.setColor(Color.black);
                g.fillRect(0, 0, getWidth(), getHeight());
            } // else

            if (imgGraphics == null)
                g.dispose();
        } // if

        if (screenGraphics != null)
            paint(screenGraphics);
        else
        {
            g = getGraphics();
            paint(g);
            g.dispose();
        } // else
    } // cleanup


        // Some Swing-specific features...

        // !!! Should make it into the parent class?
    protected synchronized void drawBackgroundImage(Graphics g)
    {
        int h = this.bgImg.getHeight(this);
        int w = this.bgImg.getWidth(this);

        if ((h == -1) || (w == -1))
            return;

        switch (this.bgDrawMode)
        {
            case MODE_SCALED:
                g.drawImage(this.bgImg, 0, 0, this.backBufferWidth,
                            this.backBufferHeight, this);
                break;

            case MODE_TILED:
                for (int x = 0; x < this.backBufferWidth; x += w)
                {
                    for (int y = 0; y < this.backBufferHeight; y += h)
                    {
                        g.drawImage(this.bgImg, x, y, this);
                    } // for
                } // for
                break;

            case MODE_CENTERED:
                g.drawImage(this.bgImg,
                            (this.backBufferWidth - w) / 2,
                            (this.backBufferHeight - h) / 2,
                            this);
                break;
        } // switch
    } // drawBackgroundImage

    /*
    public synchronized void setBackground(Image img, int mode) throws ExecException
    {
        if ((mode >= 0) && (mode <= HIGHEST_MODE))
        {
            if (mode != MODE_SCALED)
                this.bgImg = img;
            else
            {
                this.bgImg = img.getScaledInstance(this.backBufferWidth,
                                                   this.backBufferHeight,
                                                   //img.SCALE_DEFAULT);
                                                   img.SCALE_SMOOTH);
            } // else

            this.bgDrawMode = mode;
            cleanup();
        } // if
    } // setBackground


        // ImageObserver implementation...

    public synchronized boolean imageUpdate(Image img, int f, int x, int y, int w, int h)
    {
        if (img != this.bgImg)
            return(false);


        if ((f & SOMEBITS) != 0)
        {
            Graphics g = getGraphics();
            drawBackgroundImage(g);
            g.dispose();
            return(true);
        } // if

        if ( ((f & ALLBITS) != 0) || ((f & FRAMEBITS) != 0) )
        {
            cleanup();
            return(false);
        } // if

        if ( ((f & ERROR) != 0) || ((f & ABORT) != 0) )
        {
            this.bgImg.flush();
            this.bgImg = null;
            return(false);
        } // if

        return(true);
    } // imageUpdate
    */

} // SwingRenderer

// end of SwingRenderer.java ...

