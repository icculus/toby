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

package last.toby.interpreter.stdfunctions;

import last.toby.turtlespace.TurtleSpace;
import last.toby.interpreter.*;
import last.toby.exceptions.*;
import last.toby.util.*;

    /**
     *  !!! comment me!
     *  @author Ryan C. Gordon.
     */
public abstract class StdFuncLogicContext extends FunctionLogicContext
{
    protected TurtleSpace tspace = null;
    protected GlobalLogicContext globalContext = null;

    public StdFuncLogicContext()
    {
        super(FlowException.LINENUM_UNDEFINED, null);
        this.functionName = getFuncName();
    } // Constructor

    public void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        this.globalContext = glob;
        Object obj = glob.getScreen();

        if ((obj instanceof TurtleSpace) == false)
            ParseException._throw(TobyLanguage.INTERNAL_ERROR);

        tspace = (TurtleSpace) obj;
    } // linkImpl

    public final void addParameter(Intrinsic newParam)
    {
        throw(new IllegalArgumentException(TobyLanguage.INTERNAL_ERROR));
    } // addParameter (override from superclass.)

    public final void addLocal(Intrinsic newParam)
    {
        throw(new IllegalArgumentException(TobyLanguage.INTERNAL_ERROR));
    } // addLocal (override from superclass.)

    public final String getFuncName()
    {
        return(getFuncNameImpl());
    } // getFuncName (override from superclass.)

    public final int getParamCount()
    {
        return(getParamCountImpl());
    } // getParamCount (override from superclass.)

    public final Intrinsic executeImpl() throws FlowException
    {
        return(stdFuncExecuteImpl());
    } // executeImpl

        /**
         * This is here just to remind every StdFunction that it
         *  needs to have a unique, static name.
         *
         *  @return Standard function's name.
         */
    protected abstract String getFuncNameImpl();

        /**
         * This is here just to remind every StdFunction that it
         *  needs to have a unique, static param count.
         *
         *  @return Standard function's name.
         */
    protected abstract int getParamCountImpl();

    protected abstract Intrinsic stdFuncExecuteImpl() throws FlowException;
} // StdFuncLogicContext

// end of StdFuncLogicContext.java ...

