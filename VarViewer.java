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

/**
 * This class is used to display defined TOBY variables, and their current
 *  values.
 *
 *    Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;

import javax.swing.*;

public final class VarViewer extends JComponent implements VarWatcher
{
    private Vector vars;

    public VarViewer()
    {
        super();
        vars = new Vector();
        setBackground(Color.white);
        setDoubleBuffered(true);
    } // Constructor


    public void paintComponent(Graphics g)
    {
        int i;
        Intrinsic var;
        int y = 0;
        int lineCount;
        Dimension d = getSize();
        FontMetrics fm = g.getFontMetrics(getFont());
        String str;

        lineCount = d.height / fm.getHeight();

        for (i = 0; i < lineCount; i++)
        {
            g.setColor(Color.black);
            y += fm.getHeight();
            g.drawLine(0, y, d.width, y);
            if (vars.size() > i)
            {
                var = (Intrinsic) vars.elementAt(i);
                g.setColor(Color.blue);
                str = var.getIdentifier();
                g.drawString((str == null) ? "(null)" : str, 0, y);
                g.setColor(Color.red);
                str = var.getValue();
                g.drawString((str == null) ? "(null)" : str,
                             (int) (d.width * 0.75), y);
            } // if
        } // for
    } // paint


        // VarWatcher implementation...

    public void varBeginInterpretation()
    {
        vars.removeAllElements();
    } // varBeginInterpretation


    public void varEndInterpretation(boolean normalTermination)
    {
        if (normalTermination)
            vars.removeAllElements();
    } // varEndInterpretation


    public void varUpdated(Intrinsic var)
    {
        System.out.println("varUpdated(var == [" + var.toString() + "])...");
        repaint();
    } // varUpdate


    public void varCreated(Intrinsic var)
    {
        vars.addElement(var);
        repaint();
    } // varCreated


    public void varDestroyed(Intrinsic var)
    {
        vars.removeElement(var);
        repaint();
    } // varDestroyed


    public void varInScope(Intrinsic var)
    {
        // !!! color update ...
    } // varInScope


    public void varOutOfScope(Intrinsic var)
    {
        // !!! color update ...
    } // varOutOfScope

} // VarViewer

// end of VarViewer.java ...


