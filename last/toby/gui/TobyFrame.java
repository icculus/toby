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
import java.io.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
//import java.awt.print.*;

import last.toby.Toby;
import last.toby.util.*;
import last.toby.turtlespace.swing.*;

    /**
     *  This class is the encompassing Frame component, and contains the
     *   menubar and other GUI components. File i/o is also handled here.
     *
     *    @author Ryan C. Gordon. (icculus@icculus.org)
     */
public final class TobyFrame extends JFrame implements DocumentListener,
                                                       WindowListener
{
        // Instance variables...
    private TobyPanel tobyPanel;
    private String fileName = null;
    private File currentDirectory = null;
    private boolean textModified = false;
    private TobyMenuBar tmb = null;

        // !!! remove these later.
    private boolean painted = false;
    private String fileOpenPending = null;


    public TobyFrame()
    {
        super();

        Dimension d = getToolkit().getScreenSize();
        Container rootPane = getContentPane();

        tmb = new TobyMenuBar(this);
        currentDirectory = new File(System.getProperty("user.dir"));
        setTitle();
 
        addWindowListener(this);
        rootPane.setLayout(new BorderLayout());
        setResizable(true);
        setLocation((int) (d.width * 0.05), (int) (d.height * 0.05));
        setSize((int) (d.width * 0.90), (int) (d.height * 0.90));
        setJMenuBar(tmb);

        tobyPanel = new TobyPanel(this);
        rootPane.add("Center", tobyPanel);

        setVisible(true);
    } // Constructor


    private void setTobyIcon()
    {
        /*  !!! TriangleTurtle is gone...
        if (Incompatibilities.isSetFrameIconBroken == false)
        {
            Turtle turtle = new TriangleTurtle();
            Image image = createImage(24, 24);
            Graphics g = image.getGraphics();

            g.setColor(Color.black);
            g.fillRect(0, 0, 24, 24);
            turtle.setWidth(24);
            turtle.setHeight(24);
            turtle.setXY(12, 12, g, image);
            g.dispose();

            setIconImage(image);
        } // if
        */
    } // setTobyIcon


        // !!! just gets around a stupid bug. Remove later.
    public void paint(Graphics g)
    {
        super.paint(g);
        this.painted = true;
        if (fileOpenPending != null)
        {
            openFile(this.fileOpenPending);
            this.fileOpenPending = null;
        } // if
    } // paint


    public void doPrint()
    {
        String msg = TobyLanguage.NOT_YET_IMPLEMENTED;
        JOptionPane.showMessageDialog(null, msg,
                                      Toby.TITLE + " " + Toby.VERSION,
                                      JOptionPane.INFORMATION_MESSAGE);

//        PrinterJob.printDialog();
//        PrinterJob.pageDialog();
    } // doPrint


    public void setTitle()
    {
        StringBuffer sb = new StringBuffer();

        sb.append(Toby.TITLE);
        sb.append(" ");
        sb.append(Toby.VERSION);
        sb.append(": ");

        if (fileName != null)
            sb.append(fileName);
        else
        {
            sb.append('[');
            sb.append(TobyLanguage.NEW_PROGRAM);
            sb.append(']');
        } // else

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
        return(fileName);
    } // getCurrentFileName


    public void displayHelp()
    {
            // JavaHelp, perhaps?
        String msg = TobyLanguage.NOT_YET_IMPLEMENTED;

        JOptionPane.showMessageDialog(null, msg,
                                      Toby.TITLE + " " + Toby.VERSION,
                                      JOptionPane.INFORMATION_MESSAGE);
    } // displayHelp


    public void createAboutBox()
    {
        TobySplash splash = new TobySplash("splash.jpg", Toby.VERSION);

        splash.addMouseListener
        (
            new MouseAdapter()
            {
                public void mouseClicked(MouseEvent e)
                {
                    ((TobySplash) e.getSource()).doneWithSplash(0);
                } // focusLost
            } // FocusAdapter
        );
    } // createAboutBox


    private String selectFileName(int fileMode)
    {
        JFileChooser chooser = new JFileChooser();
        int rc;
        String retVal = null;

        chooser.setFileFilter(new GenericFilenameFilter(".toby", "TOBY source code"));   // !!! temp!
        chooser.setMultiSelectionEnabled(false);
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileHidingEnabled(false);
        chooser.setCurrentDirectory(currentDirectory);

        if (fileMode == JFileChooser.SAVE_DIALOG)
            rc = chooser.showSaveDialog(this);
        else if (fileMode == JFileChooser.OPEN_DIALOG)
            rc = chooser.showOpenDialog(this);
        else
            return(null);

        if (rc == JFileChooser.APPROVE_OPTION)
        {
            currentDirectory = chooser.getSelectedFile();
            retVal = currentDirectory.getPath();
        } // if
        
        return(retVal);
    } // selectFileName


    public void newFile()
    {
        if (saveAndContinue() == true)
        {
            tobyPanel.getInputArea().clearCode();
            fileName = null;
            textModified = false;
            tmb.setSaveEnabled(false);
            setTitle();
        } // if
    } // newFile


    public void openFile(String fileName)
    {

                // !!! remove me!
            if (this.painted == false)
            {
                this.fileOpenPending = fileName;
                return;
            } // if


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
                } // try
                catch (FileNotFoundException fnfe)
                {
                        // Not found? Try adding a ".toby" to the filename
                        //  and try again...
                    try
                    {
                        fileName += ".toby";
                        br = new LineNumberReader(new FileReader(fileName));
                    } // try
                    catch (FileNotFoundException fnfe2)
                    {
                            // Still not found? Report the error...
                        String errMsg;
                        br = null;
                        errMsg = TobyLanguage.FILE_NOT_FOUND;
                        errMsg = TobyLanguage.replaceFmtTokenInStr(0, errMsg, fileName);
                        JOptionPane.showMessageDialog(null, errMsg,
                                              TobyLanguage.ERROR,
                                              JOptionPane.ERROR_MESSAGE);
                    } // catch
                } // catch

                try
                {
                    if (br != null)
                    {
                        tobyPanel.getInputArea().setSource(br);
                        this.fileName = fileName;
                        textModified = false;
                        tmb.setSaveEnabled(false);
                        setTitle();
                    } // if
                } // try
                catch (IOException ioe)
                {
                    String errMsg;
                    errMsg = TobyLanguage.CANNOT_LOAD_FILE;
                    errMsg = TobyLanguage.replaceFmtTokenInStr(0, errMsg, fileName);
                    JOptionPane.showMessageDialog(null, errMsg,
                                          TobyLanguage.ERROR,
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
                                            TobyLanguage.SAVE_MODIFIED_PROG,
                                            TobyLanguage.PLEASE_CONFIRM,
                                            JOptionPane.YES_NO_CANCEL_OPTION,
                                            JOptionPane.QUESTION_MESSAGE);

            if (rc == JOptionPane.YES_OPTION)
                retVal = saveFile(fileName);
            else if (rc == JOptionPane.CANCEL_OPTION)
                retVal = false;
        } // if

        return(retVal);
    } // saveAndContinue


    public boolean saveFile(String fName)
    {
        boolean retVal = false;
        BufferedWriter bw;
        String src;
        int rc;

        if (fName == null)
        {
            fName = selectFileName(JFileChooser.SAVE_DIALOG);

            if (fName != null)
            {
                fName = fName.trim();
                if (fName.toLowerCase().endsWith(".toby") == false)
                    fName += ".toby";

                    /*
                     * Normally we use (currentDirectory) just to extract
                     *  the path, but it DOES contain the filename, too.
                     */
                if (currentDirectory.exists() == true)
                {
                    String str = TobyLanguage.OVERWRITE_FILENAME;
                    str = TobyLanguage.replaceFmtTokenInStr(0, str, fName);
                    rc = JOptionPane.showConfirmDialog(null, str,
                                            TobyLanguage.FILE_EXISTS,
                                            JOptionPane.YES_NO_CANCEL_OPTION,
                                            JOptionPane.QUESTION_MESSAGE);

                    if (rc == JOptionPane.CANCEL_OPTION)
                        return(false);
                    else if (rc == JOptionPane.NO_OPTION)
                        return(saveFile(null));
                    // JOptionPane.YES_OPTION falls through to save file.
                } // if
            } // if
        } // if

        if (fName != null)
        {
            src = tobyPanel.getInputArea().getSource();

            try
            {
                bw = new BufferedWriter(new FileWriter(fName));
                bw.write(src, 0, src.length());
                bw.close();
                this.fileName = fName;
                textModified = false;
                tmb.setSaveEnabled(false);
                setTitle();
                retVal = true;
            } // try
            catch (IOException ioe)
            {
                String errMsg = TobyLanguage.CANNOT_WRITE_TO;
                errMsg = TobyLanguage.replaceFmtTokenInStr(0, errMsg, fName);
                JOptionPane.showMessageDialog(null, errMsg, TobyLanguage.ERROR,
                                              JOptionPane.ERROR_MESSAGE);
            } // catch
        } // if

        return(retVal);
    } // saveFile


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
        setTobyIcon();
    } // windowOpened

    public void windowIconified(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowActivated(WindowEvent e) {}
    public void windowDeactivated(WindowEvent e) {}

} // Toby

// end of Toby.java ...


