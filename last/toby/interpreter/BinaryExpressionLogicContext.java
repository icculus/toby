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
     *  !!! comment me!
     *  @author Ryan C. Gordon.
     */
public abstract class BinaryExpressionLogicContext extends ExpressionLogicContext
{
    public BinaryExpressionLogicContext(int sourceLine)
    {
        super(sourceLine);
    } // Constructor

    protected Intrinsic simplify() throws ParseException
    {
return(null);  // !!! FIXME!
/*
        Intrinsic retVal = null;

        verifyLocked();
        if (children.length != 2)
            ParseException._throw(TobyLanguage.INTERNAL_ERROR);

        children[0] = simplifyAChild((LogicContext) children[0]);
        children[1] = simplifyAChild((LogicContext) children[1]);

            // Our children may have changed at this point...
            //  ...see if we can simplify ourself, now...

        if ((children[0] instanceof IntrinsicLogicContext) &&
            (children[1] instanceof IntrinsicLogicContext))
        {
            try
            {
                Intrinsic i1 = ((IntrinsicLogicContext) children[0]).execute();
                Intrinsic i2 = ((IntrinsicLogicContext) children[1]).execute();

                if ((i1.isConstantValue()) && (i2.isConstantValue()))
                    retVal = executeImpl(i1, i2);
            } // try
            catch (FlowException e)
            {
                ParseException._throw(e.msg, e.procName, e.exceptionLine);
            } // catch
        } // if

        return(retVal);
*/
    } // simplify


    private LogicContext simplifyAChild(LogicContext lc) throws ParseException
    {
        LogicContext retVal = lc;

        if (lc instanceof ExpressionLogicContext)
        {
            Intrinsic intr = ((ExpressionLogicContext) lc).simplify();
            if (intr != null)
            {
                retVal = new IntrinsicLogicContext(lc.getSourceLine(), intr);
                retVal.setParent(this);
            } // if
        } // if

        return(retVal);
    } // simplifyAChild

    protected void freeResourcesImpl() {}

    protected void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        if (children.length != 2)
            ParseException._throw(TobyLanguage.INTERNAL_ERROR);

            // !!! this should move to the superclass.
        if ( (parent == null) ||  // root of expression tree?
             ((parent instanceof ExpressionLogicContext) == false) )
            simplify();
    } // link


    protected final Intrinsic executeImpl() throws FlowException
    {
        Intrinsic operand1 = ((LogicContext) children[0]).execute();
        Intrinsic operand2 = ((LogicContext) children[1]).execute();
        return(executeImpl(operand1, operand2));
    } // execute

    protected abstract Intrinsic executeImpl(Intrinsic operand1,
                                             Intrinsic operand2)
                                                 throws FlowException;
} // BinaryExpressionLogicContext

// end of BinaryExpressionLogicContext.java ...

