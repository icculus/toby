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

package last.toby.util;
import last.toby.exceptions.*;

    /**
     *  !!! comment me!
     *
     *  @author Ryan C. Gordon.
     */
public class BugSquasher
{
    protected static final boolean debugging = true;

// !!! Use a generic exception.
        /**
         * Works like a C++ assertion.
         *
         *   assert(2 > 4) will throw a ParseException, since it's false.
         *   assert(2 < 4) will not.
         *
         *   @param expression true or false expression.
         *   @throw ParseException if (expression) is true.
         */
    public static void _assert(boolean expression) throws ParseException
    {
        if (debugging == true)
        {
            if (expression == true)
                ParseException._throw(TobyLanguage.INTERNAL_ERROR);
        } // if
    } // _assert


        /**
         * If (debugging) is (true), then println debug info to stdout.
         *
         *   @param funcName name of method where debugging info lies.
         *   @param msg Message to print.
         */
    public static void println(String funcName, String msg)
    {
        if (debugging == true)
        {
            System.out.print(" * ");
            if (funcName != null)
            {
                System.out.print(funcName);
                System.out.print("(): ");
            } // if
            System.out.println(msg);
        } // if
    } // println

} // BugSquasher

// end of BugSquasher.java ...

