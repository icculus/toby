/*
 * TOBY -- An abstract interpreter engine and system for learning.
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

#ifndef _INCLUDE_RETURNEXCEPTION_H_
#define _INCLUDE_RETURNEXCEPTION_H_

#include "exceptions/FlowException.h"


/*
 *  ReturnExceptions are used to break the logic flow when a RETURN statement
 *   is hit. FunctionLogicContexts explicitly listen for this and pass control
 *   back to their parent contexts when such an exception is caught.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class ReturnException : public FlowException
{
public:
    static ReturnException *getInstance(void)
    {
        return(new ReturnException());  // !!!
    } // ReturnException::getInstance

    public TobyObject *getReturnValue(void)
    {
        return(returnValue);
    } // ReturnException::getReturnValue

    static void _throw(TobyObject *value) throw (ReturnException *)
    {
        ReturnException *e = getInstance();
        e->returnValue = value;
        initThrow(e, "");
        throw(e);
    } // ReturnException::_throw

private:
    // no public construction.
    ReturnException(void) : FlowException(), returnValue(NULL) {}
    ReturnException(char *_msg) : FlowException(_msg), returnValue(NULL) {}
    TobyObject *returnValue;
}; // class ReturnException

#endif  // !defined _INCLUDE_RETURNEXCEPTION_H_

// end of ReturnException.h ...


