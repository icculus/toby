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
 * The NothingIntrinsic is somewhat equivalent to C's <em>void</em>.
 *
 * Don't instantiate this class; use NothingIntrinsic.nothing
 *  for your needs. In fact, you CAN'T instantiate it; the constructor
 *  is private.
 *
 *    Written by Ryan C. Gordon.
 */

public class NothingIntrinsic extends Intrinsic
{
    public static NothingIntrinsic nothing = new NothingIntrinsic();


    private NothingIntrinsic()
    {
        super(null);
    } // Constructor


    public String getType()
    {
        return(TobyInterpreter.INTRINSIC_NOTHING);
    } // getType


    public void setValue(String newValue) throws TobyParseException
    {
        if (newValue != null)
        {
            TobyParseException.throwException(TobyInterpreter.PROCERR_BAD_TYPE,
                                              null);
        } // if

        value = null;
    } // setValue

} // NothingIntrinsic

// end of NothingIntrinsic.java ...


