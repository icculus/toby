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

package last.toby.turtlespace.sdl;

import java.awt.Color;
import last.toby.turtlespace.*;
import last.toby.exceptions.ExecException;

    /**
     *  !!! comment.
     *   @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public class SDLRenderer implements TurtleSpaceRenderer
{
    static { System.loadLibrary("SDLRenderer"); }

    public SDLRenderer()
    {
        native_Constructor();
    } // Constructor

    public void notifyGrabbed()
    {
    } // notifyGrabbed

    public void notifyUngrabbed()
    {
    } // notifyUngrabbed

    public double getTurtleSpaceWidth()
    {
        return(640);
    } // getTurtleSpaceWidth

    public double getTurtleSpaceHeight()
    {
        return(480);
    } // getTurtleSpaceHeight

    public double getDesiredTurtleWidth()
    {
        return(640 * 0.02);
    } // getDesiredTurtleWidth

    public double getDesiredTurtleHeight()
    {
        return(640 * 0.02);
    } // getDesiredTurtleHeight

    public void renderString(double x, double y, double angle,
                                      Color c, String str)
    {
        //System.err.println("SDLRenderer.renderString() is a no-op, currently.");
    } // renderString

    public void renderLine(double x1, double y1, double x2, double y2, Color c)
    {
        native_renderLine(x1, y1, x2, y2, c.getRed(), c.getGreen(), c.getBlue());
    } // renderLine

    public void renderTurtle(Turtle turtle) throws ExecException
    {
        //System.err.println("SDLRenderer.renderTurtle() is a no-op, currently.");
    } // renderTurtle

    public void blankTurtle(Turtle turtle) throws ExecException
    {
        //System.err.println("SDLRenderer.blankTurtle() is a no-op, currently.");
    } // blankTurtle

    public void cleanup() throws ExecException
    {
        native_cleanup();
    } // cleanup

    private static native void native_Constructor();
    private static native void native_renderLine(double x1, double y1, double x2, double y2, int r, int g, int b);
    private static native void native_cleanup();
} // SDLRenderer

// end of SDLRenderer.java ...

