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

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import last.toby.util.*;
import last.toby.parsers.*;
import last.toby.interpreter.*;
import last.toby.exceptions.*;
import last.toby.turtlespace.*;
import last.toby.turtlespace.swing.*;
import java.net.*;


    /**
     * A class to keep all the code involving the Toby frame's menu bar in
     *  one place.
     *
     *    @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public final class TobyMenuBar extends JMenuBar implements ActionListener,
                                                           ItemListener,
                                                           SourceWatcher,
                                                           Runnable
{
    protected TobyFrame parent;
    protected GlobalLogicContext globalContext = null;
    protected boolean startWithStep = false;

    public JMenu fileMenu;
    public JMenuItem newItem;
    public JMenuItem openItem;
    public JMenuItem saveItem;
    public JMenuItem saveAsItem;
    public JMenuItem printItem;
    public JMenuItem quitItem;

    public JMenu helpMenu;
    public JMenuItem aboutItem;
    public JMenuItem helpItem;

    public JMenu runMenu;
    public JMenuItem startStopItem;
    public JMenuItem clearItem;

    public JMenu debugMenu;
    public JMenuItem stepItem;
    public JCheckBoxMenuItem traceItem;
    public JCheckBoxMenuItem watchVarsItem;

    private JMenuItem setupMenuItem(JMenu m, String itemStr,
                                    boolean isEnabled, KeyStroke keys)
    {
        JMenuItem mi = new JMenuItem(itemStr);

        mi.setEnabled(isEnabled);
        mi.addActionListener(this);
        mi.setAccelerator(keys);
        m.add(mi);
        return(mi);
    } // setupMenuItem


    private JCheckBoxMenuItem setupCheckBoxMenuItem(JMenu m,
                                                   String itemStr,
                                                   boolean selected,
                                                   boolean isEnabled,
                                                   KeyStroke keys)
    {
        JCheckBoxMenuItem cbmi = new JCheckBoxMenuItem(itemStr, selected);

        cbmi.setEnabled(isEnabled);
        cbmi.addItemListener(this);
        cbmi.setAccelerator(keys);
        m.add(cbmi);
        return(cbmi);
    } // setupCheckBoxMenuItem


    private JMenu setupMenu(String menuTitle, boolean isEnabled, KeyStroke keys)
    {
        JMenu retVal = new JMenu(menuTitle, isEnabled);
        //retVal.setAccelerator(keys);
        return(retVal);
    } // setupMenu


        /**
         *  A good argument for preprocessor macros: This function just wraps
         *   KeyStroke.getKeyStroke(), so my code can be more readable...
         */
    private KeyStroke key(int keyCode, int modifiers)
    {
        return(KeyStroke.getKeyStroke(keyCode, modifiers, false));
    } // key


    public TobyMenuBar(TobyFrame parentFrame)
    {
        parent = parentFrame;

        fileMenu   = setupMenu(TobyLanguage.MENUNAME_FILE, true,
                                   key(KeyEvent.VK_F, Event.ALT_MASK));
        newItem    = setupMenuItem(fileMenu, TobyLanguage.MENUITEM_NEW, true,
                                   key(KeyEvent.VK_N, Event.CTRL_MASK));
        openItem   = setupMenuItem(fileMenu, TobyLanguage.MENUITEM_OPEN, true,
                                   key(KeyEvent.VK_O, Event.CTRL_MASK));
        saveItem   = setupMenuItem(fileMenu, TobyLanguage.MENUITEM_SAVE, false,
                                   key(KeyEvent.VK_S, Event.CTRL_MASK));
        saveAsItem = setupMenuItem(fileMenu, TobyLanguage.MENUITEM_SAVEAS, true,
                                   key(KeyEvent.VK_A, Event.CTRL_MASK));
        printItem = setupMenuItem(fileMenu, TobyLanguage.MENUITEM_PRINT, true,
                                   key(KeyEvent.VK_P, Event.CTRL_MASK));
        fileMenu.addSeparator();
        quitItem   = setupMenuItem(fileMenu, TobyLanguage.MENUITEM_QUIT, true,
                                   key(KeyEvent.VK_Q, Event.CTRL_MASK));
        add(fileMenu);


        runMenu    = setupMenu(TobyLanguage.MENUNAME_RUN, true,
                                   key(KeyEvent.VK_R, Event.ALT_MASK));
        startStopItem = setupMenuItem(runMenu, TobyLanguage.MENUITEM_STARTCODE,
                                   true, key(KeyEvent.VK_F5, 0));
        clearItem  = setupMenuItem(runMenu, TobyLanguage.MENUITEM_CLEAR, true,
                                   key(KeyEvent.VK_C, Event.CTRL_MASK));
        add(runMenu);


        debugMenu = setupMenu(TobyLanguage.MENUNAME_DEBUG, true,
                                   key(KeyEvent.VK_D, Event.ALT_MASK));

        stepItem  = setupMenuItem(debugMenu, TobyLanguage.MENUITEM_STEP, true,
                                   key(KeyEvent.VK_F8, 0));

        traceItem = setupCheckBoxMenuItem(debugMenu,
                                          TobyLanguage.MENUITEM_TRACE,
                                          false, true,
                                          key(KeyEvent.VK_T, Event.CTRL_MASK));

        watchVarsItem = setupCheckBoxMenuItem(debugMenu,
                                   TobyLanguage.MENUITEM_WATCHVARS,
                                   false, true,
                                   key(KeyEvent.VK_W, Event.CTRL_MASK));

        add(debugMenu);


        helpMenu   = setupMenu(TobyLanguage.MENUNAME_HELP, true,
                                   key(KeyEvent.VK_H, Event.ALT_MASK));
        aboutItem  = setupMenuItem(helpMenu, TobyLanguage.MENUITEM_ABOUT, true,
                                   key(KeyEvent.VK_B, Event.CTRL_MASK));
        helpItem   = setupMenuItem(helpMenu, TobyLanguage.MENUITEM_HELP, true,
                                   key(KeyEvent.VK_H, Event.CTRL_MASK));
        add(helpMenu);
    } // Constructor


    public void setSaveEnabled(boolean savable)
    {
        saveItem.setEnabled(savable);
    } // setSaveable


    private void doStep()
    {
        TobyInputArea tia = parent.getTobyPanel().getInputArea();
        if (tia != null)
            tia.setTracing(true);

        if (this.globalContext != null)
        {
            this.globalContext.pauseExecution();
            this.globalContext.triggerStep();
        } // if
        else
        {
            this.startWithStep = true;
            new Thread(this).start();
        } // else
    } // doStep


    private void setupTrace(JCheckBoxMenuItem cbmi)
    {
        boolean val = cbmi.getState();
        if (this.globalContext != null)
            this.globalContext.setTracing(val);

        TobyInputArea tia = parent.getTobyPanel().getInputArea();
        if (tia != null)
            tia.setTracing(val);
    } // setupTrace


    private void setupWatchVars(JCheckBoxMenuItem cbmi)
    {
        VarWatcher vw = parent.getTobyPanel().getVarWatcher();

        if (this.globalContext == null)
            return;

        if (cbmi.getState())              // true == enable variable watching.
            this.globalContext.addVarWatcher(vw);
        else
            this.globalContext.removeVarWatcher(vw);
    } // setupWatchVars


        // ActionListener implementation ...

    public void actionPerformed(ActionEvent e)
    {
        JMenuItem mi = (JMenuItem) e.getSource();

        if (mi == newItem)
            parent.newFile();

        else if (mi == openItem)
            parent.openFile(null);

        else if (mi == saveItem)
            parent.saveFile(parent.getCurrentFileName());

        else if (mi == saveAsItem)
            parent.saveFile(null);

        else if (mi == quitItem)
            parent.requestToQuit();

        else if (mi == helpItem)
            parent.displayHelp();
/*
// !!!
        {
//            try
//            {
//                URL url = new URL("file:/home/icculus/zen.jpg");
                String url = "/home/icculus/zen.jpg";
                Toolkit tk = Toolkit.getDefaultToolkit();
                Image img = tk.createImage(url);
                JTurtleSpace tspace = (JTurtleSpace) parent.getTobyPanel().getTurtleSpace();
                tspace.setBackground(img, tspace.MODE_SCALED);
//            } // try
//            catch (MalformedURLException exc)
//            {
//                JOptionPane.showMessageDialog(null, "Bad URL.",
//                                              TobyLanguage.ERROR,
//                                              JOptionPane.ERROR_MESSAGE);
//            } // catch

        } // else if
*/

        else if (mi == aboutItem)
            parent.createAboutBox();

        else if (mi == printItem)
            parent.doPrint();

        else if (mi == startStopItem)
        {
            if (this.globalContext != null)
                this.globalContext.endExecution();
            else
            {
                this.startWithStep = false;
                new Thread(this).start();
            } // else
        } // else if

        else if (mi == stepItem)
            doStep();

        else if (mi == clearItem)
            parent.getTobyPanel().getTurtleSpace().cleanup();
    } // actionPerformed


        // ItemListener implementation...

    public void itemStateChanged(ItemEvent e)
    {
        JCheckBoxMenuItem cbmi = (JCheckBoxMenuItem) e.getSource();

        if (cbmi == watchVarsItem)
            setupWatchVars(cbmi);

        else if (cbmi == traceItem)
            setupTrace(cbmi);
    } // itemStateChanged


        // SourceWatcher implementation...

    public void beginInterpretation()
    {
        startStopItem.setText(TobyLanguage.MENUITEM_STOPCODE);
        startStopItem.setAccelerator(key(KeyEvent.VK_ESCAPE, 0));
        newItem.setEnabled(false);
        openItem.setEnabled(false);
        clearItem.setEnabled(false);
    } // beginInterpretation


    public void endInterpretation(boolean normalTermination)
    {
        startStopItem.setText(TobyLanguage.MENUITEM_STARTCODE);
        startStopItem.setAccelerator(key(KeyEvent.VK_F5, 0));
        newItem.setEnabled(true);
        openItem.setEnabled(true);
        clearItem.setEnabled(true);
    } // endInterpretation


    public void sourceNewLine(int newSourceLine) {}
    public void sourceError(ExecException exc) {}

        // !!! Moving this to another class (ExecutionThread?) might be good.
    public void run()
    {
        TurtleSpace tspace = parent.getTobyPanel().getTurtleSpace();
        TobyInputArea tia = parent.getTobyPanel().getInputArea();
        try
        {
            tspace.grabTurtleSpace();
            tspace.removeAllTurtles();
            tspace.addTurtle();
            tspace.useTurtle(0);
            tspace.defaultAllTurtles();
            tspace.cleanup();
            tspace.enableFence();

                // !!! TobyParser?
            Parser p = new last.toby.parsers.toby.TobyParser();
            String str = tia.getSource();
            StringReader strReader = new StringReader(str);
            this.globalContext = p.parseCode(strReader, tspace);

            this.globalContext.resetState();

            this.globalContext.addSourceWatcher(this);
            this.globalContext.addSourceWatcher(tia);

            setupTrace(this.traceItem);
            setupWatchVars(this.watchVarsItem);

            if (this.startWithStep == false)
                this.globalContext.execute();
            else
            {
                this.globalContext.pauseExecution();
                this.globalContext.execute();
                this.globalContext.triggerStep();
            } // else

            long parseTime = p.getTotalParseTime();
            long linkTime = p.getTotalLinkTime();
            long execTime = this.globalContext.getTotalExecTime();
            long totalTime = execTime + parseTime + linkTime;
            System.err.println();
            System.err.println("Parse time : " + parseTime + " milliseconds.");
            System.err.println("Link  time : " + linkTime + " milliseconds.");
            System.err.println("Exec. time : " + execTime + " milliseconds.");
            System.err.println("Total time : " + totalTime + " milliseconds.");
            System.err.println();
        } // try
        catch (FlowException exc)
        {
            String errMsg = "Error: " + exc.getMessage() +
                            " on line " + exc.exceptionLine +
                            " in function " + exc.procName;
            JOptionPane.showMessageDialog(null, errMsg,
                                          TobyLanguage.ERROR,
                                          JOptionPane.ERROR_MESSAGE);


                // !!! temp
            exc.printStackTrace();
        } // catch
        catch (Exception exc)
        {

                // !!! tobylanguage?
            String errMsg = exc.getClass().getName() + ":" + exc.getMessage();
            errMsg += "\n\n";
            errMsg += "This should not happen. Email icculus@lokigames.com!";
            JOptionPane.showMessageDialog(null, errMsg,
                                          TobyLanguage.ERROR,
                                          JOptionPane.ERROR_MESSAGE);
            exc.printStackTrace();
        } // catch

        tspace.releaseTurtleSpace();
        this.globalContext = null;
    } // run

} // TobyMenuBar

// end of TobyMenuBar.java ...

