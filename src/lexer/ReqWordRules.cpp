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

#include "lexer/ReqWordRules.h"

ReqWordRules::ReqWordRules(const char *_word)
    : word(NULL)

{
    if (_word != NULL)
    {
        word = new char[strlen(_word) + 1];
        strcpy(word, _word);
    } // if
} // Constructor


ReqWordRules::~ReqWordRules(void)
{
    delete[] word;
} // Destructor


bool ReqWordRules::testRules(Tokenizer *toker) throw (IOException *)
{
    if (word == NULL)
        return(false);

    bool retval = toker->mustGetWord(word);
    if (!retval)
        toker->pushBack();

    return(retval);
} // ReqWordRules::testRules

// end of ReqWordRules.cpp ...

