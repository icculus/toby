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
 * A class to keep all the code involving the Toby frame's menu bar in
 *  one place.
 *
 *    Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public final class TobyMenuBar extends JMenuBar implements ActionListener,
                                                           ItemListener,
                                                           SourceWatcher
{
    public static final String MENUNAME_FILE   = "File";
    public static final String MENUITEM_NEW    = "New";
    public static final String MENUITEM_OPEN   = "Open...";
    public static final String MENUITEM_SAVE   = "Save";
    public static final String MENUITEM_SAVEAS = "Save as...";
    public static final String MENUITEM_PRINT  = "Print...";
    public static final String MENUITEM_QUIT   = "Quit";

    public static final String MENUNAME_HELP  = "Help";
    public static final String MENUITEM_HELP  = "Help...";
    public static final String MENUITEM_ABOUT = "About...";

    public static final String MENUNAME_RUN       = "Run";
    public static final String MENUITEM_STARTCODE = "Start program";
    public static final String MENUITEM_STOPCODE  = "Stop program";
    public static final String MENUITEM_CLEAR     = "Cleanup TurtleSpace";

    public static final String MENUNAME_DEBUG     = "Debug";
    public static final String MENUITEM_TRACE     = "Trace";
    public static final String MENUITEM_STEP      = "Step";
    public static final String MENUITEM_WATCHVARS = "Watch variables";

    private Toby parent;
    private boolean codeRunning = false;

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
    public JMenuItem traceItem;
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


    private KeyStroke key(int keyCode, int modifiers)
    /*
     *  A good argument for preprocessor macros: This function just wraps
     *   KeyStroke.getKeyStroke(), so my code can be more readable...
     */
    {
        return(KeyStroke.getKeyStroke(keyCode, modifiers, false));
    } // key


    public TobyMenuBar(Toby parentFrame)
    {
        parent = parentFrame;

        fileMenu   = setupMenu(MENUNAME_FILE, true,
                                   key(KeyEvent.VK_F, Event.ALT_MASK));
        newItem    = setupMenuItem(fileMenu, MENUITEM_NEW, true,
                                   key(KeyEvent.VK_N, Event.CTRL_MASK));
        openItem   = setupMenuItem(fileMenu, MENUITEM_OPEN, true,
                                   key(KeyEvent.VK_O, Event.CTRL_MASK));
        saveItem   = setupMenuItem(fileMenu, MENUITEM_SAVE, false,
                                   key(KeyEvent.VK_S, Event.CTRL_MASK));
        saveAsItem = setupMenuItem(fileMenu, MENUITEM_SAVEAS, true,
                                   key(KeyEvent.VK_A, Event.CTRL_MASK));
        printItem = setupMenuItem(fileMenu, MENUITEM_PRINT, true,
                                   key(KeyEvent.VK_P, Event.CTRL_MASK));
        fileMenu.addSeparator();
        quitItem   = setupMenuItem(fileMenu, MENUITEM_QUIT, true,
                                   key(KeyEvent.VK_Q, Event.CTRL_MASK));
        add(fileMenu);


        runMenu    = setupMenu(MENUNAME_RUN, true,
                                   key(KeyEvent.VK_R, Event.ALT_MASK));
        startStopItem = setupMenuItem(runMenu, MENUITEM_STARTCODE, true,
                                   key(KeyEvent.VK_F5, 0));
        clearItem  = setupMenuItem(runMenu, MENUITEM_CLEAR, true,
                                   key(KeyEvent.VK_C, Event.CTRL_MASK));
        add(runMenu);


        debugMenu = setupMenu(MENUNAME_DEBUG, true,
                                   key(KeyEvent.VK_D, Event.ALT_MASK));

        stepItem  = setupMenuItem(debugMenu, MENUITEM_STEP, true,
                                   key(KeyEvent.VK_F8, 0));

        traceItem = setupCheckBoxMenuItem(debugMenu, MENUITEM_TRACE,
                                          false, true,
                                   key(KeyEvent.VK_T, Event.CTRL_MASK));

        watchVarsItem = setupCheckBoxMenuItem(debugMenu, MENUITEM_WATCHVARS,
                                              false, true,
                                   key(KeyEvent.VK_W, Event.CTRL_MASK));

        add(debugMenu);


        helpMenu   = setupMenu(MENUNAME_HELP, true,
                                   key(KeyEvent.VK_H, Event.ALT_MASK));
        aboutItem  = setupMenuItem(helpMenu, MENUITEM_ABOUT, true,
                                   key(KeyEvent.VK_B, Event.CTRL_MASK));
        helpItem   = setupMenuItem(helpMenu, MENUITEM_HELP, true,
                                   key(KeyEvent.VK_H, Event.CTRL_MASK));
        add(helpMenu);
    } // Constructor


    public void setSaveEnabled(boolean savable)
    {
        saveItem.setEnabled(savable);
    } // setSaveable


    private void doStep()
    {
        TurtleSpace tspace = parent.getTobyPanel().getTurtleSpace();
        String src;

        if (tspace.isCodeRunning() == false)
        {
            src = parent.getTobyPanel().getInputArea().getSource();
            try
            {
                tspace.setCodeWithStep(src);
            } // try
            catch (TobyParseException tpe)
            {
                TobyInterpreter.displayParseException(tpe);
            } // catch
        } // if
        else
            tspace.stepInterpreter();
    } // doStep


    private void setupTrace(JCheckBoxMenuItem cbmi)
    {
        parent.getTobyPanel().getTurtleSpace().enableTracing(cbmi.getState());
    } // setupTrace


    private void setupWatchVars(JCheckBoxMenuItem cbmi)
    {
        TurtleSpace tspace = parent.getTobyPanel().getTurtleSpace();
        VarWatcher vw = parent.getTobyPanel().getVarWatcher();

        if (cbmi.getState())              // true == enable variable watching.
            tspace.addVarWatcher(vw);
        else
            tspace.removeVarWatcher(vw);
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

        else if (mi == aboutItem)
            parent.createAboutBox();

        else if (mi == printItem)
            parent.doPrint();

        else if (mi == startStopItem)
        {
            if (codeRunning == false)
                parent.getTobyPanel().getInputArea().runCode();
            else
                parent.getTobyPanel().getTurtleSpace().haltInterpreter();
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
        codeRunning = true;
        startStopItem.setText(MENUITEM_STOPCODE);
        startStopItem.setAccelerator(key(KeyEvent.VK_ESCAPE, 0));
        newItem.setEnabled(false);
        openItem.setEnabled(false);
        clearItem.setEnabled(false);
    } // beginInterpretation


    public void endInterpretation(boolean normalTermination)
    {
        codeRunning = false;
        startStopItem.setText(MENUITEM_STARTCODE);
        startStopItem.setAccelerator(key(KeyEvent.VK_F5, 0));
        newItem.setEnabled(true);
        openItem.setEnabled(true);
        clearItem.setEnabled(true);
    } // endInterpretation


    public void sourceUpdated(int newSourceLine) {}
    public void sourceError(int errLine) {}

} // TobyMenuBar

// end of TobyMenuBar.java ...

