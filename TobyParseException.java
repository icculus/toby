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
 *  A simple class filled with details of a parsing error.
 *
 *   Written by Ryan C. Gordon.
 */


public class TobyParseException extends Exception
{
        // Constants...
    public final static int LINENUM_UNDEFINED = -1;


        // Instance variables...
    public String procName = null;
    public int errLine = LINENUM_UNDEFINED;
    public boolean terminated = false;


        // Methods...

    public TobyParseException(String err)
    /**
     *  Contructor; Just here to pass a String to Exception.<init>...
     *
     *    params : err == String for Exception contructor...
     *   returns : Constructor; void.
     */
    {
        super(err);
    } // Constructor


    public static void throwException(String err, String pName, int lineNum)
                                         throws TobyParseException
    /**
     *  Call this for convenient TPE throwing...
     *
     *     params : err  == error message.
     *              proc == reference to procedure object that choked.
     *    returns : technically, void. Always throws an exception, though.
     *     throws : TobyParseException == hey, why else did you call this?
     */
    {
        TobyParseException tpe = new TobyParseException(err);

        tpe.errLine = lineNum;
        tpe.procName = pName;
        throw(tpe);
    } // throwException (overloaded; included line number.)


    public static void throwException(String err, String pName)
                                         throws TobyParseException
    /**
     *  Call this for convenient TPE throwing...
     *
     *     params : err  == error message.
     *              proc == reference to procedure object that choked.
     *    returns : technically, void. Always throws an exception, though.
     *     throws : TobyParseException == hey, why else did you call this?
     */
    {
        throwException(err, pName, LINENUM_UNDEFINED);
    } // throwException (overloaded; no line number.)

} // TobyParseException

// end of TobyParseException.java ...


