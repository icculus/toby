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
import java.awt.image.*;
import java.awt.event.*;
import javax.swing.*;
import last.toby.Toby;
import last.toby.util.TobyLanguage;

    /**
     * This is a "splash screen" component. It reads in a graphic from disk,
     *  and displays it in a centered window. The "version" string is drawn
     *  in the lower right-hand corner of the image. A slim (2 pixel) border
     *  is drawn around the graphic.
     *
     * This window remains until its doneWithSplash() method is called, at which
     *  time the window dispose()s of itself. This can be accomplished best by
     *  attaching a timer to another object or attaching an event listener to
     *  this.
     *
     *    @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public class TobySplash extends JWindow implements ActionListener
{

    public static final Color DEFAULT_VERCOLOR = Color.yellow;
    public static final Color DEFAULT_BORDERCOLOR = Color.black;

    protected Image img;
    protected Timer timer;
    protected int imgWidth;
    protected int imgHeight;
    protected int screenWidth;
    protected int screenHeight;
    protected String verString;
    protected Color verColor = DEFAULT_VERCOLOR;
    protected Color borderColor = DEFAULT_BORDERCOLOR;
    protected boolean printedBanner = false;


    public TobySplash(String filename, String verString,
                        Color verColor, Color borderColor)
    {
        super();

        this.verColor = verColor;
        this.borderColor = borderColor;

        Toolkit tk = Toolkit.getDefaultToolkit();
        img = tk.createImage(filename);
        if (img == null)
        {
            printBanner();
            dispose();
        } // if
        else
        {
            setBackground(borderColor);
            this.verString = verString;
            Dimension d = tk.getScreenSize();
            screenWidth = d.width;
            screenHeight = d.height;
            prepareImage(img, this);
        } // else
    } // Constructor (overloaded; takes colors).


    public TobySplash(String filename, String verString)
    {
        this(filename, verString, DEFAULT_VERCOLOR, DEFAULT_BORDERCOLOR);
    } // Constructor (overloaded; doesn't take colors).


    public static void printBanner()
    {
        System.out.println();
        System.out.println(Toby.TITLE + " " + Toby.VERSION);
        System.out.println("  " + TobyLanguage.COPYRIGHT);
        System.out.println("  " + TobyLanguage.WRITTENBY);
        System.out.println("  " + TobyLanguage.TRANSLATION_BY);
        System.out.println();
    } // printBanner


    public void doneWithSplash(int delayTime)
    {
        if (delayTime <= 0)
            actionPerformed(null);
        else
        {
            timer = new Timer(delayTime, this);
            timer.setRepeats(false);
            timer.start();
        } // else
    } // doneWithSplash


    public void paint(Graphics g)
    {
        FontMetrics fm = g.getFontMetrics();
        g.drawImage(img, 2, 2, this);
        g.setColor(verColor);
        g.drawString(verString,
                     (getWidth() - fm.stringWidth(verString)) - 2,
                     getHeight() - 2);
    } // paint


    private void drawImageFrame(int w, int h)
    {
        if (isVisible() == true)
            repaint();
        else
        {
            w += 4;   // adjust for 2-pixel wide border on each side...
            h += 4;
            setSize(w, h);
            setLocation((screenWidth - w) / 2, (screenHeight - h) / 2);
            show();
        } // else
    } // drawImageFrame


        // ActionListener implementation...

    public void actionPerformed(ActionEvent e)
    {
        timer = null;
        if (img != null)
        {
            img.flush();
            img = null;
        } // if
        dispose();
    } // actionPerformed


        // ImageObserver implementation...

    public boolean imageUpdate(Image img, int flags, int x, int y, int w, int h)
    {
        boolean retVal = true;

        if ((flags & ImageObserver.ALLBITS) != 0)
        {
            drawImageFrame(w, h);
            retVal = false;
        } // if

        else if ((flags & ImageObserver.FRAMEBITS) != 0)
            drawImageFrame(w, h);

        else if ( ((flags & ImageObserver.ERROR) != 0) ||
                  ((flags & ImageObserver.ABORT) != 0) )
        {
            if (printedBanner == false)
            {
                printBanner();   // Print this, since there's no image.
                printedBanner = true;
            } // if
            retVal = false;
            img.flush();
            img = null;
            dispose();
        } // else if

        return(retVal);
    } // imageUpdate

} // TobySplash

// end of TobySplash.java ...

