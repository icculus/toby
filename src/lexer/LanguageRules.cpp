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

#include "lexer/LanguageRules.h"

LanguageRules::LanguageRules(const char *_name, const char *_ext,
                             LexerRules *_firstElem, LexerRules *_tokRule,
                             int numKids, LexerRules **kids)
    : numChildren(numKids),
      children(kids),
      firstElement(_firstElem),
      tokRule(_tokRule)
{
    assert(_name != NULL);
    name = new char[strlen(_name) + 1];
    strcpy(name, _name);

    assert(_ext != NULL);
    extension = new char[strlen(_ext) + 1];
    strcpy(extension, _ext);
} // Constructor


LanguageRules::~LanguageRules(void)
{
    if (children)
    {
        assert(numChildren > 0);
        for (int i = 0; i < numChildren; i++)
            delete children[i];

        delete[] children;
    } // if

    if (tokRule != NULL)
        delete tokRule;

    if (name != NULL)
        delete[] name;

    if (extension != NULL)
        delete[] extension;
} // Destructor


const char *LanguageRules::getName(void)
{
    return(name);  // READ. ONLY.
} // LanguageRules::getName


const char *LanguageRules::getFileExtension(void)
{
    return(extension);  // READ. ONLY.
} // LanguageRules::getName


    // just kick off the first element, and see how it does...
bool LanguageRules::testRules(Tokenizer *toker) throw (IOException *)
{
    if (tokRule != NULL)
    {
        if (!tokRule->testRules(toker))
            return(false);
    } // if

    if (firstElement == NULL)
        return(false);

    return(firstElement->testRules(toker));
} // testRules

// end of LanguageRules.cpp ...

