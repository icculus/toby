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
import last.toby.util.*;

    /**
     * !!! comment me.
     *
     *   @author Ryan C. Gordon (icculus@icculus.org).
     */
public class FunctionCallExprLogicContext extends ExpressionLogicContext
{
    protected String funcName = null;
    protected FunctionLogicContext funcToRun = null;
    protected GlobalLogicContext globalLogicContext = null;
    protected Intrinsic pushQueue[] = null;

    public FunctionCallExprLogicContext(int sourceLine, String funcName)
    {
        super(sourceLine);
        this.funcName = funcName;
    } // Constructor

    protected void freeResourcesImpl() {} // nothing to see here, folks.

    protected Intrinsic simplify() throws ParseException
    {
        // !!! This might be a good place to deal with inlining, at a later time.
        verifyLocked();

        for (int i = 0; i < children.length; i++)
        {
            ExpressionLogicContext elc = (ExpressionLogicContext) children[i];
            Intrinsic intr = elc.simplify();
            if (intr != null)
            {
                IntrinsicLogicContext ilc;
                ilc = new IntrinsicLogicContext(elc.getSourceLine(), intr);
                elc.freeResources(true);
                children[i] = ilc;
            } // if
        } // for
        return(null);
    } // simplify

    protected void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        verifyLocked();

//        _D("linkImpl", "Looking for function [" + funcName + "].");

        this.funcToRun = glob.findFunction(this.funcName);
        if (this.funcToRun == null)
            ParseException._throw(TobyLanguage.NOT_A_FUNC);

        int neededParamCount = this.funcToRun.getParamCount();

//        _D("linkImpl", "Need (" + neededParamCount + ") params. " +
//                       "Actually have (" + children.length + ").");

        if (neededParamCount != children.length)
            ParseException._throw(TobyLanguage.BADNUM_ARGS);

        pushQueue = new Intrinsic[children.length];

        this.globalLogicContext = glob;
    } // linkImpl

    protected Intrinsic executeImpl() throws FlowException
    {
        ExpressionLogicContext elc;
        Intrinsic retVal = null;
        int i;
        int max = children.length;
        int pushed = 0;
        Intrinsic var;

//        _D("executeImpl", "calling [" + funcName + "]...");

            // args get pushed last-one-first.
            //  the pushQueue is here since we can't touch local
            //  variables once we've started adding to the stack.
        for (i = max - 1; i >= 0; i--)
        {
            elc = (ExpressionLogicContext) children[i];
            var = elc.execute().getCopy();
            this.pushQueue[pushed] = var;
            pushed++;
        } // for

        try
        {
            pushed = 0;

            this.globalLogicContext.pushStackFrame();
            for (i = 0; i < max; i++)
            {
                var = this.pushQueue[i];
//                _D("executeImpl", "pushing arg [" + var.toString() + "]...");
                this.globalLogicContext.pushLocal(var);
                pushed++;
            } // for
            retVal = funcToRun.execute();
        } // try

        finally
        {
            this.globalLogicContext.popMultipleLocals(pushed);
            this.globalLogicContext.popStackFrame();
        } // finally

        return(retVal);
    } // executeImpl
} // FunctionCallExprLogicContext

// end of FunctionCallExprLogicContext.java ...

