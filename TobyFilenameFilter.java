/*
 * This really simple class just verifies that only *.toby files are
 *  acceptable to a JFileChooser.
 *
 *   Copyright (c) Lighting and Sound Technologies, 1999.
 *    Written by Ryan C. Gordon.
 */

public class TobyFilenameFilter extends javax.swing.filechooser.FileFilter
{

        // FilenameFilter implementation ...

    public boolean accept(java.io.File file)
    {
        if (file.isDirectory())
            return(true);

        String name = file.getName();
//        System.out.println("asking for [" + name + "]...");
        return( (name.toLowerCase().endsWith(".toby")) ? true : false );
    } // accept

    public String getDescription()
    {
        return("TOBY source code");
    } // getDescription
} // TobyFilenameFilter

// end of TobyFilenameFilter.java ...

