/**
 *  !!! comment
 *
 *
 *
 *
 */

import java.awt.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.event.*;


public class EditorInformationDisplay extends JComponent implements
                                                         CaretListener
{

    private JTextArea editor = null;
    private int caretX = 1;
    private int caretY = 1;
    private String displayText = null;


    public EditorInformationDisplay(JTextArea areaToMonitor)
    {
        editor = areaToMonitor;
        editor.addCaretListener(this);
        setBackground(Color.red);
        setForeground(Color.yellow);
        setDisplayText();
        setDoubleBuffered(true);
    } // Constructor


    private void setDisplayText()
    {
        StringBuffer sb = new StringBuffer();

        sb.append("line ");
        sb.append(caretY);
        sb.append(", column ");
        sb.append(caretX);
        displayText = sb.toString();
        sb.setLength(0);    // empty StringBuffer...
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
        repaint();
    } // caretUpdate

} // EditorInformationDisplay

// end of EditorInformationDisplay.java ...

