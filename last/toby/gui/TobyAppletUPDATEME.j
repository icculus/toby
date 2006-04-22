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
import java.awt.event.*;
import java.net.*;
import java.io.*;
import javax.swing.*;
import last.toby.interpreter.SourceWatcher;


    /**
     *   The Applet version of Toby.
     *
     *    @author Ryan C. Gordon. (icculus@icculus.org)
     */
public class TobyApplet extends JApplet implements ActionListener, Runnable,
                                                   SourceWatcher
{
    public static final String PARAMNAME_URL = "URL";
    public static final String PARAMNAME_SOURCE = "SOURCE";
    public static final String DOWNLOAD_FAILED = "Source code download FAILED.";
    public static final String MSG_CLICKTOSTART = "Click here to run the program.";
    public static final String MSG_CLICKTOSTOP = "Click here to stop the program.";
    public static final String MSG_BAD_JAVA =
           "You're Java support is outdated; goto http://java.sun.com/";

    protected TurtleSpace tspace;
    protected JButton runButton;
    protected String sourceCode;
    protected boolean noSourceCode = true;
    protected boolean interruptRead = false;


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
        Container rootPane = getContentPane();
        tspace = new TobyInterpreter();
        runButton = new JButton(MSG_CLICKTOSTART);

        tspace.addSourceWatcher(this);

        runButton.setEnabled(false);
        runButton.addActionListener(this);

        rootPane.setLayout(new BorderLayout());
        rootPane.add("Center", tspace);
        rootPane.add("South", runButton);
    } // buildAppletComponents


    private void setSourceCode(String src)
    {
        sourceCode = src;
        noSourceCode = false;
        runButton.setEnabled(true);
    } // setSourceCode


    private void grabSourceCode()
    {
        showStatus("Downloading Toby's instructions...please wait...");

        if (getParameter(PARAMNAME_URL) != null)
            grabNetCode();
        else
            grabParamCode();
    } // grabSourceCode


    public void stop()
    {
        interruptRead = true;
    } // stop


    private void grabNetCode()
    /*
     *  Due to blocking issues, net reading is done is a separate thread.
     *   When finished, the thread terminates itself, but can be commanded
     *   to terminate prematurely by setting (interruptRead) to (true).
     */
    {
        new Thread(this).start();
    } // grabNetCode


    private void grabParamCode()
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

        setSourceCode(srcBuffer.toString());
    } // grabParamCode


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
            JRootPane rp = this.getRootPane();
            rp.putClientProperty("defeatSystemEventQueueCheck", Boolean.TRUE);
 
            buildAppletComponents();
        } // else
    } // init


    public void start()
    {
        if (noSourceCode)
            grabSourceCode();
    } // start


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

        sb.append("Copyright (C) 1999 Ryan C. Gordon.\r\n");
        sb.append(" Written by Ryan C. Gordon.\r\n");

        return(sb.toString());
    } // getAppletInfo


        // ActionListener implementation...

    public void actionPerformed(ActionEvent e)
    {
        // Must be the "Run" button...

        if (tspace.isCodeRunning() == true)
            tspace.haltInterpreter();
        else
            kickOffCodeRun();
    } // actionPerformed


        // Runnable implementation...

    public void run()
    /**
     * Net code is downloaded here.
     *
     */
    {
        BufferedReader br;
        String endl = System.getProperty("line.separator");
        String inLine;
        StringBuffer sb = null;
        URL url;

        interruptRead = false;

        try
        {
            url = new URL(getParameter(PARAMNAME_URL));
            br = new BufferedReader(new InputStreamReader(url.openStream()));
            sb = new StringBuffer();

            while ((!interruptRead) && ((inLine = br.readLine()) != null))
            {
                sb.append(inLine);
                sb.append(endl);
            } // while

            sb.append(endl);
            br.close();
        } // try

        catch (Exception e)
        {
            interruptRead = true;
        } // catch

        if (interruptRead)
            showStatus(DOWNLOAD_FAILED);
        else
            setSourceCode(sb.toString());
    } // run


        // SourceWatcher implementation ...

    public void beginInterpretation()
    {
        runButton.setText(MSG_CLICKTOSTOP);
    } // beginInterpretation


    public void endInterpretation(boolean normalTermination)
    {
        runButton.setText(MSG_CLICKTOSTART);
    } // endInterpretation

    public void sourceUpdated(int newSourceLine) {}
    public void sourceError(int errLine) {}

} // TobyApplet

// end of TobyApplet.java ...

