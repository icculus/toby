/*
 * This really simple class just verifies that only *.toby files are
 *  acceptable to a FileDialog.
 *
 *   Copyright (c) Lighting and Sound Technologies, 1999.
 *    Written by Ryan C. Gordon.
 */

public class TobyFilenameFilter implements java.io.FilenameFilter
{

        // FilenameFilter implementation ...

    public boolean accept(java.io.File dir, String name)
    {
        System.out.println("asking for [" + name + "]...");
        return( (name.toLowerCase().endsWith(".toby")) ? true : false );
    } // accept
} // TobyFilenameFilter

// end of TobyFilenameFilter.java ...
