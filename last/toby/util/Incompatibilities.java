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
 * This class deals with incompatibilities in various Java implementations.
 *  For example, earlier versions of Blackdown.org's JVM for Intel Linux
 *  crash if you try to set a Frame's icon in a certain way. This is
 *  the apocrypha. This class is here to encapsulate all the esoteric
 *  quirks of various platforms, so that these minor bug checks don't
 *  need to be littered throughout the rest of the code.
 *
 * To use this class, just call the static method checkForIncompatibilities().
 *  If it returns (false), there is a crippling problem with the running
 *  virtual machine and/or class libraries, and the program should abort
 *  now to prevent ugliness later. This is mostly a version check to make
 *  sure that you have at least a Java 1.2 environment. After this call,
 *  all the public static fields are initialized, and may be checked to
 *  determine if a known incompatibility exists on this platform.
 *
 *      Written by Ryan C. Gordon.
 */

package last.toby.util;

public class Incompatibilities
{
    public static boolean isSetFrameIconBroken = true;

        /**
         * Older versions of Blackdown.org's JVM choked on Frame.setIcon()
         *  calls. If this VM, or any other VM known to have this bug,
         *  is running, set (isSetFrameIconBroken) to (true).
         */
    private static void checkIfSetFrameIconBroken()
    {
        String verStr = System.getProperty("java.version");
        String osName = System.getProperty("os.name").toLowerCase();
        String vendor = System.getProperty("java.vendor").toLowerCase();

        if ( (osName.equals("linux")) &&
             (vendor.indexOf("blackdown") != -1) &&
             (verStr.compareTo("1.1.7") <= 0) )
        {
            isSetFrameIconBroken = true;
            System.err.println(TobyLanguage.SETICON_BROKEN);
            System.err.println(TobyLanguage.WORK_AROUND_IT);
            System.err.println();
        } // if
        else
            isSetFrameIconBroken = false;
    } // checkIfSetFrameIconBroken


        /**
         *  Check if we are running a Java VM that is at least
         *   version 1.2, since some of this code is Java 1.2-specific.
         *   Also checks for other implementation-specific problems.
         *
         *    @return (true) if VM is 1.2 compatible, (false) otherwise.
         */
    private static boolean badJavaVersion()
    {
        String verStr = System.getProperty("java.version");
        boolean retVal = false;

        if (verStr.compareTo("1.2") < 0)        // Earlier than Java 1.2?
        {
            System.err.println(TobyLanguage.MSG_BAD_JAVA);
            retVal = true;
        } // if

        return(retVal);
    } // badJavaVersion


        /**
         * Call this method before doing anything substantial in your
         *  program. If it returns (false), terminate the program
         *  immediately. After this call, the public static fields
         *  of this class are initialized and can be checked for
         *  finer-grained incompatibility details.
         *
         *    @return (false) if crippling error, (true) if everything's ok.
         */
    public static boolean checkForIncompatibilities()
    {
        boolean retVal = true;

        checkIfSetFrameIconBroken();
        if (badJavaVersion() == true)
            retVal = false;

        return(retVal);
    } // checkForIncompatibilites

} // Incompatibilities

// end if Incompatibilities.java ...

