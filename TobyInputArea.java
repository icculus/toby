/**
 *  This class will offer a place for users to enter and edit source code.
 *
 *    Copyright (c) Lighting and Sound Technologies, 1997.
 *     Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.Vector;

public class TobyInputArea extends Panel implements LayoutManager,
                                                    SourceWatcher
{
    private TextArea inputArea = null;
    private String endl;
    private Vector lineIndexes;

    public TobyInputArea()
    {
        endl = "\n";        // !!! System.getProperty("line.separator");
        lineIndexes = new Vector();

        buildComponents();
        setBackground(Color.gray);
        setForeground(Color.black);
        setLayout(this);
    } // Constructor


    public void setSource(BufferedReader inReader) throws IOException
    /**
     * Set the text area to source read from (inReader). This Reader
     *  is closed after use by this function.
     *
     *    params : inReader == where to read the source from.
     *   returns : void.
     *    throws : IOException on (inReader) errors...
     */
    {
        StringBuffer sb = new StringBuffer();
        String str;
        int ch;

        do
        {
            try
            {
                str = inReader.readLine();
            } // try
            catch (IOException ioe)
            {
                sb.setLength(0);
                inReader.close();
                throw(ioe);       // rethrow.
            } // catch

            if (str != null)
            {
                sb.append(str);
                sb.append(endl);
            } // if
        } while (str != null);

        clearCode();

        inputArea.setText(sb.toString());
        sb.setLength(0);
        inReader.close();
    } // setSource


    public String getSource()
    {
        return(inputArea.getText() + endl);
    } // getSource


    private void highlightLine(int lineNum)
    {
        int start;
        int end;

        lineNum--;   // line number is passed as option base 1, not 0...

        if ((lineNum < 0) || (lineNum >= lineIndexes.size() - 1))
            end = start = 0;
        else
        {
            start = ((Integer) lineIndexes.elementAt(lineNum)).intValue();
            end = ((Integer)lineIndexes.elementAt(lineNum + 1)).intValue();
        } // else

        inputArea.select(start, end);
    } // highlightLine


    public void runCode()
    {
        String src;
        TobyPanel panel = (TobyPanel) getParent();
        Toby tobyFrame = (Toby) panel.getParent();
        TurtleSpace tspace = panel.getTurtleSpace();

        setLineIndex();
        src = getSource();

        try
        {
            tspace.setCode(src);        // repaints.
        } // try
        catch (TobyParseException tpe)
        {
            highlightLine(tpe.errLine);
            TobyInterpreter.displayParseException(tpe, tobyFrame);
        } // catch
    } // runCode


    private void setLineIndex()
    {
        String src = getSource();
        int lastIndex = 0;
        int endlLen = endl.length();

        lineIndexes.removeAllElements();
        lineIndexes.addElement(new Integer(0));

        do
        {
            lastIndex = src.indexOf(endl, lastIndex);
            if (lastIndex != -1)
            {
                lastIndex += endlLen;
                lineIndexes.addElement(new Integer(lastIndex));
            } // if
        } while (lastIndex != -1);
    } // setLineIndex
    

    public void clearCode()
    {
        TurtleSpace tspace = ((TobyPanel) getParent()).getTurtleSpace();

        inputArea.setText("");              // nukes textarea.

        try
        {
            tspace.setCode(null);               // repaints tspace.
        } // try
        catch (TobyParseException tpe)
        {
            // never hits this; Don't care...
        } // catch
    } // clearCode


    private void buildComponents()
    /**
     *   Build and add all the components that are part of this
     *    TobyInputArea panel...
     *
     *      params : void.
     *     returns : void. Components are built, though.
     */
    {
        inputArea = new TextArea();
        inputArea.setEditable(true);
        add(inputArea);
    } // buildComponents


    public void paint(Graphics g)
    {
        Dimension d = getSize();
        
            // Draw border around this Component...
        g.setColor(Color.darkGray);
        g.drawLine(d.width, 0, d.width, d.height); 
        g.drawLine(d.width, d.height, 0, d.height); 
        
        g.setColor(Color.lightGray);
        g.drawLine(0, 0, 0, d.height);
        g.drawLine(0, 0, d.width, 0);
    } // paint


        // SourceWatcher implementation...

    public void beginInterpretation()
    {
        inputArea.setEditable(false);
    } // beginInterpretation


    public void endInterpretation()
    {
        inputArea.setEditable(true);
    } // endInterpretation


    public void sourceUpdated(int lineNum)
    {
        TurtleSpace tspace = ((TobyPanel) getParent()).getTurtleSpace();

        if (tspace.isTraceEnabled())
            highlightLine(lineNum);
    } // sourceUpdated


    public void sourceError(int errLine)
    {
        highlightLine(errLine);
    } // sourceError


        // LayoutManager implementation...

    public void layoutContainer(Container target)
    {
        Dimension d = target.getSize();

        inputArea.setSize((int) (d.width * 0.90), (int) (d.height * 0.90));
        inputArea.setLocation((int) (d.width * 0.05), (int) (d.height * 0.05));
    } // layoutContainer


    public Dimension preferredLayoutSize(Container target)
    {
        Dimension d = target.getParent().getSize();
        Insets insets = target.getParent().getInsets();

        d.width  -= (insets.left + insets.right);
        d.height -= (insets.top + insets.bottom);
        return(d);
    } // preferredLayoutSize


    public Dimension minimumLayoutSize(Container target)
    {
        return(preferredLayoutSize(target));
    } // minimumLayoutSize

    public void addLayoutComponent(String name, Component comp) {}
    public void removeLayoutComponent(Component comp) {}
} // TobyInputArea

// end of TobyInputArea.java ...

