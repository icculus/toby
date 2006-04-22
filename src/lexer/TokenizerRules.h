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

#ifndef _INCLUDE_TOKENIZERRULES_H_
#define _INCLUDE_TOKENIZERRULES_H_

#include "lexer/LexerRules.h"
#include "exceptions/IOException.h"
#include "io/Tokenizer.h"

/*
 *  Element lexer rules: a logical grouping of subrules.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class TokenizerRules : public LexerRules
{
public:
    TokenizerRules(bool _caseSensitive, const char *_whitespaceChars,
                   size_t _numWhitespaceChars, bool _ignoreWhitespace,
                   bool _ignoreNewlines, const char *_singleLineComment,
                   bool _ignoreSingleLineComments,
                   const char *_multiLineCommentStart,
                   const char *_multiLineCommentEnd,
                   bool _ignoreMultiLineComments, const char *_quoteChars,
                   size_t _numQuoteChars, bool _escaping, char _escapeChar,
                   bool _convertNumsToWords);

    virtual ~TokenizerRules(void);
    virtual bool testRules(Tokenizer *toker) throw (IOException *);

protected:
    bool caseSensitive;
    char *whitespaceChars;
    size_t numWhitespaceChars;
    bool ignoreWhitespace;
    bool ignoreNewlines;
    char *singleLineComment;
    bool ignoreSingleLineComments;
    char *multiLineCommentStart;
    char *multiLineCommentEnd;
    bool ignoreMultiLineComments;
    char *quoteChars;
    size_t numQuoteChars;
    bool escaping;
    char escapeChar;
    bool convertNumsToWords;
}; // class TokenizerRules

#endif // !defined _INCLUDE_TOKENIZERRULES_H_

// end of TokenizerRules.h ...

