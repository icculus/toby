/**
 *  This class is used to kick off the application, and contains the
 *   encompassing Frame component...
 *
 *     Copyright (c) Lighting and Sound Technologies, 1997.
 *      Written by Ryan C. Gordon.
 */

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;

//import java.awt.print.*;


public final class Toby extends JFrame implements DocumentListener,
                                                  WindowListener
{
        // Global Constants...
    public static final String TITLE   = "Toby";
    public static final String VERSION = "v1.0alpha3";

    public static final String USAGE = "USAGE: Toby.class [sourceFile.toby]";

    public static final String WRITTENBY = "Written by Ryan C. Gordon.";
    public static final String COPYRIGHT =
                    "Copyright (c) 1997-1999 Lighting and Sound Technologies.";

    public static final String MSG_ERROR = "Error";

    public static final String MSG_BAD_JAVA =
         "Your Java Virtual Machine is too old; goto http://java.sun.com/ ...";

        // Instance variables...
    private TobyPanel tobyPanel;
    private String tobyFileName = null;
    private File currentDirectory = null;
    private boolean textModified = false;
    private TobyMenuBar tmb = null;

    public Toby()
    {
        super();

        Container rootPane = getContentPane();
        TurtleSpace tspace;
        
        tmb = new TobyMenuBar(this);
        currentDirectory = new File(".");   // !!! is this kosher?
        setTitle();
 
        addWindowListener(this);
        rootPane.setLayout(new BorderLayout());
        setResizable(true);
        setSize(getToolkit().getScreenSize());
        setJMenuBar(tmb);

        tobyPanel = new TobyPanel(this);
        rootPane.add("Center", tobyPanel);

            // !!! Awkward. Move this?
        tspace = tobyPanel.getTurtleSpace();
        tspace.addSourceWatcher(tmb);
        setVisible(true);
        show();
    } // Constructor


    private void setTobyIcon()
    {
        System.out.println("in setTobyIcon.");
        Turtle turtle = new TriangleTurtle();
        Image image = createImage(24, 24);
        Graphics g = image.getGraphics();

        System.out.println("painting...");

        g.setColor(Color.black);
        g.fillRect(0, 0, 24, 24);
        turtle.paint(g, null);
        g.dispose();

        System.out.println("doing it...");

        setIconImage(null);

        System.out.println("done.");
    } // setTobyIcon


    public void doPrint()
    {
        String msg = "Printing not yet implemented.";

        JOptionPane.showMessageDialog(null, msg, TITLE + " " + VERSION,
                                      JOptionPane.INFORMATION_MESSAGE);

//        PrinterJob.printDialog();
//        PrinterJob.pageDialog();
    } // doPrint


    public void setTitle()
    {
        StringBuffer sb = new StringBuffer();

        sb.append(TITLE);
        sb.append(" ");
        sb.append(VERSION);
        sb.append(": ");

        if (tobyFileName != null)
            sb.append(tobyFileName);
        else
            sb.append("new program");

        if (textModified)
            sb.append("    [*]");

        super.setTitle(sb.toString());
        sb.setLength(0);
    } // setTitle


    public TobyPanel getTobyPanel()
    {
        return(tobyPanel);
    } // getTobyPanel


    public String getCurrentFileName()
    {
        return(tobyFileName);
    } // getCurrentFileName


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


    public void displayHelp()
    {
            // !!! JavaHelp, perhaps?
        String msg = "Help not yet implemented.";

        JOptionPane.showMessageDialog(null, msg, TITLE + " " + VERSION,
                                      JOptionPane.INFORMATION_MESSAGE);
    } // displayHelp


    public void createAboutBox()
    {
        String title = "About " + TITLE + " " + VERSION + "...";
        Font font = new Font("SansSerif", Font.BOLD, 16);
        String[] msgText = new String[2];

        msgText[0] = COPYRIGHT;
        msgText[1] = WRITTENBY;

        JOptionPane.showMessageDialog(null, msgText, title,
                                      JOptionPane.INFORMATION_MESSAGE);
    } // createAboutBox


    private String selectFileName(int fileMode)
    {
        JFileChooser chooser = new JFileChooser();
        int rc;
        String retVal = null;

        chooser.setFileFilter(new TobyFilenameFilter());
        chooser.setMultiSelectionEnabled(false);
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileHidingEnabled(false);
        chooser.setCurrentDirectory(currentDirectory);

        if (fileMode == JFileChooser.SAVE_DIALOG)
            rc = chooser.showSaveDialog(this);
        else if (fileMode == JFileChooser.OPEN_DIALOG)
            rc = chooser.showOpenDialog(this);
        else
            throw(new IllegalArgumentException("Open, save?!"));

        if (rc == JFileChooser.APPROVE_OPTION)
        {
            currentDirectory = chooser.getSelectedFile();
            retVal = currentDirectory.getName();
        } // if
        
        return(retVal);
    } // selectFileName


    public void newFile()
    {
        if (saveAndContinue() == true)
        {
            tobyPanel.getInputArea().clearCode();
            tobyFileName = null;
            textModified = false;
            tmb.setSaveEnabled(false);
            setTitle();
        } // if
    } // newFile


    public void openFile(String fileName)
    {
        LineNumberReader br;

        if (saveAndContinue() == true)
        {
            if (fileName == null)
                fileName = selectFileName(JFileChooser.OPEN_DIALOG);

            if (fileName != null)
            {
                try
                {
                    br = new LineNumberReader(new FileReader(fileName));
                    tobyPanel.getInputArea().setSource(br);
                    tobyFileName = fileName;
                    textModified = false;
                    tmb.setSaveEnabled(false);
                    setTitle();
                } // try

                catch (FileNotFoundException fnfe)
                {
                    String errMsg;

                    errMsg = "File [" + fileName + "] not found.";
                    JOptionPane.showMessageDialog(null, errMsg, "Error",
                                          JOptionPane.ERROR_MESSAGE);
                } // catch

                catch (IOException ioe)
                {
                    String errMsg;
                    errMsg = "Cannot load [" + fileName + "].";
                    JOptionPane.showMessageDialog(null, errMsg, "Error",
                                          JOptionPane.ERROR_MESSAGE);
                } // catch
            } // if
        } // if
    } // openFile


    private boolean saveAndContinue()
    {
        boolean retVal = true;
        int rc;

        if (textModified)
        {
            rc = JOptionPane.showConfirmDialog(null,
                                            "Save modified program?", "Whoa!",
                                            JOptionPane.YES_NO_CANCEL_OPTION,
                                            JOptionPane.QUESTION_MESSAGE);

            if (rc == JOptionPane.YES_OPTION)
                retVal = saveFile(tobyFileName);
            else if (rc == JOptionPane.CANCEL_OPTION)
                retVal = false;
        } // if

        return(retVal);
    } // saveAndContinue


    public boolean saveFile(String fileName)
    {
        boolean retVal = false;
        BufferedWriter bw;
        String src;

        if (fileName == null)
        {
            fileName = selectFileName(JFileChooser.SAVE_DIALOG);
        } // if

        if (fileName != null)
        {
            src = tobyPanel.getInputArea().getSource();

            try
            {
                bw = new BufferedWriter(new FileWriter(fileName));
                bw.write(src, 0, src.length());
                bw.close();
                tobyFileName = fileName;
                textModified = false;
                tmb.setSaveEnabled(false);
                setTitle();
                retVal = true;
            } // try
            catch (IOException ioe)
            {
                String errMsg;
                errMsg = "Cannot write to [" + tobyFileName + "]";
                JOptionPane.showMessageDialog(null, errMsg, "Error",
                                      JOptionPane.ERROR_MESSAGE);
            } // catch
        } // if

        return(retVal);
    } // saveFile


    private static void kickOffGUI(String[] args)
    {
        Toby toby;

        try
        {
            if (args.length <= 1)
            {
                toby = new Toby();
                if (args.length == 1)
                    toby.openFile(args[0]);
            } // if
            else if (args.length > 1)
            {
                System.err.println(USAGE);
                System.err.println();
                System.exit(0);
            } // else
        } // try

        catch (NoClassDefFoundError e)
        {
            System.err.println("Cannot find a necessary .class file.");
            System.err.println(" Perhaps you need to set your CLASSPATH?");
            System.err.println(" Also, make sure you have Swing 1.1 or later!");
        } // catch

        catch (InternalError e)
        {
            System.err.println("Your Java Virtual Machine has thrown" +
                               " an error...chances are, you don't have");
            System.err.println(" the GUI support you need. Please start" +
                               " your Window system before running this");
            System.err.println(" program. For record, here's the error:");
            System.err.println("\"" + e.getMessage() + "\"");
            System.exit(0);
        } // catch
    } // kickOffGUI


    public static void printBanner()
    {
        System.out.println();
        System.out.println(TITLE + " " + VERSION);
        System.out.println("  " + COPYRIGHT);
        System.out.println("  " + WRITTENBY);
        System.out.println();
    } // printBanner


    public static void main(String[] args)
    {
        printBanner();

        if (badJavaVersion())
            System.err.println(MSG_BAD_JAVA);
        else
            kickOffGUI(args);
    } // main



        // DocumentListener implementation...

    public void changedUpdate(DocumentEvent e)
    {
        textModificationAlert();
    } // changeUpdate


    public void insertUpdate(DocumentEvent e)
    {
        textModificationAlert();
    } // insertUpdate


    public void removeUpdate(DocumentEvent e)
    {
        textModificationAlert();
    } // removeUpdate


    private void textModificationAlert()
    {
        if (textModified == false)
        {
            textModified = true;
            tmb.setSaveEnabled(true);
            setTitle();
        } // if
    } // textModified


    public boolean requestToQuit()
    {
        boolean retVal = false;

        if (saveAndContinue() == true)
        {
            dispose();
            retVal = true;
        } // if

        return(retVal);
    } // requestToQuit


        // WindowListener implementation...

    public void windowClosing(WindowEvent e)
    {
        requestToQuit();
    } // windowClosing


    public void windowClosed(WindowEvent e)
    {
        System.exit(0);
    } // windowClosed

    public void windowOpened(WindowEvent e)
    {
//        setTobyIcon();
    } // windowOpened

    public void windowIconified(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowActivated(WindowEvent e) {}
    public void windowDeactivated(WindowEvent e) {}

} // Toby

// end of Toby.java ...


