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
    private Color turtleColor;      // Color of turtle.
    private Color centerColor;      // Color of center line.
    private double centerHalf;      // ((Size of center line) / 2).


    public TriangleTurtle()
    {
        super();
        turtleColor = Color.green;   // !!! make constants?
        centerColor = Color.blue;
    } // Constructor


    public void setTurtleSize(double x)
    /**
     *  Basically, we'll use this to readjust our size-related
     *   member (centerHalfLine), which is used in paintInternal();
     *
     *     params : x == size of turtle. This the size of one side of triangle.
     *    returns : void.
     */
    {
            // We'll use the Pythagorian Theorem to figure out how long
            //  the center line of the triangle is. Then, divided by two,
            //  we'll have half the center line. This is needed for the
            //  paint() method. Splitting a equilateral triangle down the
            //  center gives us a right triangle of which we know the length
            //  of two of the sides, (sideLength, and sideLength / 2) so we
            //  can use the Pythagorian Theorem...have you hugged your math
            //  teacher today?  :)

        centerHalf = TobyGeometry.pythagorian(x / 2, x);
        super.setTurtleSize(x);
    } // setTurtleSize



    public void paintImpl(Graphics[] g)
    /**
     *  We need to take into account the direction that the turtle is
     *   facing when we draw him. This complicates the drawing process
     *   considerably, but makes me greatful that I occasionally picked
     *   up on what my high school math teachers were trying to explain.
     *   (Thanks Mrs. Strohm and Mrs. Tannery!)
     *
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
        double sideLength = getTurtleSize();
        double angle = getAngle();
        int i;
        DoublePoint point;
        DoublePoint centerPoint = getXY();
        double halfSide = sideLength / 2;
        double[] pointsX = new double[3];
        double[] pointsY = new double[3];
        int[] intsX = new int[3];
        int[] intsY = new int[3];

            // Calculate front-facing point of triangle...
            //  We start from dead center of turtle, which we're storing in
            //  centerPoint, and head out half the length of the centerline,
            //  which we calculated when we sized this component, and stored
            //  in centerHalf. We assign the calculated point to the arrays.

        point = TobyGeometry.calculateLine(angle, centerHalf, centerPoint);
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
            //  We'll store this point in our temp variable, (point).

        point = TobyGeometry.calculateLine(angle, halfSide,
                                           pointsX[1], pointsY[1]);

           // round all the points for screen placement...
        for (i = 0; i < pointsX.length; i++)
        {
            intsX[i] = TobyGeometry.roundDoubleToInt(pointsX[i]);
            intsY[i] = TobyGeometry.roundDoubleToInt(pointsY[i]);
        } // for

        for (i = 0; i < g.length; i++)
        {
            g[i].setColor(turtleColor);              // Color and...

                //  ...draw the triangle.
            g[i].fillPolygon(intsX, intsY, 3);

            g[i].setColor(centerColor);              // draw center line.
            g[i].drawLine(intsX[0], intsY[0], (int) point.x, (int) point.y);
        } // for

                // Wow, wasn't that fun? Of course it was.
    } // paintImpl

} // TriangleTurtle

// end of TriangleTurtle.java ...

