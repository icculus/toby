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
     *  !!! comment me.
     *   @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public class NumberIntrinsic extends Intrinsic
{
    protected double value = 0.00;

    public NumberIntrinsic()
    {
        this(0.00);
    } // Constructor (overloaded; default.)

    public NumberIntrinsic(double value)
    {
        super();
        this.value = value;
    } // Constructor (overloaded; takes double.)

    public boolean isConstantValue()
    {
        return(true);
    } // isConstantValue

    public String toString()
    {
        return(java.lang.Double.toString(value));
    } // toString

    protected Intrinsic getCopyImpl()
    {
        NumberIntrinsic retVal = new NumberIntrinsic();
        retVal.value = this.value;
        return(retVal);
    } // getCopyImpl

    public void setValue(Intrinsic newVal) throws FlowException
    {
        value = newVal.getDoubleValue();
    } // setValue overload

    public void setValue(int newVal) throws FlowException
    {
        value = (double) newVal;
    } // setValue overload

    public void setValue(double newVal) throws FlowException
    {
        value = newVal;
    } // setValue overload

    public void setValue(float newVal) throws FlowException
    {
        value = (double) newVal;
    } // setValue overload

    public void setValue(byte newVal) throws FlowException
    {
        value = (double) newVal;
    } // setValue overload

    public void setValue(long newVal) throws FlowException
    {
        value = (double) newVal;
    } // setValue overload

    public void setValue(short newVal) throws FlowException
    {
        value = (double) newVal;
    } // setValue overload

    public double getDoubleValue() throws FlowException
    {
        return(value);
    } // getValue overload

    public int getIntValue() throws FlowException
    {
        return((int) value);
    } // getValue overload

    public char getCharValue() throws FlowException
    {
        return((char) value);
    } // getValue overload

    public byte getByteValue() throws FlowException
    {
        return((byte) value);
    } // getValue overload

    public long getLongValue() throws FlowException
    {
        return((long) value);
    } // getValue overload

    public short getShortValue() throws FlowException
    {
        return((short) value);
    } // getValue overload

    public float getFloatValue() throws FlowException
    {
        return((float) value);
    } // getValue overload

    public Intrinsic getIntrinsicValue() throws FlowException
    {
        return(getCopy());
    } // getValue overload

    public int compare(Intrinsic intr) throws FlowException
    {
        double d = intr.getDoubleValue();
        int retVal = 0;

        if (value > d)
            retVal = 1;
        else if (value < d)
            retVal = -1;

        return(retVal);
    } // compare override

    public boolean isEqual(Intrinsic intr) throws FlowException
    {
        return(compare(intr) == 0);
    } // isEqual override

    protected void linkImpl(GlobalLogicContext glob) throws ParseException {}
} // NumberIntrinsic

// end of NumberIntrinsic.java ...

