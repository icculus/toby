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
 * TOBY intrinsic: The Boolean (true/false) type.
 *
 *    Written by Ryan C. Gordon.
 */

public class BooleanIntrinsic extends Intrinsic
{
    public BooleanIntrinsic(String ident, String initValue)
                               throws TobyParseException
    {
        super(ident, initValue);
    } // Constructor


    public BooleanIntrinsic(String ident)
    {
        super(ident);
        value = TobyInterpreter.KEYWORD_FALSE;
    } // Constructor


    public String getType()
    {
        return(TobyInterpreter.INTRINSIC_BOOLEAN);
    } // getType


    public void setValue(String newValue) throws TobyParseException
    {
        if ((newValue.equals(TobyInterpreter.KEYWORD_TRUE)) ||
            (newValue.equals(TobyInterpreter.KEYWORD_FALSE)))
        {
            value = newValue;
        } // if
        else
        {
            TobyParseException.throwException(TobyInterpreter.PROCERR_BAD_TYPE,
                                              null);
        } // else
    } // setValue

} // BooleanIntrinsic

// end of BooleanIntrinsic.java ...


