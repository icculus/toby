/**
 *   The Applet version of Toby.
 *
 *    Copyright (c) Lighting and Sound Technologies, 1997.
 *     Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.awt.event.*;
import java.applet.Applet;


public class TobyApplet extends Applet implements ActionListener
{
    public static final String PARAMNAME_SOURCE = "SOURCE";
    public static final String MSG_BAD_JAVA =
           "You're Java support is outdated; goto http://java.sun.com/";

    protected TurtleSpace tspace;
    protected Button againButton;
    protected String sourceCode;


    public static boolean badJavaVersion()
    /**
     *  Check if we are running a Java VM that is at least version 1.1,
     *   since much of this code is Java 1.1-specific.
     *
     *      params : void.
     *     returns : true if VM is 1.1 compatible, false otherwise.
     */
    {
        String verStr = System.getProperty("java.version");

        if (verStr.compareTo("1.1") < 0)        // Earlier than Java 1.1?
            return(true);
        else
            return(false);
    } // badJavaVersion


    private void buildAppletComponents()
    {
        tspace = new TobyInterpreter();
        againButton = new Button("Run from start.");

        againButton.addActionListener(this);

        setLayout(new BorderLayout());
        add("Center", tspace);
        add("South", againButton);
    } // buildAppletComponents


    private void grabSourceCode()
    {
        StringBuffer srcBuffer = new StringBuffer();
        String endl = System.getProperty("line.separator");
        String paramName;
        String paramValue;
        int lineCount = 0;

        do
        {
            lineCount++;
            paramName = PARAMNAME_SOURCE + lineCount;
            paramValue = getParameter(paramName);
            if (paramValue != null)
            {
                srcBuffer.append(paramValue);
                srcBuffer.append(endl);
            } // if
        } while (paramValue != null);

        srcBuffer.append(endl);
        sourceCode = srcBuffer.toString();
    } // grabSourceCode


    private void kickOffCodeRun()
    {
        StringBuffer sb;

        try
        {
            tspace.setCode(sourceCode);
        } // try
        catch (TobyParseException tpe)
        {
            sb = new StringBuffer();

            if (tpe.procName != null)
            {
                sb.append("Error in TOBY function \"");
                sb.append(tpe.procName);
                sb.append("\" ");

                if (tpe.errLine != TobyParseException.LINENUM_UNDEFINED)
                {
                    sb.append("(");
                    sb.append(tpe.errLine);
                    sb.append(") ");
                } // if

                sb.append(": ");
            } // if

            sb.append(tpe.getMessage());
            showStatus(sb.toString());
        } // catch
    } // kickOffCodeRun


    public void init()
    /**
     *  Applets start their life cycle here. If we are using a
     *   Java 1.1-capable applet viewer, then this collects applet
     *   parameters (the TOBY source code), and builds a TurtleSpace
     *   object, which handles the rest of the program. An "AGAIN!" button
     *   is also produced, so users may re-run the code.
     *
     *       params : void.
     *      returns : void.
     */
    {
        if (badJavaVersion())
            showStatus(MSG_BAD_JAVA);
        else
        {
            grabSourceCode();
            buildAppletComponents();
            kickOffCodeRun();
        } // else
    } // init


    public String getAppletInfo()
    /**
     *  Overrides default java.applet.Applet method.
     *
     *    params : void.
     *   returns : basic applet info in a text string.
     */
    {
        StringBuffer sb = new StringBuffer();

        sb.append(Toby.TITLE);
        sb.append(" ");
        sb.append(Toby.VERSION);
        sb.append("\r\n");

        sb.append("Copyright (c) Lighting and Sound Technologies, 1997.\r\n");
        sb.append(" Written by Ryan C. Gordon.\r\n");

        return(sb.toString());
    } // getAppletInfo


        // ActionListener implementation...

    public void actionPerformed(ActionEvent e)
    {
        // Must be the "Again" button...

        kickOffCodeRun();
    } // actionPerformed

} // TobyApplet

// end of TobyApplet.java ...

