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

#include "lexer/TokenizerRules.h"

// !!! This is some serious ugly right here.
TokenizerRules::TokenizerRules(bool _caseSensitive,
                   const char *_whitespaceChars,
                   size_t _numWhitespaceChars, bool _ignoreWhitespace,
                   bool _ignoreNewlines, const char *_singleLineComment,
                   bool _ignoreSingleLineComments,
                   const char *_multiLineCommentStart,
                   const char *_multiLineCommentEnd,
                   bool _ignoreMultiLineComments, const char *_quoteChars,
                   size_t _numQuoteChars, bool _escaping, char _escapeChar,
                   bool _convertNumsToWords)
    : caseSensitive(_caseSensitive),
      whitespaceChars(NULL),
      numWhitespaceChars(_numWhitespaceChars),
      ignoreWhitespace(_ignoreWhitespace),
      ignoreNewlines(_ignoreNewlines),
      singleLineComment(NULL),
      ignoreSingleLineComments(_ignoreSingleLineComments),
      multiLineCommentStart(NULL),
      multiLineCommentEnd(NULL),
      ignoreMultiLineComments(_ignoreMultiLineComments),
      quoteChars(NULL),
      numQuoteChars(_numQuoteChars),
      escaping(_escaping),
      escapeChar(_escapeChar),
      convertNumsToWords(_convertNumsToWords)
{
    if ((numWhitespaceChars > 0) && (_whitespaceChars != NULL))
    {
        whitespaceChars = new char[numWhitespaceChars];
        memcpy(whitespaceChars, _whitespaceChars, numWhitespaceChars);
    } // if

    if (_singleLineComment != NULL)
    {
        singleLineComment = new char[strlen(_singleLineComment) + 1];
        strcpy(singleLineComment, _singleLineComment);
    } // if

    if (_multiLineCommentStart != NULL)
    {
        multiLineCommentStart = new char[strlen(_multiLineCommentStart) + 1];
        strcpy(multiLineCommentStart, _multiLineCommentStart);
    } // if

    if (_multiLineCommentEnd != NULL)
    {
        multiLineCommentEnd = new char[strlen(_multiLineCommentEnd) + 1];
        strcpy(multiLineCommentEnd, _multiLineCommentEnd);
    } // if

    if ((numQuoteChars > 0) && (_quoteChars != NULL))
    {
        quoteChars = new char[numQuoteChars];
        memcpy(quoteChars, _quoteChars, _numQuoteChars);
    } // if
} // Constructor


TokenizerRules::~TokenizerRules(void)
{
    if (whitespaceChars != NULL)
        delete[] whitespaceChars;

    if (singleLineComment != NULL)
        delete[] singleLineComment;

    if (multiLineCommentStart != NULL)
        delete[] multiLineCommentStart;

    if (multiLineCommentEnd != NULL)
        delete[] multiLineCommentEnd;

    if (quoteChars != NULL)
        delete[] quoteChars;
} // Destructor


bool TokenizerRules::testRules(Tokenizer *toker) throw (IOException *)
{
    toker->setCaseSensitive(caseSensitive);
    toker->setWhitespaceChars(whitespaceChars, numWhitespaceChars);
    toker->setIgnoreWhitespace(ignoreWhitespace);
    toker->setIgnoreNewlines(ignoreNewlines);
    toker->setSingleLineComment(singleLineComment);
    toker->setIgnoreSingleLineComments(ignoreSingleLineComments);
    toker->setMultiLineComment(multiLineCommentStart, multiLineCommentEnd);
    toker->setIgnoreMultiLineComments(ignoreMultiLineComments);
    toker->setQuoteChars(quoteChars, numQuoteChars);
    toker->setEscaping(escaping);
    toker->setEscapeChar(escapeChar);
    toker->setNumbersAreWords(convertNumsToWords);

    return(true);
} // testRules

// end of TokenizerRules.cpp ...

