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

package last.toby;

import last.toby.gui.*;
import last.toby.util.*;

    /**
     *  This class is used to kick off the rest of the application.
     *   main() resides here.
     *
     *     @author Ryan C. Gordon (icculus@lokigames.com)
     */
public final class Toby
{
    public static final String TITLE   = "Toby";
    public static final String VERSION = "v1.0alpha5";

    private static void kickOffGUI(String fileName)
    {
        TobyFrame toby = null;
        TobySplash splash;
        Error error = null;

        try
        {
            splash = new TobySplash("splash.jpg", VERSION);
            toby = new TobyFrame();
            splash.doneWithSplash(1500);
        } // try

        catch (NoClassDefFoundError e)
        {
            System.err.println(TobyLanguage.MISSING_CLASS1);
            System.err.println(TobyLanguage.MISSING_CLASS2);
            System.err.println(TobyLanguage.MISSING_CLASS3);
            error = e;
        } // catch

        catch (InternalError e)
        {
            System.err.println(TobyLanguage.NO_GUI1);
            System.err.println(TobyLanguage.NO_GUI2);
            System.err.println(TobyLanguage.NO_GUI3);
            error = e;
        } // catch

        if (error != null)
        {
            System.err.println();
            System.err.println(TobyLanguage.HERES_THE_ERR);
            System.err.println();
            System.err.println("   \"" + error.getMessage() + "\"");
            System.err.println();
            System.exit(0);
        } // if

        if ((fileName != null) && (toby != null))
            toby.openFile(fileName);
    } // kickOffGUI


    public static void main(String[] args)
    {
        if (Incompatibilities.checkForIncompatibilities() == false)
            System.exit(0);
        else
        {
            if (args.length <= 1)
                kickOffGUI((args.length == 1) ? args[0] : null);
            else
            {
                System.err.println(TobyLanguage.USAGE);
                System.err.println();
                System.exit(0);
            } // else
        } // else
    } // main

} // Toby

// end of Toby.java ...

