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

public final class Toby extends JFrame
{
        // Global Constants...
    public static final String TITLE   = "Toby";
    public static final String VERSION = "v0.50alpha";

    public static final String USAGE = "USAGE: Toby.class [sourceFile.toby]";

    public static final String WRITTENBY = "Written by Ryan C. Gordon.";
    public static final String COPYRIGHT =
                     "Copyright (c) 1997 Lighting and Sound Technologies";

    public static final String MSG_ERROR = "Error";

    public static final String MSG_BAD_JAVA =
         "Your Java Virtual Machine is too old; goto http://java.sun.com/ ...";

        // Global variables...
    public final static boolean debugging = false;
    public static AppManager appMan = null;

        // Instance variables...
    private TobyPanel tobyPanel;
    private String tobyFileName = null;
    private File currentDirectory = null;

    public Toby()
    {
        super();

        Container rootPane = getContentPane();
        TobyMenuBar tmb = new TobyMenuBar(this);
        TurtleSpace tspace;

        currentDirectory = new File(".");   // !!! is this kosher?
        setTitle(null);
 
        addWindowListener(appMan);
        rootPane.setLayout(new BorderLayout());
        setResizable(true);
        setSize(getToolkit().getScreenSize());
        setJMenuBar(tmb);

        tobyPanel = new TobyPanel(this);
        rootPane.add("Center", tobyPanel);

        tspace = tobyPanel.getTurtleSpace();
        tspace.addSourceWatcher(tmb);
        setVisible(true);
        show();
    } // Constructor


    public void setTitle(String subTitle)
    {
        if (subTitle == null)
            subTitle = "new program";

        super.setTitle(TITLE + " " + VERSION + "   [" + subTitle + "]");
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
        // !!! check if code is modified, and wants to be saved...

        tobyPanel.getInputArea().clearCode();
        setTitle(null);
    } // newFile


    public void openFile(String fileName)
    {
        LineNumberReader br;

            // !!! ask to save modified buffer?

        if (fileName == null)
            fileName = selectFileName(JFileChooser.OPEN_DIALOG);

        if (fileName != null)
        {
            try
            {
                br = new LineNumberReader(new FileReader(fileName));
                tobyPanel.getInputArea().setSource(br);
                tobyFileName = fileName;
                setTitle(tobyFileName);
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
    } // openFile


    public void saveFile(String fileName)
    {
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
                setTitle(tobyFileName);
            } // try
            catch (IOException ioe)
            {
                String errMsg;
                errMsg = "Cannot write to [" + tobyFileName + "]";
                JOptionPane.showMessageDialog(null, errMsg, "Error",
                                      JOptionPane.ERROR_MESSAGE);
            } // catch
        } // if
    } // saveFile


    private static void kickOffGUI(String[] args)
    {
        Toby toby;

        try
        {
            if (args.length <= 1)
            {
                appMan = new AppManager();
                toby = new Toby();
                toby.tobyPanel.getTurtleSpace().cleanup();  // !!! hack
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

} // Toby

// end of Toby.java ...


