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

#ifndef _INCLUDE_REQCHARRULES_H_
#define _INCLUDE_REQCHARRULES_H_

#include <ctype.h>
#include "lexer/LexerRules.h"
#include "exceptions/IOException.h"
#include "io/Tokenizer.h"

/*
 *  ReqChar lexer rule: succeeds if the next char in the current word token
 *   matches within the specified range of chars.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class ReqCharRules : public LexerRules
{
public:
    typedef enum
    {
        CTYPE_ALPHANUMERIC,
        CTYPE_LETTER,
        CTYPE_NUMBER,
        CTYPE_SPECIFIC,
        CTYPE_END
    } chartype;

    ReqCharRules(ReqCharRules::chartype _ctype, char _specific);
    virtual ~ReqCharRules(void);
    virtual bool testRules(Tokenizer *toker) throw (IOException *);

protected:
    bool (*compareFunc)(char ch, char specific);
    char specific;
}; // class ReqCharRules

#endif // !defined _INCLUDE_REQCHARRULES_H_

// end of ReqCharRules.h ...

