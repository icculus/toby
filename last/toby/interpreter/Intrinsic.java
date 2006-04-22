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
     * This is an abstract class that represents an intrinsic data type in
     *  TOBY during interpretation.
     *
     *   @author Ryan C. Gordon. (icculus@icculus.org)
     */
public abstract class Intrinsic implements Linkable
{
    protected String name = null;  // !!! make this private. Use getters/setters.

        /**
         * If (debugging) is (true), then println debug info to stdout.
         *
         *   @param funcName name of method where debugging info lies.
         *   @param msg Message to print.
         */
//    public static void _D(String funcName, String msg)
//    {
//        last.toby.util.BugSquasher.println(funcName, msg);
//    } // _D

    public void setIdentifier(String name)
    {
        this.name = name;
    } // setName

    public String getIdentifier()
    {
        return(this.name);
    } // getIdentifier

    public void setValue(Intrinsic newVal) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

    public void setValue(int newVal) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

    public void setValue(double newVal) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

    public void setValue(float newVal) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

    public void setValue(char newVal) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

    public void setValue(byte newVal) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

    public void setValue(long newVal) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

    public void setValue(short newVal) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

    public void setValue(boolean newVal) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

    public void setValue(Object newVal) throws FlowException
    {
        if (newVal instanceof Intrinsic)
            setValue((Intrinsic) newVal);
        else
            ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

    public void setValue(String newVal) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
    } // setValue overload

// !!! change all these name comments from "overload"...

    public int getIntValue() throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return(0);
    } // getValue overload

    public char getCharValue() throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return('0');
    } // getValue overload

    public byte getByteValue() throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return(0);
    } // getValue overload

    public long getLongValue() throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return(0);
    } // getValue overload

    public short getShortValue() throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return(0);
    } // getValue overload

    public boolean getBooleanValue() throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return(false);
    } // getValue overload
    
    public String getStringValue() throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return(null);
    } // getValue overload

    public float getFloatValue() throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return((float) 0.00);
    } // getValue overload

    public double getDoubleValue() throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return((double) 0.00);
    } // getValue overload

    public Object getObjectValue() throws FlowException
    {
        return(this);
    } // getValue overload

    public Intrinsic getIntrinsicValue() throws FlowException
    {
        return(this);
    } // getValue overload

    public int compare(Intrinsic intr) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return(0);
    } // compare

    public boolean isEqual(Intrinsic intr) throws FlowException
    {
        ExecException._throw(TobyLanguage.TYPE_MMATCH);  // !!!
        return(false);
    } // isEqual

    public void link(GlobalLogicContext glob) throws ParseException
    {
        // just to keep things consistent...
        linkImpl(glob);
    } // link

    public final Intrinsic getCopy()
    {
        Intrinsic retVal = getCopyImpl();
        retVal.setIdentifier(this.name);
        return(retVal);
    } // getCopy

    public abstract boolean isConstantValue();
    protected abstract Intrinsic getCopyImpl();
    protected abstract void linkImpl(GlobalLogicContext glob)
                                     throws ParseException;
} // Intrinsic

// end of Intrinsic.java ...

