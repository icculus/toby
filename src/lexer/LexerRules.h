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

#ifndef _INCLUDE_LEXERRULES_H_
#define _INCLUDE_LEXERRULES_H_

#include "util/TobyObject.h"
#include "io/Tokenizer.h"

/*
 * The base class for Lexer rules. A deviritive class, after construction,
 *  can test the current state of a Tokenizer based on rules specific to that
 *  derivitive of LexerRules.
 *
 * The subclass can optionally be able to produce the the C++ code needed to
 *  rebuild that instance of the rules. Here be dragons.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class LexerRules : public TobyObject
{
public:
        // Test the state of (toker) based on rules the subclass provides.
        //  Return (false) if the state doesn't qualify based on those rules,
        //  (true) if it does. It should update the state of (toker) as
        //  appropriate (for example, swallow tokens that match, rollback
        //  tokens if they don't, etc...)
    virtual bool testRules(Tokenizer *toker) throw (IOException *) = 0;



                /*
                 * ...this is the part with the dragons...
                 */



        // This should return a null-terminated string that
        //  contains the C++ code needed to construct said object.
        // This is optional, and is used for any declarations that need to
        //  be at the top of the source file. If not needed, return NULL.
        // The caller will delete[] the return value (if not NULL).
        //  An example return value is
        //   "static LexerRules *element_WHILESTATEMENT = NULL;\n"
        //   "static LexerRules *build_element_WHILESTATEMENT(void);\n"
        //  That's one string, btw.
    virtual const char *outputDeclarations(void) { return(NULL); }

        // This should return a null-terminated string that
        //  contains the C++ code needed to construct said object.
        // This is optional, and is used to define anything that was
        //  declared at the top of the source file with outputDeclarations().
        // If not needed, return NULL.
        // The caller will delete[] the return value (if not NULL).
    virtual const char *outputDefinitions(void) { return(NULL); }

        // This should return a null-terminated string that
        //  contains the C++ code needed to construct said object.
        // This is NOT optional, and is used for the creation of a object's
        //  instance.
        // The caller will delete[] the return value.
        //  An example return value is "new MyClass(\"string\", 1)" or
        //  "build_element_WHILESTATEMENT()" ...
        //  Note the lack of assignment or semicolon.
    virtual const char *outputConstructor(void) { assert(false); return(NULL); }

        // This should return a null-terminated string that
        //  contains the C++ code needed to construct said object.
        // This is optional, and is used for any resolutions that need to
        //  be accomplished after all the rules have been built.
        // If not needed, return NULL.
        // The caller will delete[] the return value (if not NULL).
        //  An example return value is
        //   "requireElement_12->element = element_WHILESTATEMENT;\n"
        //  This one's got a semicolon. Confused, yet?
    virtual const char *outputResolutions(void) { return(NULL); }

protected:
        // Delete (numRules) rules from the (rules) array, if the
        //  array isn't null. Then, delete[] (rules). This is a convenience
        //  function for subclasses.
    virtual void deleteRuleList(size_t numRules, LexerRules **rules);
}; // class LexerRules

#endif // !defined _INCLUDE_LEXERRULES_H_

// end of LexerRules.h ...

