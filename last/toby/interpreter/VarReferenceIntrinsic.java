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
import last.toby.util.*;
import last.toby.exceptions.*;

    /**
     * !!! comment me!
     *
     *   @author Ryan C. Gordon. (icculus@icculus.org)
     */
public class VarReferenceIntrinsic extends Intrinsic
{
    protected GlobalLogicContext globalContext = null;
    protected LogicContext parentContext = null;
    protected int varIndex = 0xDEADBEEF;
    protected boolean global = false;
    protected int lineno = -1;

    public VarReferenceIntrinsic(String identifier,
                                 LogicContext parentContext,
                                 int lineNum)
    {
        setIdentifier(identifier);
        this.parentContext = parentContext;
        this.lineno = lineNum;
    } // Constructor

    public boolean isConstantValue()
    {
        return(false);
    } // isConstantValue

    public String toString()
    {
        String str = null;
        return(name + "==[" + getRealVar().toString() + "]");
    } // toString

    protected Intrinsic getCopyImpl()
    {
/* !!!
        VarReferenceIntrinsic retVal;
        retVal = new VarReferenceIntrinsic(this.name, this.parentContext,
                                            this.lineno);
        retVal.varIndex = this.varIndex;
        retVal.global = this.global;
        retVal.globalContext = this.globalContext;
        return(retVal);
*/

        return(getRealVar().getCopy());
    } // getCopyImpl

    public int getStackIndex()
    {
        return(this.varIndex);
    } // getStackIndex

    public boolean isGlobal()
    {
        return(this.global);
    } // isGlobal

    public boolean isLocal()
    {
        return(this.global == false);
    } // isLocal

    protected Intrinsic getRealVar()
    {
//        _D("getRealVar", "varIndex of [" + this.name +
//                         "] == (" + this.varIndex + ").");

        Intrinsic retVal = null;

        if (this.global == true)
            retVal = this.globalContext.globalFromTopOfStack(this.varIndex);
        else
            retVal = this.globalContext.localFromTopOfStack(this.varIndex);

        return(retVal);
    } // getRealVar

    public void setValue(Intrinsic newVal) throws FlowException
    {
        getRealVar().setValue(newVal);
// !!!        parentContext.notifyVar
    } // setValue overload

    public void setValue(int newVal) throws FlowException
    {
        getRealVar().setValue(newVal);
    } // setValue overload

    public void setValue(double newVal) throws FlowException
    {
        getRealVar().setValue(newVal);
    } // setValue overload

    public void setValue(float newVal) throws FlowException
    {
        getRealVar().setValue(newVal);
    } // setValue overload

    public void setValue(char newVal) throws FlowException
    {
        getRealVar().setValue(newVal);
    } // setValue overload

    public void setValue(byte newVal) throws FlowException
    {
        getRealVar().setValue(newVal);
    } // setValue overload

    public void setValue(long newVal) throws FlowException
    {
        getRealVar().setValue(newVal);
    } // setValue overload

    public void setValue(short newVal) throws FlowException
    {
        getRealVar().setValue(newVal);
    } // setValue overload

    public void setValue(Object newVal) throws FlowException
    {
        getRealVar().setValue(newVal);
    } // setValue overload

    public void setValue(String newVal) throws FlowException
    {
        getRealVar().setValue(newVal);
    } // setValue overload

    public int getIntValue() throws FlowException
    {
        return(getRealVar().getIntValue());
    } // getValue overload

    public char getCharValue() throws FlowException
    {
        return(getRealVar().getCharValue());
    } // getValue overload

    public byte getByteValue() throws FlowException
    {
        return(getRealVar().getByteValue());
    } // getValue overload

    public long getLongValue() throws FlowException
    {
        return(getRealVar().getLongValue());
    } // getValue overload

    public short getShortValue() throws FlowException
    {
        return(getRealVar().getShortValue());
    } // getValue overload

    public String getStringValue() throws FlowException
    {
        return(getRealVar().getStringValue());
    } // getValue overload

    public float getFloatValue() throws FlowException
    {
        return(getRealVar().getFloatValue());
    } // getValue overload

    public double getDoubleValue() throws FlowException
    {
        return(getRealVar().getDoubleValue());
    } // getValue overload

    public boolean getBooleanValue() throws FlowException
    {
        return(getRealVar().getBooleanValue());
    } // getValue overload

    public Object getObjectValue() throws FlowException
    {
        return(getRealVar().getObjectValue());
    } // getValue overload

    public Intrinsic getIntrinsicValue() throws FlowException
    {
        return(getRealVar().getIntrinsicValue());
    } // getValue overload

    public int compare(Intrinsic intr) throws FlowException
    {
        return(getRealVar().compare(intr));
    } // compare

    public boolean isEqual(Intrinsic intr) throws FlowException
    {
        return(getRealVar().isEqual(intr));
    } // isEqual

    protected void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        String id = getIdentifier();

//        _D("linkImpl", "looking for variable named [" + id + "].");

        this.globalContext = glob;
        LogicContext rc = parentContext.checkScopesForSymbol(id);
        if (rc == null)
            ParseException._throw(TobyLanguage.NOT_VAR, this.lineno);
        this.global = (rc == glob);
        this.varIndex = rc.getSymbolIndex(id);
    } // link
} // VarReferenceIntrinsic

// end of VarReferenceIntrinsic.java ...

