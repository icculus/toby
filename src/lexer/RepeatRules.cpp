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

#include "lexer/RepeatRules.h"

RepeatRules::RepeatRules(int _minval, int _maxval,
                         size_t numKids, LexerRules **kids)
    : minRepeats(_minval),
      maxRepeats(_maxval),
      numChildren(numKids),
      children(kids)
{
} // Constructor


RepeatRules::~RepeatRules(void)
{
    deleteRuleList(numChildren, children);
} // Destructor


bool RepeatRules::testRules(Tokenizer *toker) throw (IOException *)
{
    int repeatCount = 0;

    while (true)
    {
        for (size_t i = 0; i < numChildren; i++)
        {
            if (children[i]->testRules(toker) == false)
            {
                if (repeatCount < minRepeats)
                    return(false);

                if ((maxRepeats > 0) && (repeatCount > maxRepeats))
                    return(false);

                return(true);
            } // if
        } // for

        repeatCount++;
    } // while

    return(false);  // Shouldn't ever hit this point.
} // RepeatRules::testRules


bool RepeatRules::resolve(LexerRules *langRules)
{
    for (size_t i = 0; i < numChildren; i++)
    {
        if (children[i]->resolve(langRules) == false)
            return(false);
    } // for

    return(true);
} // RepeatRules::resolve

// end of RepeatRules.cpp ...

