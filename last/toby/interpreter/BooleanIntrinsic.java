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

package last.toby.interpreter;
import last.toby.exceptions.*;
import last.toby.util.TobyLanguage;

    /**
     *  !!! comment.
     *   @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public class BooleanIntrinsic extends Intrinsic
{
    public static final BooleanIntrinsic TRUE = new BooleanIntrinsic(true);
    public static final BooleanIntrinsic FALSE = new BooleanIntrinsic(false);

    protected boolean value = false;

    public BooleanIntrinsic()
    {
        this(false);
    } // Constructor (overloaded; default.)

    public BooleanIntrinsic(boolean value)
    {
        super();
        this.value = value;
    } // Constructor (overloaded; takes double.)

    public String toString()
    {
        return(value ? "true" : "false");
    } // toString

    protected Intrinsic getCopyImpl()
    {
        BooleanIntrinsic retVal = new BooleanIntrinsic();
        retVal.value = this.value;
        return(retVal);
    } // getCopyImpl

    public boolean isConstantValue()
    {
        return(true);
    } // isConstantValue

    public void setValue(boolean newVal) throws FlowException
    {
        this.value = newVal;
    } // setValue overload

    public void setValue(Intrinsic newVal) throws FlowException
    {
        value = newVal.getBooleanValue();
    } // setValue overload

    public boolean getBooleanValue() throws FlowException
    {
        return(value);
    } // getValue

    public boolean isEqual(Intrinsic intr) throws FlowException
    {
        return(intr.getBooleanValue() == this.value);
    } // isEqual override

    protected void linkImpl(GlobalLogicContext glob) throws ParseException {}
} // BooleanIntrinsic

// end of BooleanIntrinsic.java ...

