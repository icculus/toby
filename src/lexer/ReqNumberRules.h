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

#ifndef _INCLUDE_REQNUMBERRULES_H_
#define _INCLUDE_REQNUMBERRULES_H_

#include "lexer/LexerRules.h"
#include "exceptions/IOException.h"
#include "io/Tokenizer.h"

/*
 *  ReqNumber lexer rule: succeeds if the next token is whitespace.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class ReqNumberRules : public LexerRules
{
public:
    ReqNumberRules(void);
    virtual ~ReqNumberRules(void);
    virtual bool testRules(Tokenizer *toker) throw (IOException *);
}; // class ReqNumberRules

#endif // !defined _INCLUDE_REQNUMBERRULES_H_

// end of ReqNumberRules.h ...

