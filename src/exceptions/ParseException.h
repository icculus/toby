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

#ifndef _INCLUDE_PARSEEXCEPTION_H_
#define _INCLUDE_PARSEEXCEPTION_H_

#include "exceptions/FlowException.h"


/*
 *  ParseExceptions are thrown when there is a syntax error or some other
 *   incorrect language construct is encountered at parse time. This
 *   exception is also used to signal an unresolved symbol during the "link"
 *   stage.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class ParseException : public FlowException
{
public:
    static ParseException *getInstance(void)
    {
        return(new ParseException());  // !!!
    } // ParseException::getInstance

    static void _throw(const char *err, const char *p, int l)
                                                throw (ParseException *)
    {
        ParseException *e = getInstance();
        initThrow(e, err, p, l);
        throw(e);
    } // ParseException::_throw

    static void _throw(const char *err, const char *pName)
                                                throw (ParseException *)
    {
        ParseException *e = getInstance();
        initThrow(e, err, pName);
        throw(e);
    } // ParseException::_throw

    static void _throw(const char *err, int lineNum) throw (ParseException *)
    {
        ParseException *e = getInstance();
        initThrow(e, err, lineNum);
        throw(e);
    } // ParseException::_throw

    static void _throw(const char *err) throw (ParseException *)
    {
        ParseException *e = getInstance();
        initThrow(e, err);
        throw(e);
    } // ParseException::_throw

private:
    // no public construction.
    ParseException(void) : FlowException() {}
    ParseException(char *_msg) : FlowException(_msg) {}
}; // class ParseException

#endif  // !defined _INCLUDE_PARSEEXCEPTION_H_

// end of ParseException.h ...


