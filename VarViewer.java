/**
 * This class is used to display defined TOBY variables, and their current
 *  values.
 *
 *   Copyright (c) Lighting and Sound Technologies, 1997.
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


    public void paint(Graphics g)
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


