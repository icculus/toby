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

package last.toby.turtlespace.spotlet;
import last.toby.turtlespace.TurtleSpace;
import last.toby.exceptions.ExecException;
import last.toby.util.*;
import com.sun.kjava.*;

    /**
     *  !!! comment.
     *   @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public class SpotletTurtleSpace extends Spotlet implements TurtleSpace
{
    protected static Graphics g = Graphics.getGraphics();
    protected PalmTurtle[] turtles = null;
    protected PalmTurtle turtle = null;      // Current turtle.

    public SpotletTurtleSpace()
    {
        super();
    } // Constructor


    public void finalize() throws Throwable
    {
        releaseTurtleSpace();

        for (int i = 0; i < turtles.length; i++)
            turtles[i] = null;
        turtles = null;

        super.finalize();
    } // finalize


    public synchronized void paint()
    {
        if (turtles != null)
        {
            for (int i = 0; i < turtles.length; i++)
            {
                if (turtles[i] != null)
                    turtles[i].paint(g);
            } // for
        } // if
    } // paint


        // TurtleSpace implementation...

    public synchronized void grabTurtleSpace()
    {
        register(NO_EVENT_OPTIONS);   // need to get keys to abort.
        g.clearScreen();
    } // grabTurtleSpace


    public synchronized void releaseTurtleSpace()
    {
    } // releaseTurtleSpace


    public synchronized double getTurtleSpaceWidth()
    {
        return((double) getWidth());
    } // getTurtleSpaceWidth


    public synchronized double getTurtleSpaceHeight()
    {
        return((double) getHeight());
    } // getTurtleSpaceHeight


    public double getDesiredTurtleWidth()
    {
        return(5.0);
    } // getDesiredTurtleWidth


    public double getDesiredTurtleHeight()
    {
        return(getDesiredTurtleWidth());
    } // getDesiredTurtleHeight


    protected synchronized Turtle getTurtleByIndex(int turtleIndex)
    {
        PalmTurtle retVal = null;

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


    public synchronized int addTurtle()
    {
        Turtle newTurtle = new PalmTurtle();
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

        PalmTurtle newArray[] = new PalmTurtle[max + 1];

        if (turtles != null)
            System.arraycopy(turtles, 0, newArray, 0, max);

        newArray[max] = newTurtle;
        turtles = newArray;
        return(max);
    } // addTurtle


    public synchronized boolean removeTurtle(int turtleIndex)
    {
        boolean retVal = false;
        PalmTurtle t = getTurtleByIndex(turtleIndex);

        if (turtle == t)
            this.turtle = null;

        if (t != null)
        {
            t.setVisible(false);
            turtles[turtleIndex] = null;
            retVal = true;
        } // if

        return(retVal);
    } // removeTurtle


    public synchronized void removeAllTurtles()
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
            PalmTurtle t;

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
            if ( (x < 0.00) || (x > (double) getWidth()) ||
                 (y < 0.00) || (y > (double) getHeight()) )
            {
                ExecException._throw(TobyLanguage.TURTLE_FENCED);
            } // if
        } // if
    } // verifyTurtleInsideFence


    public synchronized void useTurtle(int turtleIndex) throws ExecException
    {
        PalmTurtle newTurtle = getTurtleByIndex(turtleIndex);
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
        turtle.setX(x);
        verifyTurtleInsideFence(x, turtle.getY());
    } // setTurtleX


    public synchronized void setTurtleY(double y) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.setY(y);
        verifyTurtleInsideFence(turtle.getX(), y);
    } // setTurtleY


    public synchronized void setTurtleXY(double x, double y) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.setXY(x, y);
        verifyTurtleInsideFence(x, y);
    } // setTurtleXY


    public synchronized void setTurtleWidth(double newWidth) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.setWidth(newWidth);
    } // setTurtleWidth


    public synchronized void setTurtleHeight(double newHeight) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.setHeight(newHeight);
    } // setTurtleHeight

    public synchronized void setTurtleVisibility(boolean isVis) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.setVisible(isVis);
    } // setTurtleVisible
        
    public synchronized void showTurtle() throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.setVisible(true);
    } // showTurtle

    public synchronized void showAllTurtles() throws ExecException
    {
        for (int i = 0; i < turtles.length; i++)
        {
            if (turtles[i] != null)
                turtles[i].setVisible(false);
        } // for
    } // showAllTurtles

    public synchronized void hideTurtle() throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.setVisible(false);
    } // hideTurtle

    public synchronized void hideAllTurtles() throws ExecException
    {
        for (int i = 0; i < turtles.length; i++)
        {
            if (turtles[i] != null)
                turtles[i].setVisible(false);
        } // for
    } // hideAllTurtles

    public float getDefaultPenRed()    { return((float) 0.0); }
    public float getDefaultPenBlue()   { return((float) 0.0); }
    public float getDefaultPenGreen()  { return((float) 0.0); }


    public synchronized void advanceTurtle(double distance) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);

        DoublePoint point;

            // !!! Move all this to PalmTurtle.java to avoid get...() overhead.
        double angle = turtle.getAngle();
        double turtleX = turtle.getX();
        double turtleY = turtle.getY();

        point = TobyGeometry.calculateLine(angle, distance, turtleX, turtleY);

        if (turtle.isPenDown())   // draw the line covering path turtle took?
        {
            int x1 = TobyGeometry.roundDoubleToInt(turtleX);
            int y1 = TobyGeometry.roundDoubleToInt(turtleY);
            int x2 = TobyGeometry.roundDoubleToInt(point.x);
            int y2 = TobyGeometry.roundDoubleToInt(point.y);
            Color c = turtle.getPenColor();

            screenGraphics.setColor(c);
            imgGraphics.setColor(c);
            screenGraphics.drawLine(x1, y1, x2, y2);
            imgGraphics.drawLine(x1, y1, x2, y2);
        } // if

        turtle.setXY(point.x, point.y);
        verifyTurtleInsideFence(point.x, point.y);
    } // advanceTurtle


    public synchronized void rotateTurtle(double degree) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.rotate(degree);
    } // rotateTurtle


    public synchronized void setTurtleAngle(double angle) throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        turtle.setAngle(angle);
    } // setTurtleAngle


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


    protected synchronized void homeTurtle(PalmTurtle t)
    {
        int x = getWidth() / 2;
        int y = getHeight() / 2;

        t.setXY(x, y);
    } // homeTurtle (overload; takes a PalmTurtle object)


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


    public synchronized void cleanup()
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


    protected synchronized void defaultTurtle(PalmTurtle t)
    {
        if (t != null)
        {
            float r = getDefaultPenRed();
            float g = getDefaultPenGreen();
            float b = getDefaultPenBlue();

            t.setAngle(PalmTurtle.DUE_NORTH);
            t.setPenDown(true);
            homeTurtle(t);
            t.setPenColor(r, g, b);
            t.setVisible(true);
        } // if
    } // defaultTurtle (overload; takes a PalmTurtle object)


    public synchronized void defaultTurtle() throws ExecException
    {
        if (turtle == null)
            ExecException._throw(TobyLanguage.NOCURTURTLE);
        defaultTurtle(turtle);
    } // defaultTurtle (overload; public version that uses current turtle.)


    public synchronized void defaultAllTurtles()
    {
        if (turtles != null)
        {
            for (int i = 0; i < turtles.length; i++)
                defaultTurtle(turtles[i]);
        } // if
    } // defaultAllTurtles


        // Some Swing-specific features...

    public synchronized void setBackground(Image img, int mode)
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

} // JTurtleSpace

// end of JTurtleSpace.java ...

