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
 * The NumberIntrinsic represent numeric variables in TOBY. The numbers
 *  are stored as Java doubles, so it can hold real and whole numbers
 *  with a lot of precision.
 *
 *     Written by Ryan C. Gordon.
 */

public class NumberIntrinsic extends Intrinsic
{
    public NumberIntrinsic(String ident, String initValue)
                               throws TobyParseException
    {
        super(ident, initValue);
    } // Constructor


    public NumberIntrinsic(String ident)
    {
        super(ident);
        value = "0.0";
    } // Constructor


    public String getType()
    {
        return(TobyInterpreter.INTRINSIC_NUMBER);
    } // getType


    public void setValue(String newValue) throws TobyParseException
    {
        try
        {
                // Wrapping it in a Double class checks for a valid number...
            value = new Double(newValue).toString();
        } // try
        catch (NumberFormatException nfe)
        {
            TobyParseException.throwException(TobyInterpreter.PROCERR_BAD_TYPE,
                                              null);
        } // catch
    } // setValue

} // NumberIntrinsic

// end of NumberIntrinsic.java ...


