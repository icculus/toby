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

#include "lexer/LexerRules.h"

void LexerRules::deleteRuleList(size_t numRules, LexerRules **rules)
{
    if (rules)
    {
        assert(numRules > 0);
        for (size_t i = 0; i < numRules; i++)
            delete rules[i];

        delete[] rules;
    } // if
} // LexerRules::deleteRuleList


static const char *emptyString(void)
{
    char *retval = new char[1];
    retval[0] = '\0';
    return((const char *) retval);
} // emptyString


const char *LexerRules::outputDeclarations(void)
{
    return(emptyString());
} // LexerRules::outputDeclarations


const char *LexerRules::outputDefinitions(void)
{
    return(emptyString());
} // LexerRules::outputDefinitions


const char *LexerRules::outputConstructor(void)
{
    assert(false);
    return(emptyString());
} // LexerRules::outputConstructor


const char *LexerRules::outputResolutions(void)
{
    return(emptyString());
} // LexerRules::outputResolutions


// end of LexerRules.cpp ...


