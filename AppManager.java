/**
 *  This class will keep track of how many windows are open,
 *   and when they've all been closed, System.exit() is called.
 *
 *  To use: construct ONE appManager per application, and have
 *   all significant windows call addWindowListener(), pointing
 *   to that appManager.
 *
 *  It should be noted that, without a class like this, Frames
 *   won't close when you click the cute little X on it.
 *
 *  It should also be noted that, in the case of Applets, this
 *   class shouldn't be used, since we usually won't generate extra
 *   frame windows, and we never call System.exit() inside the
 *   sandbox, anyway.
 * 
 *    Copyright (c) Lighting and Sound Technologies, 1997.
 *     Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.awt.event.*;


public class AppManager extends WindowAdapter
{
    protected int frameCount = 0;

    public void windowOpened(WindowEvent e)
    {
        frameCount++;
    } // windowOpened


    public void windowClosing(WindowEvent e)
    {
        e.getWindow().dispose();
    } // windowClosing


    public void windowClosed(WindowEvent e)
    {
        frameCount--;

        if (frameCount <= 0)
            System.exit(0);
    } // windowClosed
} // AppManager


// end of AppManager.java ...

