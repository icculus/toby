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

#include "lexer/ReqSingleLineCommentRules.h"

ReqSingleLineCommentRules::ReqSingleLineCommentRules(void)
{
} // Constructor


ReqSingleLineCommentRules::~ReqSingleLineCommentRules(void)
{
} // Destructor


bool ReqSingleLineCommentRules::testRules(Tokenizer *toker) throw (IOException *)
{
    if (toker->nextToken() == Tokenizer::TT_SINGLELINECOMMENT)
        return(true);

    toker->pushBack();
    return(false);
} // ReqSingleLineCommentRules::testRules

// end of ReqSingleLineCommentRules.cpp ...

