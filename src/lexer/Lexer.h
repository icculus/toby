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

#ifndef _INCLUDE_LEXER_H_
#define _INCLUDE_LEXER_H_

#include "util/TobyObject.h"
#include "util/TobyCollection.h"
#include "lexer/LanguageRules.h"

/*
 * Lexers for specific grammars register themselves with this class at
 *  runtime, and then can be located by name or standard file extension
 *  by code wanting a logic tree generated from source.
 *
 * The implementation of loadLanguages() is left up to a platform-specific
 *  implementation...frequently, this is one that loads lexing rules from
 *  an XML file, or one that builds a LexerRules tree from machine-generated
 *  C++ code. (Or NullLexer, for portability).
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class Lexer : public TobyObject
{
public:
    Lexer(void);
    virtual ~Lexer(void);
    virtual void registerLanguage(LanguageRules *lang);
    virtual LanguageRules *getLanguageRulesByName(const char *name);
    virtual LanguageRules *getLanguageRulesByExtension(const char *ext);

        // This gets filled in along with the platform-specific subclass...
    Lexer *getInstance(void);

        // This should build LexerRules trees for every available language,
        //  and pass each of them through registerLanguage().
    virtual void loadLanguages(void) = 0;

protected:
    TobyCollection *languages;
}; // class Lexer

#endif // !defined _INCLUDE_LEXER_H_

// end of Lexer.h ...

