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

    /**
     *  !!! comment me!
     *
     *  @author Ryan C. Gordon.
     */
public class LoopLogicContext extends LogicContext
{
    public ExpressionLogicContext startExpr;
    public ExpressionLogicContext endExpr;
    public ExpressionLogicContext stepExpr;

    public LoopLogicContext(int sourceLine,
                            ExpressionLogicContext startExpr,
                            ExpressionLogicContext endExpr,
                            ExpressionLogicContext stepExpr)
    {
        super(sourceLine);
        this.startExpr = startExpr;
        this.endExpr = endExpr;
        this.stepExpr = stepExpr;
    } // Constructor


    protected void freeResourcesImpl()
    {
        if (startExpr != null)
            startExpr.freeResources(true);

        if (endExpr != null)
            endExpr.freeResources(true);

        if (stepExpr != null)
            stepExpr.freeResources(true);

        startExpr = endExpr = stepExpr = null;
    } // freeResourcesImpl


    protected void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        if (startExpr != null)
            startExpr.link(glob);

        if (endExpr != null)
            endExpr.link(glob);

        if (stepExpr != null)
            stepExpr.link(glob);
    } // linkImpl


    public Intrinsic executeImpl() throws FlowException
    {
        boolean compValue;
        int i;
        boolean getOut = false;

        if (startExpr != null)
            startExpr.execute();

        do
        {
            if (endExpr != null)
            {
                if ((boolean) endExpr.execute().getBooleanValue() == false)
                    getOut = true;
            } // if

            if (getOut == false)
            {
                defaultExecuteImpl();

//                _D("executeImpl", "stepExpr is " +
//                    ((stepExpr == null) ? "" : "NOT") + " null.");

                if (stepExpr != null)
                    stepExpr.execute();
            } // if
        } while (getOut == false);

        return(NothingIntrinsic.nothing);
    } // executeImpl

} // LoopLogicContext

// end of LoopLogicContext.java ...

