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
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.event.*;
import last.toby.util.TobyLanguage;

    /**
     * This is a simple JComponent that outputs the current cursor
     *  location in a JTextArea everytime it is updated.
     *
     *    @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public class EditorInformationDisplay extends JComponent
                                                implements CaretListener
{
    private JTextArea editor = null;
    private int caretX = 1;
    private int caretY = 1;
    private String displayText = null;


    public EditorInformationDisplay(JTextArea areaToMonitor)
    {
        editor = areaToMonitor;
        editor.addCaretListener(this);
        setBackground(Color.blue.darker());
        setForeground(Color.yellow);
        setDisplayText();
        setDoubleBuffered(true);
    } // Constructor


    private void setDisplayText()
    {
        String str = TobyLanguage.CARET_POSITION;
        str = TobyLanguage.replaceFmtTokenInStr(0, str, caretY);
        str = TobyLanguage.replaceFmtTokenInStr(1, str, caretX);
        displayText = str;
    } // setDisplayText


    public void paintComponent(Graphics g)
    {
        int height = getHeight();
        int width = getWidth();
        FontMetrics fm = g.getFontMetrics();
        int fontDescent = fm.getMaxDescent();
        int stringWidth = fm.stringWidth(displayText);

        g.setColor(getBackground());
        g.fillRect(0, 0, width, height);

        g.setColor(getForeground());
        g.drawString(displayText, width - stringWidth, height - fontDescent);
    } // paintComponent


        // CaretListener implementation...

    public void caretUpdate(CaretEvent e)
    {
        int dotLoc = e.getDot();

        try
        {
            caretY = editor.getLineOfOffset(dotLoc);
            caretX = (dotLoc - editor.getLineStartOffset(caretY)) + 1;
            caretY++;   // make option base 1.
        } // try
        catch (BadLocationException ble)
        {
            caretX = caretY = -1;     // bogus values for error.
        } // catch

        setDisplayText();
        paintImmediately(0, 0, getWidth(), getHeight());
    } // caretUpdate

} // EditorInformationDisplay

// end of EditorInformationDisplay.java ...

