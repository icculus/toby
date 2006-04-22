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

import java.awt.Color;
import last.toby.exceptions.ExecException;

    /**
     *  !!! comment.
     *   @author Ryan C. Gordon. (icculus@icculus.org)
     */
public interface TurtleSpaceRenderer
{
    public abstract void notifyGrabbed();
    public abstract void notifyUngrabbed();

    public abstract double getTurtleSpaceWidth();
    public abstract double getTurtleSpaceHeight();
    public abstract double getDesiredTurtleWidth();
    public abstract double getDesiredTurtleHeight();

    public abstract void renderString(double x, double y, double angle,
                                      Color c, String str);
    public abstract void renderLine(double x1, double y1, double x2, double y2, Color c);
    public abstract void renderTurtle(Turtle turtle) throws ExecException;
    public abstract void blankTurtle(Turtle turtle) throws ExecException;

    public abstract void cleanup() throws ExecException;
} // TurtleSpaceRenderer

// end of TurtleSpaceRenderer.java ...

