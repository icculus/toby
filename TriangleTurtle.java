/**
 *  This class extends Turtle, and makes a happy little green
 *   triangle friend. Not for the trigonometrically faint of heart.
 *
 *  Copyright (c) Lighting and Sound Technologies, 1997.
 *   Written by Ryan C. Gordon.
 */

import java.awt.Graphics;
import java.awt.Point;
import java.awt.Color;

public final class TriangleTurtle extends Turtle
{
    private static final Color DEFAULT_TURTLE_COLOR = Color.green;
    private static final Color DEFAULT_CENTER_COLOR = Color.blue;

    private boolean isVis = true;       // Turtle visible?
    private Color turtleColor;          // Color of turtle.
    private Color centerColor;          // Color of center line.
    private double centerLineHalf;      // ((Size of center line) / 2).

        /*
         * These store the results of calculating the dimensions of the Turtle.
         *
         * The Triangle these doubles define would draw in at position (0, 0)
         *  in TurtleSpace. They are offset and stored in (intsX) and (intsY)
         *  during paintImpl() for correct placement.
         */
    private double[] pointsX;
    private double[] pointsY;
    private DoublePoint centerDbl;

        /*
         * These store the less-precise screen coordinates for the Turtle's
         *  polygon points, but we keep them as instance variables,
         *  so we don't make a new array every time we call paintImpl() (which
         *  can be several thousand times even in a simple TOBY program...)
         */
    private int[] intsX;
    private int[] intsY;
    private Point centerInt;


        /**
         * Create a new TriangleTurtle, that's green with a blue line.
         */
    public TriangleTurtle()
    {
        super();
        turtleColor = DEFAULT_TURTLE_COLOR;
        centerColor = DEFAULT_CENTER_COLOR;
    } // Constructor


        /**
         * This gives us a change to create our arrays before the
         *  superclass's constructor forces us to use them. This is
         *  the first method the superclass calls.
         */
    protected void initTurtle()
    {
        pointsX = new double[3];
        pointsY = new double[3];
        intsX = new int[3];
        intsY = new int[3];
        centerInt = new Point();
    } // initTurtle


        /**
         * Basically, we'll use this notification to readjust our
         *  size-related member (centerHalfLine), which is used in
         *  to calculate the Turtle. These are ignored if Turtle is
         *  not visible.
         *
         *    @param newSize Size of turtle. This the size of one side
         *                   of the triangle.
         */
    protected void sizeNotify(double newSize)
    {
            // We'll use the Pythagorian Theorem to figure out how long
            //  the center line of the triangle is. Then, divided by two,
            //  we'll have half the center line. This is needed for the
            //  calcTriangle() method. Splitting an equilateral triangle down
            //  the center gives us a right triangle of which we know the
            //  length of two of the sides, (sideLength, and sideLength / 2)
            //  so we can use the Pythagorian Theorem...have you hugged your
            //  math teacher today?  :)

        centerLineHalf = TobyGeometry.pythagorian(newSize / 2, newSize) / 2;

        if (isVis)
            calcTriangle(getAngle(), newSize);
    } // setTurtleSize


        /**
         * Take advantage of the notification of a new angle to recalculate
         *  the triangle. These are ignored if Turtle is not visible.
         *
         *  @param newAngle New angle Turtle is facing.
         */
    protected void angleNotify(double newAngle)
    {
        if (isVis)
            calcTriangle(newAngle, getTurtleSize());
    } // angleNotify


        /**
         * Set a flag signifying whether Turtle is visible. If made visible,
         *  recalculate the Triangle, to make sure it's up to date.
         *
         *    @param isVis Is Turtle now visible?
         */
    protected void visibleNotify(boolean isVis)
    {
        this.isVis = isVis;
        if (isVis == true)
            calcTriangle(getAngle(), getTurtleSize());
    } // visibleNotify


        /*
         * Notifies us that the Turtle has changed positions, but not angle.
         *  We update our screen coordinates accordingly.
         *
         *    @param x New X coordinate.
         *    @param y New Y coordinate.
         */
    protected void positionNotify(double x, double y)
    {
        if (isVis == true)
            calcTriangleLocation(x, y);
    } // positionNotify


        /**
         * We need to take into account the direction that the turtle is
         *  facing when we draw him. This complicates the drawing process
         *  considerably, but makes me greatful that I occasionally picked
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
    private void calcTriangle(double angle, double sideLength)
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
        calcTriangleLocation(getX(), getY());

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


        /**
         * Actual painting is done here. Take the data we calculated in
         *  calcTriangle(), add the Turtle's current coordinates to those
         *  points, and draw.
         *
         *    @param g Graphics context to draw Turtle upon.
         */
    protected void paintImpl(Graphics g)
    {
        g.setColor(turtleColor);              // Color and...

                //  ...draw the triangle.
        g.fillPolygon(intsX, intsY, intsX.length);

        g.setColor(centerColor);              // draw center line.
        g.drawLine(intsX[0], intsY[0], centerInt.x, centerInt.y);
    } // paintImpl

} // TriangleTurtle

// end of TriangleTurtle.java ...

