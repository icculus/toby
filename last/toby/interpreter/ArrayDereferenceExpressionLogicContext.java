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

// !!! Dang...that's a long class name.
public class ArrayDereferenceExpressionLogicContext
                                        extends BinaryExpressionLogicContext
{
    // children[0] == array to dereference.
    // children[1] == index of element in array.

    private boolean autoCast = false;

    public ArrayDereferenceExpressionLogicContext(int sourceLine,
                                                  boolean autoCast)
    {
        super(sourceLine);
        this.autoCast = autoCast;
    } // Constructor


        /* !!! lose this if the below !!! can't be resolved.
    protected void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        super.linkImpl(glob);

           !!! children[0] will never be an ArrayIntrinsic, since, at best, it
           !!!  will be an IntrinsicLogicContext wrapper.
        
        if ( ((children[0] instanceof ArrayIntrinsic) == false) &&
             ((children[0] instanceof ArrayDereferenceExpressionLogicContext) == false) )
        {
            ParseException._throw(TobyLanguage.NOT_AN_ARRAY);
        } // if
    } // linkImpl
        */

    protected Intrinsic executeImpl(Intrinsic operand1, Intrinsic operand2)
                                                 throws FlowException
    {
            // the getIntrinsicValue() call will pull the array out of
            //  a VarReferenceIntrinsic, it is wrapped as such.
        ArrayIntrinsic intr = (ArrayIntrinsic) operand1.getIntrinsicValue();
        int index = 0;

        if (this.autoCast == true)
            index = operand2.getIntValue();
        else
        {
            double d = operand2.getDoubleValue();
            index = (int) d;
            if ( (d - ((double) index)) != 0.0 )
                ExecException._throw(TobyLanguage.NOT_WHOLE_NUM);
        } // else

        return(intr.getElement(index));
    } // executeImpl

} // ArrayDereferenceExpressionLogicContext

// end of ArrayDereferenceExpressionLogicContext.java ...

