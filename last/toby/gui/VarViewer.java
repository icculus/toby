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

package last.toby.gui;

import java.awt.*;
import java.util.*;
import javax.swing.*;
import last.toby.interpreter.*;

    /**
     * This class is used to display defined TOBY variables, and their current
     *  values.
     *
     *    @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public final class VarViewer extends JComponent implements VarWatcher
{
    protected Stack globals = null;
    protected Stack locals = null;
    protected int framePointer = 0;

    public VarViewer()
    {
        super();
        setBackground(Color.white);
        setDoubleBuffered(true);
    } // Constructor


    private void drawVar(Graphics g, Intrinsic var,
                         boolean changed,
                         int varX, int valX, int y,
                         FontMetrics fm)
    {
        String str;

        g.setColor((changed) ? Color.red : Color.blue);

        str = var.getIdentifier();
        g.drawString((str == null) ? "(null)" : str, varX + 2, y);

        str = var.toString();
        if (str == null)
            str = "(null)";
        int valOffset = fm.stringWidth(str);
        g.drawString(str, (valX - 2) - valOffset, y);
    } // drawVar


    public void paintComponent(Graphics g)
    {
        int i;
        int y = 0;
        int lineCount;
        Dimension d = getSize();
        int halfWidth = d.width / 2;
        FontMetrics fm = g.getFontMetrics(getFont());
        int globalCount = -1;
        int localCount = -1;

        if (this.globals != null)
            globalCount = this.globals.size();

        if (this.locals != null)
            localCount = this.locals.size();

/*
System.out.println("framepointer == (" + this.framePointer + ").");
System.out.println("localCount == (" + localCount + ").");
for (int v = 0; v < localCount; v++)
    System.out.println(" - " + ((Intrinsic) this.locals.elementAt(v)).getIdentifier());
*/

        lineCount = d.height / fm.getHeight();

        for (i = 0; i < lineCount; i++)
        {
            g.setColor(Color.black);
            y += fm.getHeight();
            g.drawLine(0, y, d.width, y);

                // !!! falses in drawVar calls need to change!

            if (i < globalCount)
            {
                drawVar(g, (Intrinsic) this.globals.elementAt(i),
                        false, 0, halfWidth, y, fm);
            } // if

            if ((this.locals != null) && (this.framePointer + i < localCount))
            {
                drawVar(g,
                        (Intrinsic) this.locals.elementAt(this.framePointer+i),
                        false, halfWidth, d.width, y, fm);
            } // if
        } // for

        g.setColor(Color.black);
        g.drawLine(halfWidth, 0, halfWidth, d.height);
    } // paint


        // VarWatcher implementation...

    public void varDefineGlobals(Stack _globalStack)
    {
        this.globals = _globalStack;
    } // varSetGlobals

    public void varDefineLocalStack(Stack _localStack)
    {
        this.locals = _localStack;
    } // varSetLocalStack

    public void varNewStackFrame(int pos)
    {
        this.framePointer = pos;
        paintImmediately(0, 0, getWidth(), getHeight());
    } // varSetFramePointer

    public void varUpdated(VarReferenceIntrinsic var)
    {
        paintImmediately(0, 0, getWidth(), getHeight());
    } // varUpdated

} // VarViewer

// end of VarViewer.java ...


