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

#include "lexer/ReqCharRules.h"


static bool compareAlphanumeric(char ch, char specific)
{
    return(isalnum(ch));
} // compareAlphanumeric


static bool compareLetter(char ch, char specific)
{
    return(isalpha(ch));
} // compareLetter


static bool compareNumber(char ch, char specific)
{
    return(isdigit(ch));
} // compareNumber


static bool compareSpecific(char ch, char specific)
{
    return(ch == specific);
} // compareSpecific


static bool compareEnd(char ch, char specific)
{
    return(ch == -1);
} // compareSpecific


ReqCharRules::ReqCharRules(ReqCharRules::chartype _ctype, char _specific)
    : compareFunc(NULL),
      specific(_specific)
      
{
    switch (_ctype)
    {
        case CTYPE_ALPHANUMERIC:
            compareFunc = compareAlphanumeric;
            break;
        case CTYPE_LETTER:
            compareFunc = compareLetter;
            break;
        case CTYPE_NUMBER:
            compareFunc = compareNumber;
            break;
        case CTYPE_SPECIFIC:
            compareFunc = compareSpecific;
            break;
        case CTYPE_END:
            compareFunc = compareEnd;
            break;
        default:
            assert(false);
    } // switch
} // Constructor


ReqCharRules::~ReqCharRules(void)
{
} // Destructor


bool ReqCharRules::testRules(Tokenizer *toker) throw (IOException *)
{
    if (compareFunc(toker->getNextWordChar(), specific))
        return(true);

    toker->pushBackWordChar();
    return(false);
} // ReqCharRules::testRules

// end of ReqCharRules.cpp ...

