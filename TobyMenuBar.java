/**
 * A class to keep all the code involving the Toby frame's menu bar in
 *  one place.
 *
 *   Copyright (c) Lighting and Sound Technologies, 1997.
 *    Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.awt.event.*;

public final class TobyMenuBar extends MenuBar implements ActionListener,
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

    public Menu fileMenu;
    public MenuItem newItem;
    public MenuItem openItem;
    public MenuItem saveItem;
    public MenuItem saveAsItem;
    public MenuItem quitItem;

    public Menu helpMenu;
    public MenuItem aboutItem;
    public MenuItem helpItem;

    public Menu runMenu;
    public MenuItem startItem;
    public MenuItem stopItem;
    public MenuItem clearItem;

    public Menu debugMenu;
    public MenuItem stepItem;
    public MenuItem traceItem;
    public CheckboxMenuItem watchVarsItem;


    private MenuItem setupMenuItem(Menu m, String itemStr,
                                   int keyCode, boolean isEnabled)
    {
        MenuItem mi = new MenuItem(itemStr);

        mi.setEnabled(isEnabled);
        mi.addActionListener(this);
        mi.setShortcut(new MenuShortcut(keyCode));
        m.add(mi);
        return(mi);
    } // setupMenuItem


    private CheckboxMenuItem setupCheckboxMenuItem(Menu m,
                                                   String itemStr,
                                                   int keyCode,
                                                   boolean selected,
                                                   boolean isEnabled)
    {
        CheckboxMenuItem cbmi = new CheckboxMenuItem(itemStr, selected);

        cbmi.setEnabled(isEnabled);
        cbmi.addItemListener(this);
        cbmi.setShortcut(new MenuShortcut(keyCode));
        m.add(cbmi);
        return(cbmi);
    } // setupCheckboxMenuItem


    private Menu setupMenu(String menuTitle, boolean isEnabled, int keyCode)
    {
        Menu retVal = new Menu(menuTitle, isEnabled);
        retVal.setShortcut(new MenuShortcut(keyCode));
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
        startItem  = setupMenuItem(runMenu, MENUITEM_STARTCODE, KeyEvent.VK_F5, true);
        stopItem   = setupMenuItem(runMenu, MENUITEM_STOPCODE, KeyEvent.VK_P, false);
        clearItem  = setupMenuItem(runMenu, MENUITEM_CLEAR, KeyEvent.VK_C, true);
        add(runMenu);

        debugMenu = setupMenu(MENUNAME_DEBUG, true, KeyEvent.VK_D);
        stepItem  = setupMenuItem(debugMenu, MENUITEM_STEP, KeyEvent.VK_S, true);
        traceItem = setupCheckboxMenuItem(debugMenu, MENUITEM_TRACE, KeyEvent.VK_T,
                                          false, true);
        watchVarsItem = setupCheckboxMenuItem(debugMenu, MENUITEM_WATCHVARS,
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
                TobyInterpreter.displayParseException(tpe, parent);
            } // catch
        } // if
        else
            tspace.stepInterpreter();
    } // doStep


    private void setupTrace(CheckboxMenuItem cbmi)
    {
        parent.getTobyPanel().getTurtleSpace().enableTracing(cbmi.getState());
    } // setupTrace


    private void setupWatchVars(CheckboxMenuItem cbmi)
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
        MenuItem mi = (MenuItem) e.getSource();

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

        else if (mi == startItem)
            parent.getTobyPanel().getInputArea().runCode();

        else if (mi == stopItem)
            parent.getTobyPanel().getTurtleSpace().haltInterpreter();

        else if (mi == stepItem)
            doStep();

        else if (mi == clearItem)
            parent.getTobyPanel().getTurtleSpace().cleanup();
    } // actionPerformed


        // ItemListener implementation...

    public void itemStateChanged(ItemEvent e)
    {
        CheckboxMenuItem cbmi = (CheckboxMenuItem) e.getSource();

        if (cbmi == watchVarsItem)
            setupWatchVars(cbmi);

        else if (cbmi == traceItem)
            setupTrace(cbmi);
    } // itemStateChanged


        // SourceWatcher implementation...

    public void beginInterpretation()
    {
        newItem.setEnabled(false);
        openItem.setEnabled(false);
        startItem.setEnabled(false);
        clearItem.setEnabled(false);
        stopItem.setEnabled(true);
    } // beginInterpretation

    public void endInterpretation()
    {
        newItem.setEnabled(true);
        openItem.setEnabled(true);
        stopItem.setEnabled(false);
        startItem.setEnabled(true);
        clearItem.setEnabled(true);
    } // endInterpretation

    public void sourceUpdated(int newSourceLine) {}
    public void sourceError(int errLine) {}

} // TobyMenuBar

// end of TobyMenuBar.java ...

