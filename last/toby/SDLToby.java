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

// !!! ugh.
import java.io.*;
import java.net.*;

import last.toby.gui.*;
import last.toby.util.*;
import last.toby.turtlespace.*;
import last.toby.turtlespace.sdl.*;
import last.toby.parsers.*;
import last.toby.parsers.toby.*;
import last.toby.interpreter.*;


    /**
     *  This class is used to kick off the rest of the application.
     *   main() resides here.
     *
     *     @author Ryan C. Gordon (icculus@icculus.org)
     */
public final class SDLToby
{
    public static final String TITLE   = "Toby";
    public static final String VERSION = "v1.0alpha5";

    public static String sourceToLoad = null;

    public static void printUsage()
    {
        System.err.println(TobyLanguage.USAGE);
        System.err.println();
    } // printUsage

    public static boolean checkCommandLine(String[] args)
    {
        int max = args.length;
        boolean tooManyFiles = false;

        for (int i = 0; i < max; i++)
        {
            String arg = args[i].trim();

            if (arg.startsWith("--") == true)
            {
                if (arg.startsWith("--langfile=") == true)
                {
                    // !!! this is a bad system.
                    String s = arg.substring(11);
                    try
                    {
                        LineNumberReader lnr = null;
                        if (s.indexOf("://") == -1)
                        {
                            lnr = new LineNumberReader(new FileReader(s));
                        } // if
                        else
                        {
                            URL url = null;
                            try
                            {
                                url = new URL(s);
                            } // try
                            catch (MalformedURLException murle)
                            {
                                // can't localize this.
                                System.err.println("Bad URL in --langfile!");
                                return(false);
                            } // catch

                            InputStream is = url.openStream();
                            InputStreamReader isr = new InputStreamReader(is);

                            lnr = new LineNumberReader(isr);
                        } // else

                        TobyLanguage.loadLangFile(lnr, s);
                        lnr.close();
                    } // try

                    catch (IOException ioe)
                    {
                            // may or may not be localized at point of trouble.
                        System.err.println(TobyLanguage.LANGIOEXCEPT);

                            //  ...so this will have to do...
                        System.err.println(ioe.getMessage());
                        return(false);
                    } // catch
                } // if

                else
                {
                    printUsage();
                    return(false);
                } // else
            } // if

            else   // a non "--commandline" ...
            {
                if (sourceToLoad != null)
                    tooManyFiles = true;
                else
                    sourceToLoad = arg;
            } // else
        } // for

            // this is here so --langfile gets processed before output.
        if (tooManyFiles)
        {
            System.err.println(TobyLanguage.TOOMANYFILES);
            return(false);
        } // if

        return(true);
    } // checkCommandLine


    private static void runTobyProgram()
    {
        try
        {

        SDLRenderer renderer = new SDLRenderer();
        TurtleSpace tspace = new TurtleSpace(renderer);

        tspace.grabTurtleSpace();
        tspace.removeAllTurtles();
        tspace.addTurtle();
        tspace.useTurtle(0);
        tspace.defaultAllTurtles();
        tspace.cleanup();
        tspace.enableFence();

            // !!! TobyParser?
        Parser p = new last.toby.parsers.toby.TobyParser();
        BufferedReader br = new BufferedReader(new FileReader(sourceToLoad));
        GlobalLogicContext globalContext = p.parseCode(br, tspace);

        globalContext.resetState();
        globalContext.execute();

        long parseTime = p.getTotalParseTime();
        long linkTime = p.getTotalLinkTime();
        long execTime = globalContext.getTotalExecTime();
        long totalTime = execTime + parseTime + linkTime;

            // !!! This needs to make it's way into the GUI, and TobyLanguage.

        System.err.println();
        System.err.println(" !!! unlocalized text: !!!");
        System.err.println("Parse time : " + parseTime + " milliseconds.");
        System.err.println("Link  time : " + linkTime + " milliseconds.");
        System.err.println("Exec. time : " + execTime + " milliseconds.");
        System.err.println("Total time : " + totalTime + " milliseconds.");
        System.err.println();

        Thread.sleep(10 * 1000);

        } // try
        catch (Exception e)
        {
            System.err.println("EXCEPTION.");
            System.err.println( e.getClass().getName() + ": " + e.getMessage());
            e.printStackTrace();
        } // catch
    } // runTobyProgram


    public static void main(String[] args)
    {
        if (checkCommandLine(args) == true)
        {
            if (Incompatibilities.checkForIncompatibilities() == true)
            {
                if (sourceToLoad != null)
                    runTobyProgram();
            } // if
        } // else
    } // main

} // Toby

// end of Toby.java ...

