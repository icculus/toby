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

import last.toby.exceptions.ExecException;

    /**
     *  !!! comment.
     *   @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public interface TurtleSpace
{
    public abstract void grabTurtleSpace();
    public abstract void releaseTurtleSpace();

    public abstract double getTurtleSpaceWidth();
    public abstract double getTurtleSpaceHeight();
    public abstract double getDesiredTurtleWidth();
    public abstract double getDesiredTurtleHeight();

    public abstract float getDefaultPenRed();
    public abstract float getDefaultPenBlue();
    public abstract float getDefaultPenGreen();

    public abstract int getTurtleCount();
    public abstract int addTurtle();
    public abstract boolean removeTurtle(int turtleIndex);
    public abstract void removeAllTurtles();

    public abstract void setFence(boolean buildFence) throws ExecException;
    public abstract void enableFence() throws ExecException;
    public abstract void disableFence() throws ExecException;

    public abstract void useTurtle(int turtleIndex)  throws ExecException;

    public abstract double getTurtleX()  throws ExecException;
    public abstract double getTurtleY()  throws ExecException;
    public abstract double getTurtleWidth()  throws ExecException;
    public abstract double getTurtleHeight() throws ExecException;
    public abstract void setTurtleX(double x) throws ExecException;
    public abstract void setTurtleY(double y) throws ExecException;
    public abstract void setTurtleXY(double x, double y)  throws ExecException;
    public abstract void setTurtleWidth(double newWidth) throws ExecException;
    public abstract void setTurtleHeight(double newHeight) throws ExecException;

        /**
         * Move turtle forward (distance) length in whatever direction he's
         *  facing. If the "pen" is "down", then draw the appropriate lines
         *  in TurtleSpace. Negative values move the turtle backwards, or
         *  directly opposite of the way he's facing. The angle the turtle
         *  is pointing does not change.
         *
         *    @param distance How far to move the turtle.
         */
    public abstract void advanceTurtle(double distance) throws ExecException;
    public abstract void rotateTurtle(double degree) throws ExecException;
    public abstract void setTurtleAngle(double angle) throws ExecException;
    public abstract void setPenColor(float r, float g, float b) throws ExecException;
    public abstract void setPen(boolean drawing) throws ExecException;
    public abstract void setPenUp() throws ExecException;
    public abstract void setPenDown() throws ExecException;
    public abstract void homeTurtle() throws ExecException;
    public abstract void homeAllTurtles() throws ExecException;
    public abstract void setTurtleVisibility(boolean isVis) throws ExecException;
    public abstract void showTurtle() throws ExecException;
    public abstract void showAllTurtles() throws ExecException;
    public abstract void hideTurtle() throws ExecException;
    public abstract void hideAllTurtles() throws ExecException;
    public abstract void defaultTurtle() throws ExecException;
    public abstract void defaultAllTurtles() throws ExecException;

    public abstract void cleanup();
} // TurtleSpace

// end of TurtleSpace.java ...

