/**
 * A class to keep all the code involving the Toby frame's menu bar in
 *  one place.
 *
 *   Copyright (c) Lighting and Sound Technologies, 1997.
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
                                   int keyCode, boolean isEnabled)
    {
        JMenuItem mi = new JMenuItem(itemStr);

        mi.setEnabled(isEnabled);
        mi.addActionListener(this);
        // !!! need to covert to Swing's setAccelerator()...
        //mi.setShortcut(new MenuShortcut(keyCode));
        m.add(mi);
        return(mi);
    } // setupMenuItem


    private JCheckBoxMenuItem setupCheckBoxMenuItem(JMenu m,
                                                   String itemStr,
                                                   int keyCode,
                                                   boolean selected,
                                                   boolean isEnabled)
    {
        JCheckBoxMenuItem cbmi = new JCheckBoxMenuItem(itemStr, selected);

        cbmi.setEnabled(isEnabled);
        cbmi.addItemListener(this);
        // !!! need to covert to Swing's setAccelerator()...
        //cbmi.setShortcut(new MenuShortcut(keyCode));
        m.add(cbmi);
        return(cbmi);
    } // setupCheckBoxMenuItem


    private JMenu setupMenu(String menuTitle, boolean isEnabled, int keyCode)
    {
        JMenu retVal = new JMenu(menuTitle, isEnabled);
        // !!! need to covert to Swing's setAccelerator()...
        //retVal.setShortcut(new JMenuShortcut(keyCode));
        return(retVal);
    } // setupMenu


    public TobyMenuBar(Toby parentFrame)
    {
        parent = parentFrame;

        fileMenu   = setupMenu(MENUNAME_FILE, true, KeyEvent.VK_M);
        newItem    = setupMenuItem(fileMenu, MENUITEM_NEW, KeyEvent.VK_N, true);
        openItem   = setupMenuItem(fileMenu, MENUITEM_OPEN, KeyEvent.VK_O, true);
        saveItem   = setupMenuItem(fileMenu, MENUITEM_SAVE, KeyEvent.VK_S, true);   // change when we can figure
        saveAsItem = setupMenuItem(fileMenu, MENUITEM_SAVEAS, KeyEvent.VK_A, true); // out if text is modified. !!!
        fileMenu.addSeparator();
        quitItem   = setupMenuItem(fileMenu, MENUITEM_QUIT, KeyEvent.VK_X, true);
        add(fileMenu);

        runMenu    = setupMenu(MENUNAME_RUN, true, KeyEvent.VK_R);
        startStopItem = setupMenuItem(runMenu, MENUITEM_STARTCODE,
                                      KeyEvent.VK_G, true);
        clearItem  = setupMenuItem(runMenu, MENUITEM_CLEAR, KeyEvent.VK_C, true);
        add(runMenu);

        debugMenu = setupMenu(MENUNAME_DEBUG, true, KeyEvent.VK_D);
        stepItem  = setupMenuItem(debugMenu, MENUITEM_STEP, KeyEvent.VK_W, true);
        traceItem = setupCheckBoxMenuItem(debugMenu, MENUITEM_TRACE, KeyEvent.VK_T,
                                          false, true);
        watchVarsItem = setupCheckBoxMenuItem(debugMenu, MENUITEM_WATCHVARS,
                                              KeyEvent.VK_V, false, true);
        add(debugMenu);

        helpMenu   = setupMenu(MENUNAME_HELP, true, KeyEvent.VK_H);
        aboutItem  = setupMenuItem(helpMenu, MENUITEM_ABOUT, KeyEvent.VK_B, true);
        helpItem   = setupMenuItem(helpMenu, MENUITEM_HELP, KeyEvent.VK_H, true);
        add(helpMenu);
    } // Constructor


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
            parent.dispose();

        else if (mi == helpItem)
            parent.displayHelp();

        else if (mi == aboutItem)
            parent.createAboutBox();

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
        startStopItem.setLabel(MENUITEM_STOPCODE);
        newItem.setEnabled(false);
        openItem.setEnabled(false);
        clearItem.setEnabled(false);
    } // beginInterpretation

    public void endInterpretation()
    {
        codeRunning = false;
        startStopItem.setLabel(MENUITEM_STARTCODE);
        newItem.setEnabled(true);
        openItem.setEnabled(true);
        clearItem.setEnabled(true);
    } // endInterpretation

    public void sourceUpdated(int newSourceLine) {}
    public void sourceError(int errLine) {}

} // TobyMenuBar

// end of TobyMenuBar.java ...

