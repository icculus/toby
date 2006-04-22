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

#ifndef _INCLUDE_HALTEXCEPTION_H_
#define _INCLUDE_HALTEXCEPTION_H_

#include "exceptions/FlowException.h"


/*
 *  HaltExceptions are thrown when the user requests an early end to
 *   interpretation by hitting ESC or selecting the "STOP" menu item
 *   or whatnot. This allows for clean termination of the interpreter at
 *   any time.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class HaltException : public FlowException
{
public:
    static HaltException *getInstance(void)
    {
        return(new HaltException());  // !!!
    } // HaltException::getInstance

    static void _throw(const char *err, const char *p, int l)
                                                throw (HaltException *)
    {
        HaltException *e = getInstance();
        initThrow(e, err, p, l);
        throw(e);
    } // HaltException::_throw

    static void _throw(const char *err, const char *pName)
                                                throw (HaltException *)
    {
        HaltException *e = getInstance();
        initThrow(e, err, pName);
        throw(e);
    } // HaltException::_throw

    static void _throw(const char *err, int lineNum) throw (HaltException *)
    {
        HaltException *e = getInstance();
        initThrow(e, err, lineNum);
        throw(e);
    } // HaltException::_throw

    static void _throw(const char *err) throw (HaltException *)
    {
        HaltException *e = getInstance();
        initThrow(e, err);
        throw(e);
    } // HaltException::_throw

private:
    // no public construction.
    HaltException(void) : FlowException() {}
    HaltException(char *_msg) : FlowException(_msg) {}
}; // class HaltException

#endif  // !defined _INCLUDE_HALTEXCEPTION_H_

// end of HaltException.h ...


