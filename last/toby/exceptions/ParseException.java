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

package last.toby.exceptions;

    /**
     * !!! comment me.
     *
     *   @author Ryan C. Gordon (icculus@icculus.org).
     */
public class ParseException extends FlowException
{
    private ParseException() {}  // prevent public construction

    public static ParseException getInstance()
    {
        return(new ParseException());  // !!!
    } // getInstance

    public static void _throw(String err, String p, int l) throws ParseException
    {
        ParseException e = getInstance();
        initThrow(e, err, p, l);
        throw(e);
    } // _throw

    public static void _throw(String err, String pName) throws ParseException
    {
        ParseException e = getInstance();
        initThrow(e, err, pName);
        throw(e);
    } // _throw

    public static void _throw(String err, int lineNum) throws ParseException
    {
        ParseException e = getInstance();
        initThrow(e, err, lineNum);
        throw(e);
    } // _throw

    public static void _throw(String err) throws ParseException
    {
        ParseException e = getInstance();
        initThrow(e, err);
        throw(e);
    } // _throw

} // ParseException

// end of ParseException.java ...


