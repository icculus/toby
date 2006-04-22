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

#ifndef _INCLUDE_PARSER_H_
#define _INCLUDE_PARSER_H_

#include <string.h>
#include "util/TobyObject.h"
#include "util/TobyCollection.h"
#include "exceptions/ParseException.h"
#include "util/TobyClock.h"
#include "io/TobyReader.h"

// !!!
typedef void GlobalLogicContext;


/*
 * This abstract base class supplies the interface for various language
 *  parsers to inplement. Examples of subclasses are TobyParser, LogoParser,
 *  etc. The parser's goal is to take a stream of tokens and build it into
 *  a tree of LogicContexts, of which can then be executed as a complete
 *  program.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class Parser : public TobyObject
{
public:

    Parser(void);
    virtual ~Parser(void);

        // A simple bubble sort to alphabetize an array of strings.
        //  returns the same array that was passed in, but the contents
        //  of the array has been shifted about.
    static TobyCollection *alphabetizeArrayElements(TobyCollection *array);


        // Find out if (forThis) is in (table). The strings contained in table
        //  must be indexed in alphabetic order. This makes the searches
        //  faster.
    static int searchAlphabeticArray(TobyCollection *table, const char *str);


        // Return index of (forThis) in (table), or (-1) if it isn't there.
    static int searchArray(TobyCollection *table, const char *forThis);


        // Find out if (forThis) is in (table). (forThis) must not be just
        //  an identical string, but an identical pointer.
    static int searchArrayForReference(TobyCollection *table, const char *str);


        // Call this to parse a collection of code into a LogicContext tree.
        //  (code) may be an entire program or just a single procedure or
        //  whatever else. DO NOT OVERRIDE THIS! Implement parseCodeImpl(),
        //  instead, as this function does some setup before calling that
        //  for the actual parsing.
        //
        //   params: code   The code to parse. This is usually a StringReader,
        //                  but may come directly from disk or network.
        //           screen An object representing the screen. This is
        //                  language/platform, etc dependent, but in the
        //                  usual/intended case, it's an implementor of
        //                  last.toby.turtlespace.TurtleSpace.
        //  returns: A new ModuleLogicContext containing procedures, global
        //            vars, and other attributes resulting from the parsing of
        //            (code).
        //   throws: ParseException on syntax errors, etc...
    GlobalLogicContext *parseCode(TobyReader *code, TobyObject *screen)
                                                throw (ParseException *);

        // Get the total time that was spent during the last parsing,
        //  in milliseconds.
    toby_uint32 getTotalParseTime(void);


         // Get the total time that was spent during the last linking,
         //  in milliseconds.
    toby_uint32 getTotalLinkTime(void);


         // This is the entry point for a given parser to do its work.
         //  It returns a GlobalLogicContext, which is the root of a tree
         //  containing the logic structure of the whole program, ready for
         //  linking and then running.
    virtual GlobalLogicContext *parseCodeImpl(TobyReader *code)
                                                throw (ParseException *) = 0;

protected:
    toby_uint32 totalParseTime;
    toby_uint32 totalLinkTime;
}; // class Parser

#endif  // !defined _INCLUDE_PARSER_H_

// end of Parser.h ...

