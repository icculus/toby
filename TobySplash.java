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
 * !!! comment.
 */

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


public class TobySplash extends JDialog implements ActionListener
{
    Image img;
    Timer timer;

    public TobySplash(String filename)
    {
        super();
//        img = Toolkit.getDefaultToolkit().createImage(filename);
        timer = new Timer(5000, this);
        timer.setRepeats(false);
        setSize(500, 500);
        show();
        timer.start();
    } // Constructor


    public void update(Graphics g)
    {
        paint(g);
    } // update

    public void paintComponent(Graphics g)
    {
        if (img != null)
            g.drawImage(img, 0, 0, this);
        else
            g.drawString("Toby", 0, 0);  // !!!

//        g.setColor(Color.YELLOW);
//        g.drawString  VERSION.

    } // paintComponent


        // ActionListener interface...

    public void actionPerformed(ActionEvent e)
    {
        timer = null;
        img.flush();
        img = null;
        dispose();
    } // actionPerformed

} // TobySplash

// end of TobySplash.java ...

