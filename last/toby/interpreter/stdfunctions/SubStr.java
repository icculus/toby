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
import last.toby.util.TobyLanguage;
import last.toby.interpreter.*;
import last.toby.exceptions.*;

    /**
     *  !!! comment me!
     *  @author Ryan C. Gordon.
     */
public class SubStr extends StdFuncLogicContext
{
    protected final String getFuncNameImpl()
    {
        return("__$STDFUNC$__subStr");
    } // getFuncName

    protected final int getParamCountImpl()
    {
        return(3);
    } // getFuncName

    protected Intrinsic stdFuncExecuteImpl() throws FlowException
    {
        String str = globalContext.localFromTopOfStack(0).getStringValue();
        int startPos = globalContext.localFromTopOfStack(1).getIntValue();
        int len = globalContext.localFromTopOfStack(2).getIntValue();
        int endPos = startPos + len;
        int str_len = str.length();

        if ( (len < 0) ||
             (startPos < 0) ||
             (startPos >= str_len) ||
             (endPos >= str_len) )
        {
            ExecException._throw(TobyLanguage.OUT_OF_RANGE);
        } // if

        return(new StringIntrinsic(str.substring(startPos, endPos)));
    } // executeImpl
} // SubStr

// end of SubStr.java ...


