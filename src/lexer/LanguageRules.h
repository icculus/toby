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

#ifndef _INCLUDE_LANGUAGERULES_H_
#define _INCLUDE_LANGUAGERULES_H_

#include "lexer/LexerRules.h"
#include "exceptions/IOException.h"
#include "io/Tokenizer.h"

/*
 *  Language lexer rules: a logical grouping of a logical
 *   grouping of subrules. These are the toplevel rules for
 *   a language's grammar.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class LanguageRules : public LexerRules
{
public:
        // The (kids) array is assumed to be built specifically for
        //  a given instance (and either by computer-generated code or
        //  by the XML parser), so we don't make a copy, and just keep the
        //  original reference. This reference will be deleted at destruction,
        //  so don't delete it yourself!
    LanguageRules(const char *_name, const char *_ext,
                  LexerRules *_firstElem, LexerRules *_tokRule,
                  size_t numKids, LexerRules **kids);
    virtual ~LanguageRules(void);

    const char *getName(void);
    const char *getFileExtension(void);

    virtual bool testRules(Tokenizer *toker) throw (IOException *);
    virtual bool resolve(LexerRules *langRules);

protected:
    size_t numChildren;
    LexerRules **children;
    LexerRules *firstElement;
    LexerRules *tokRule;
    char *name;
    char *extension;
}; // class LanguageRules

#endif // !defined _INCLUDE_LANGUAGERULES_H_

// end of LanguageRules.h ...

