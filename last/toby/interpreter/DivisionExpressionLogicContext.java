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
     *  !!! comment me!
     *  @author Ryan C. Gordon.
     */
public class DivisionExpressionLogicContext extends BinaryExpressionLogicContext
{
    public DivisionExpressionLogicContext(int sourceLine)
    {
        super(sourceLine);
    } // Constructor

    protected Intrinsic executeImpl(Intrinsic operand0,
                                    Intrinsic operand1)
                                        throws FlowException
    {
        double o0 = operand0.getDoubleValue();
        double o1 = operand1.getDoubleValue();

        if (o1 == 0.00)
            ExecException._throw(TobyLanguage.DIV_BY_ZERO);

//        _D("executeImpl", "dividing [" + o0 + "] by [" + o1 + "]...");

        Intrinsic retVal = new NumberIntrinsic();
        retVal.setValue(o0 / o1);
        return(retVal);
    } // executeImpl
} // DivisionExpressionLogicContext

// end of DivisionExpressionLogicContext.java ...

