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

#ifndef _INCLUDE_EXECEXCEPTION_H_
#define _INCLUDE_EXECEXCEPTION_H_

#include "exceptions/FlowException.h"


/*
 *  ExecExceptions are for interpretation errors at runtime, as opposed to
 *   parsetime errors.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class ExecException : public FlowException
{
public:
    static ExecException *getInstance(void)
    {
        return(new ExecException());  // !!!
    } // ExecException::getInstance

    static void _throw(const char *err, const char *p, int l)
                                                throw (ExecException *)
    {
        ExecException *e = getInstance();
        initThrow(e, err, p, l);
        throw(e);
    } // ExecException::_throw

    static void _throw(const char *err, const char *pName)
                                                throw (ExecException *)
    {
        ExecException *e = getInstance();
        initThrow(e, err, pName);
        throw(e);
    } // ExecException::_throw

    static void _throw(const char *err, int lineNum) throw (ExecException *)
    {
        ExecException *e = getInstance();
        initThrow(e, err, lineNum);
        throw(e);
    } // ExecException::_throw

    static void _throw(const char *err) throw (ExecException *)
    {
        ExecException *e = getInstance();
        initThrow(e, err);
        throw(e);
    } // ExecException::_throw

private:
    // no public construction.
    ExecException(void) : FlowException() {}
    ExecException(char *_msg) : FlowException(_msg) {}
}; // class ExecException

#endif  // !defined _INCLUDE_EXECEXCEPTION_H_

// end of ExecException.h ...


