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
import last.toby.exceptions.*;
import last.toby.util.*;

    /**
     *  !!! comment.
     *   @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public class TurtleSpace
{
    private Turtle[] turtles = null;
    private java.util.Stack turtlePool = new java.util.Stack();
    private boolean hasFence = false;
    private Turtle turtle = null;      // Current turtle.
    private TurtleSpaceRenderer renderer = null;

// !!! Would like an alpha channel.
// !!! Would like a way to redefine the coordinate system of TurtleSpace.

    public TurtleSpace(TurtleSpaceRenderer _renderer)
    {
        this.renderer = _renderer;
    } // Constructor

    public void grabTurtleSpace()
    {
        renderer.notifyGrabbed();
    } // grabTurtleSpace

    public void releaseTurtleSpace()
    {
        renderer.notifyUngrabbed();
    } // grabTurtleSpace

    protected synchronized Turtle getTurtleByIndex(int turtleIndex)
    {
        Turtle retVal = null;

        if (turtles != null)
        {
            if ((turtleIndex >= 0) && (turtleIndex < turtles.length))
                retVal = turtles[turtleIndex];
        } // if

        return(retVal);
    } // getTurtleByIndex


    public synchronized int getTurtleCount()
    {
        int retVal = 0;

        if (turtles != null)
        {
            for (int i = 0; i < turtles.length; i++)
            {
                if (turtles[i] != null)
                    retVal++;
            } // for
        } // if

        return(retVal);
    } // getTurtleCount


    protected synchronized Turtle buildNewTurtle() throws ExecException
    {
        Turtle retVal = null;

        if (turtlePool.isEmpty() == false)
            retVal = (Turtle) turtlePool.pop();
        else
            retVal = new Turtle();

        defaultTurtle(retVal);

        return(retVal);
    } // buildNewTurtle


    public synchronized int addTurtle() throws ExecException
    {
        Turtle newTurtle = buildNewTurtle();
        int max = 0;

        if (turtles != null)
        {
            max = turtles.length;
            for (int i = 0; i < max; i++)
            {
                if (turtles[i] == null)
                {
                    turtles[i] = newTurtle;
                    return(i);
                } // if
            } // for
        } // if

            // if we landed here, we need to increase the size of the array...

        Turtle newArray[] = new Turtle[max + 1];

        if (turtles != null)
            System.arraycopy(turtles, 0, newArray, 0, max);

        newArray[max] = newTurtle;
        turtles = newArray;
        return(max);
    } // addTurtle


    public synchronized boolean removeTurtle(int turtleIndex) throws ExecException
    {
        boolean retVal = false;
        Turtle t = getTurtleByIndex(turtleIndex);

        if (turtle == t)
            this.turtle = null;

        if (t != null)
        {
            renderer.blankTurtle(t);
            t.setVisible(false);
            turtlePool.push(t);
            turtles[turtleIndex] = null;
            retVal = true;
        } // if

        return(retVal);
    } // removeTurtle


    public synchronized void removeAllTurtles() throws ExecException
    {
        if (turtles != null)
        {
            int max = turtles.length;

            for (int i = 0; i < max; i++)
                removeTurtle(i);
        } // if
    } // removeAllTurtles


    public void enableFence() throws ExecException
    {
        setFence(true);
    } // setFence


    public void disableFence() throws ExecException
    {
        setFence(false);
    } // setFence


    public synchronized void setFence(boolean buildFence) throws ExecException
    {
        this.hasFence = buildFence;

        if ((buildFence == true) && (turtles != null))
        {
            Turtle t;

            for (int i = 0; i < turtles.length; i++)
            {
                t = turtles[i];
                if (t != null)
                    verifyTurtleInsideFence(t.getX(),t.getY());
            } // for
        } // if
    } // setFence


    protected synchronized void verifyTurtleInsideFence(double x, double y)
                                                         throws ExecException
    {
        if (this.hasFence == true)
        {
            if ( (x < 0.00) || (x > renderer.getTurtleSpaceWidth()) ||
                 (y < 0.00) || (y > renderer.getTurtleSpaceHeight()) )
            {
                ExecException._throw(TobyLanguage.TURTLE_FENCED);
            } // if
        } // if
    } // verifyTurtleInsideFence


    public synchronized void useTurtle(int turtleIndex) throws ExecException
    {
        Turtle newTurtle = getTurtleByIndex(turtleIndex);
        if (newTurtle == null)
            ExecException._throw(TobyLanguage.BAD_ARGUMENT);
        else
            this.turtle = newTurtle;
    } // useTurtle


    public synchronized double getTurtleX() throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        return(this.turtle.getX());
    } // getTurtleX


    public synchronized double getTurtleY() throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        return(turtle.getY());
    } // getTurtleY


    public synchronized double getTurtleWidth() throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        return(turtle.getWidth());
    } // getTurtleWidth


    public synchronized double getTurtleHeight() throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        return(turtle.getHeight());
    } // getTurtleHeight


    public synchronized void setTurtleX(double x) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        if (x != turtle.getX())
        {
            renderer.blankTurtle(turtle);
            turtle.setX(x);
            renderer.renderTurtle(turtle);
            verifyTurtleInsideFence(x, turtle.getY());
        } // if
    } // setTurtleX


    public synchronized void setTurtleY(double y) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        if (y != turtle.getY())
        {
            renderer.blankTurtle(turtle);
            turtle.setY(y);
            renderer.renderTurtle(turtle);
            verifyTurtleInsideFence(turtle.getX(), y);
        } // if
    } // setTurtleY


    public synchronized void setTurtleXY(double x, double y) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        if ((y != turtle.getY()) || (x != turtle.getX()))
        {
            renderer.blankTurtle(turtle);
            turtle.setXY(x, y);
            renderer.renderTurtle(turtle);
            verifyTurtleInsideFence(x, y);
        } // if
    } // setTurtleXY


    public synchronized void setTurtleWidth(double newWidth) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        if (newWidth != turtle.getWidth())
        {
            renderer.blankTurtle(turtle);
            turtle.setWidth(newWidth);
            renderer.renderTurtle(turtle);
        } // if
    } // setTurtleWidth


    public synchronized void setTurtleHeight(double newHeight) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        if (newHeight != turtle.getHeight())
        {
            renderer.blankTurtle(turtle);
            turtle.setHeight(newHeight);
            renderer.renderTurtle(turtle);
        } // if
    } // setTurtleHeight

    protected synchronized void setTurtleVisibility(Turtle t, boolean isVis) throws ExecException
    {
        if (isVis != t.getVisible())
        {
            t.setVisible(isVis);

            if (isVis)
                renderer.renderTurtle(t);
            else
                renderer.blankTurtle(t);
        } // if
    } // setTurtleVisibility

    public synchronized void setTurtleVisibility(boolean isVis) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        setTurtleVisibility(turtle, isVis);
    } // setTurtleVisible
        
    public synchronized void showTurtle() throws ExecException
    {
        setTurtleVisibility(true);
    } // showTurtle

    public synchronized void showAllTurtles() throws ExecException
    {
        for (int i = 0; i < turtles.length; i++)
        {
            if (turtles[i] != null)
                setTurtleVisibility(turtles[i], true);
        } // for
    } // showAllTurtles

    public synchronized void hideTurtle() throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        setTurtleVisibility(turtle, false);
    } // hideTurtle

    public synchronized void hideAllTurtles() throws ExecException
    {
        for (int i = 0; i < turtles.length; i++)
        {
            if (turtles[i] != null)
                setTurtleVisibility(turtles[i], false);
        } // for
    } // hideAllTurtles

    public float getDefaultPenRed()    { return((float) 1.0); }
    public float getDefaultPenBlue()   { return((float) 1.0); }
    public float getDefaultPenGreen()  { return((float) 1.0); }


        /**
         * Move turtle forward (distance) length in whatever direction he's
         *  facing. If the "pen" is "down", then draw the appropriate lines
         *  in TurtleSpace. Negative values move the turtle backwards, or
         *  directly opposite of the way he's facing. The angle the turtle
         *  is pointing does not change.
         *
         *    @param distance How far to move the turtle.
         */
    public synchronized void advanceTurtle(double distance) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        DoublePoint point;

        double angle = turtle.getAngle();
        double turtleX = turtle.getX();
        double turtleY = turtle.getY();

        point = TobyGeometry.calculateLine(angle, distance, turtleX, turtleY);

        renderer.blankTurtle(turtle);

        if (turtle.isPenDown())   // draw the line covering path turtle took?
        {
            renderer.renderLine(turtleX, turtleY,
                                point.x, point.y, turtle.getPenColor());
        } // if

        turtle.setXY(point.x, point.y);
        renderer.renderTurtle(turtle);
        verifyTurtleInsideFence(point.x, point.y);
    } // advanceTurtle


    public synchronized void setTurtleAngle(double angle) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        renderer.blankTurtle(turtle);
        turtle.setAngle(angle);
        renderer.renderTurtle(turtle);
    } // setTurtleAngle


    public synchronized void rotateTurtle(double degree) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        setTurtleAngle(turtle.getAngle() + degree);
    } // rotateTurtle


    public synchronized void setPenColor(float r, float g, float b) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.setPenColor(r, g, b);
    } // setPenColor


    public void setPen(boolean drawing) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.setPenDown(drawing);
    } // setPen


    public void setPenUp() throws ExecException
    {
        setPen(false);
    } // setPenUp


    public void setPenDown() throws ExecException
    {
        setPen(true);
    } // setPenDown


    protected synchronized void homeTurtle(Turtle t) throws ExecException
    {
        int x = TobyGeometry.roundDoubleToInt(renderer.getTurtleSpaceWidth() / 2);
        int y = TobyGeometry.roundDoubleToInt(renderer.getTurtleSpaceHeight() / 2);

        renderer.blankTurtle(t);
        t.setXY(x, y);
        renderer.renderTurtle(t);
    } // homeTurtle (overload; takes a Turtle object)


    public synchronized void homeTurtle() throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        homeTurtle(turtle);
    } // homeTurtle (overload; public version that uses current turtle.)


    public synchronized void homeAllTurtles() throws ExecException
    {
        for (int i = 0; i < turtles.length; i++)
        {
            if (turtles[i] != null)
                homeTurtle(turtles[i]);
        } // for
    } // homeAllTurtles


    protected synchronized void defaultTurtle(Turtle t) throws ExecException
    {
        if (t != null)
        {
            float r = getDefaultPenRed();
            float g = getDefaultPenGreen();
            float b = getDefaultPenBlue();

            renderer.blankTurtle(t);
            t.setAngle(Turtle.DUE_NORTH);
            t.setPenDown(true);
            t.setPenColor(r, g, b);
            t.setVisible(true);
            homeTurtle(t);
            renderer.renderTurtle(t);
        } // if
    } // defaultTurtle (overload; takes a Turtle object)


    public synchronized void defaultTurtle() throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        defaultTurtle(turtle);
    } // defaultTurtle (overload; public version that uses current turtle.)


    public synchronized void defaultAllTurtles() throws ExecException
    {
        if (turtles != null)
        {
            for (int i = 0; i < turtles.length; i++)
                defaultTurtle(turtles[i]);
        } // if
    } // defaultAllTurtles


    public void drawString(String str) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        renderer.renderString(turtle.getX(), turtle.getY(), turtle.getAngle(),
                               turtle.getPenColor(), str);
    } // renderString

    public void cleanup() throws ExecException
    {
        renderer.cleanup();
    } // cleanup

    public double getTurtleSpaceWidth()
    {
        return(renderer.getTurtleSpaceWidth());
    } // getTurtleSpaceWidth

    public double getTurtleSpaceHeight()
    {
        return(renderer.getTurtleSpaceHeight());
    } // getTurtleSpaceWidth

} // TurtleSpace

// end of TurtleSpace.java ...

