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

#ifndef _INCLUDE_ELEMENTRULES_H_
#define _INCLUDE_ELEMENTRULES_H_

#include "lexer/LexerRules.h"
#include "exceptions/IOException.h"
#include "io/Tokenizer.h"

/*
 *  Element lexer rules: a logical grouping of subrules.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class ElementRules : public LexerRules
{
public:
        // The (kids) array is assumed to be built specifically for
        //  a given instance (and either by computer-generated code or
        //  by the XML parser), so we don't make a copy, and just keep the
        //  original reference. This reference will be deleted at destruction,
        //  so don't delete it yourself!
    ElementRules(int _rollback, size_t numKids, LexerRules **kids);
    virtual ~ElementRules(void);
    virtual bool testRules(Tokenizer *toker) throw (IOException *);

protected:
    int rollback;
    size_t numChildren;
    LexerRules **children;
}; // class ElementRules

#endif // !defined _INCLUDE_ELEMENTRULES_H_

// end of ElementRules.h ...

