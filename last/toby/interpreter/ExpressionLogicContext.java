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
public abstract class ExpressionLogicContext extends LogicContext
{
    public ExpressionLogicContext(int sourceLine)
    {
        super(sourceLine);
    } // Constructor

        /**
         * To speed up interpretation, this method is called at link time.
         *  The expression tree is recursed, looking for constant operands.
         *  These hunks of the expression are evaluated now, and ripped out
         *  of the tree, replaced by the result of the evaluation.
         *
         *   For example, if we had the expression:
         *
         *    x = 5 * 3 + myVariable;
         *
         *   Then there's really no need to figure out what the result of
         *    (5 * 3) is every time; it's always going to be 15. So, after
         *    this method has finished, the above result will go through
         *    interpretation as:
         *
         *    x = 15 + myVariable;
         *
         *  Simple and effective optimizations are your friends.
         */
    protected abstract Intrinsic simplify() throws ParseException;
} // ExpressionLogicContext

// end of ExpressionLogicContext.java ...

