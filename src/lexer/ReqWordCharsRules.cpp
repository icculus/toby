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

#include "lexer/ReqWordCharsRules.h"

ReqWordCharsRules::ReqWordCharsRules(size_t numKids, LexerRules **kids)
    : numChildren(numKids),
      children(kids)
{
} // Constructor


ReqWordCharsRules::~ReqWordCharsRules(void)
{
    deleteRuleList(numChildren, children);
} // Destructor


bool ReqWordCharsRules::testRules(Tokenizer *toker) throw (IOException *)
{
    if (toker->nextToken() == Tokenizer::TT_WORD)
    {
        size_t i;
        for (i = 0; i < numChildren; i++)
        {
            if (children[i]->testRules(toker) == false)
                break;
        } // while

        if (i == numChildren)  // passed all chars.
            return(true);
    } // if

    toker->pushBack();
    return(false);
} // ReqWordCharsRules::testRules


bool ReqWordCharsRules::resolve(LexerRules *langRules)
{
    for (size_t i = 0; i < numChildren; i++)
    {
        if (children[i]->resolve(langRules) == false)
            return(false);
    } // for

    return(true);
} // ReqWordCharsRules::resolve

// end of ReqWordCharsRules.cpp ...

