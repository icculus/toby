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

    /**
     *  Toby, unlike Java, signifies the end of for loops differently than
     *   while loops. To aid in the distinction, here's a do-nothing extension
     *   to LoopLogicContext, so the instanceof value of this class can fork
     *   into something different than other loops. Unless your parser
     *   explicitly needs to verify the type of loop in question, you may skip
     *   this subclass for its parent LoopLogicContext.
     *
     *  @author Ryan C. Gordon.
     */
public class ForLoopLogicContext extends LoopLogicContext
{
    public ForLoopLogicContext(int sourceLine,
                               ExpressionLogicContext startExpr,
                               ExpressionLogicContext endExpr,
                               ExpressionLogicContext stepExpr)
    {
        super(sourceLine, startExpr, endExpr, stepExpr);
    } // Constructor

} // ForLoopLogicContext

// end of ForLoopLogicContext.java ...

