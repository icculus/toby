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

#ifndef _INCLUDE_FLOWEXCEPTION_H_
#define _INCLUDE_FLOWEXCEPTION_H_

#include "exceptions/TobyException.h"


// !!!
#define LINENUM_UNDEFINED  -1

/*
 * FlowExceptions are anything that interrupts program flow: this includes
 *  parsing/linking errors, runtime errors, and user-initiated aborts, plus
 *  other forms of normal logic interruptions like return statements.
 *
 * This is a generic base class. Coders should look to the subclasses.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class FlowException : public TobyException
{
public:
    // !!! public final static int LINENUM_UNDEFINED = -1;

        // !!! These should be pooled...
    virtual ~FlowException(void)
    {
        if (procName != NULL)
            delete procName;
    } // Destructor


    const char *getProcName(void)
    {
        return(procName->c_str());
    } // FlowException::getProcName


    int getSourceLine(void)
    {
        return(exceptionLine);
    } // FlowException::getSourceLine


protected:
    // no public construction.
    FlowException(void) : TobyException(),
            procName(new TobyString()), exceptionLine(LINENUM_UNDEFINED)
    {
    } // Constructor


    FlowException(char *_msg) : TobyException(_msg),
            procName(new TobyString()), exceptionLine(LINENUM_UNDEFINED)
    {
    } // Constructor


    static void initThrow(FlowException *e, const char *err,
                           const char *pName, int lineNum)
    {
        e->msg->assignCStr(err);
        e->procName->assignCStr(pName);
        e->exceptionLine = lineNum;
    } // FlowException::initThrow (overloaded; the works.)


    static void initThrow(FlowException *e, const char *err, const char *pName)
    {
        initThrow(e, err, pName, LINENUM_UNDEFINED);
    } // FlowException::initThrow (overloaded; no line number.)


    static void initThrow(FlowException *e, const char *err, int lineNum)
    {
        initThrow(e, err, NULL, lineNum);
    } // FlowException::initThrow (overloaded; just line number.)


    static void initThrow(FlowException *e, const char *err)
    {
        initThrow(e, err, NULL, LINENUM_UNDEFINED);
    } // FlowException::initThrow (overloaded; no line number or procedure.)

private:
    TobyString *procName;
    int exceptionLine;
}; // class FlowException

#endif  // !defined _INCLUDE_FLOWEXCEPTION_H_

// end of FlowException.h ...


