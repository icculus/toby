/**
 * This class is used to display defined TOBY variables, and their current
 *  values. A scroll bar is provided for when there are more variables to
 *  display than there is screen real estate.
 *
 *   Copyright (c) Lighting and Sound Technologies, 1997.
 *    Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;

public final class VarViewer extends Panel implements VarWatcher,
                                                      AdjustmentListener
{
    private Scrollbar scrollBar;
    private Vector vars;

    public VarViewer()
    {
        super();
        setLayout(new BorderLayout());
        vars = new Vector();
        setBackground(Color.white);
        scrollBar = new Scrollbar(Scrollbar.VERTICAL, 0, 0, 0, 0);
        scrollBar.addAdjustmentListener(this);
        scrollBar.setUnitIncrement(1);
        scrollBar.setBlockIncrement(1);
        add("East", scrollBar);
    } // Constructor


    private Dimension getDrawingAreaSize()
    {
        Dimension retVal = getSize();
        Dimension scrollSize = scrollBar.getSize();

        retVal.width -= scrollSize.width;
        return(retVal);
    } // getDrawingAreaSize


    public void paint(Graphics g)
    {
        int i;
        Intrinsic var;
        int y = 0;
        int lineCount;
        Dimension d = getDrawingAreaSize();
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

    public void varUpdated(Intrinsic var)
    {
        repaint();
    } // varUpdate


    protected void updateScrollbar()
    {
        Dimension d = getSize();
        FontMetrics fm = getFontMetrics(getFont());

        if ((d.height / fm.getHeight()) > vars.size())
            scrollBar.setEnabled(false);
        else
        {
            d = scrollBar.getSize();
            scrollBar.setMinimum(0);
            scrollBar.setMaximum(vars.size());
            scrollBar.setVisibleAmount(d.height / vars.size());
            scrollBar.setEnabled(true);
        } // else
    } // updateScrollbar


    public void varCreated(Intrinsic var)
    {
        vars.addElement(var);
        updateScrollbar();
        repaint();
    } // varCreated


    public void varDestroyed(Intrinsic var)
    {
        vars.removeElement(var);
        updateScrollbar();
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


        // AdjustmentListener implmentation ...
    public void adjustmentValueChanged(AdjustmentEvent e)
    {
    } // adjustmentValueChanged

} // VarViewer

// end of VarViewer.java ...


