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

#include "io/Tokenizer.h"

static char *replaceString(char **origString, const char *newString)
{
    if (*origString != NULL)
        delete[] *origString;

    if (newString == NULL)
        *origString = NULL;
    else
    {
        int numChars = strlen(newString) + 1;
        *origString = new char[numChars];
        memcpy(origString, newString, numChars);
    } // else

    return(*origString);
} // replaceString


static char *replaceChars(char **origChars, const char *newChars,
                           int *numCharsOut, int numChars)
{
    assert(origChars != NULL);
    assert(numCharsOut != NULL);

    if (*origChars != NULL)
        delete[] *origChars;

    if (newChars == NULL)
    {
        *origChars = NULL;
        *numCharsOut = numChars;
    } // if
    else
    {
        *origChars = new char[numChars];
        memcpy(*origChars, newChars, numChars);
        *numCharsOut = numChars;
    } // else

    return(*origChars);
} // replaceChars


Tokenizer::Tokenizer(TobyReader *reader) :
    ttype(TT_NONE),
    str(NULL),
    num(0.0),
    in(reader),
    tokenBuffer(NULL),
    bufferAllocSize(0),
    bufferIndex(0),
    pushedBack(false),
    quoteChars(NULL),
    escapeChar('\\'),
    isEscaping(true),
    whitespaceChars(NULL),
    ignoreWhitespace(true),
    singleLineCommentStart(NULL),
    ignoreSingleLineComments(true),
    multiLineCommentStart(NULL),
    multiLineCommentEnd(NULL),
    ignoreMultiLineComments(true),
    convertNumbers(true),
    lineNum(0)
{
    assert(in != NULL);
    setQuoteChars("\"\'", 2);
    setWhitespaceChars("\t ", 2);
    setSingleLineComment("//");
    setMultiLineComment("/*", "*/");
} // Constructor


Tokenizer::~Tokenizer(void)
{
    delete in;

    if (str != NULL)
        delete[] str;

    if (tokenBuffer != NULL)
        delete[] tokenBuffer;

    if (quoteChars != NULL)
        delete[] quoteChars;

    if (whitespaceChars != NULL)
        delete[] whitespaceChars;

    if (singleLineCommentStart != NULL)
        delete[] singleLineCommentStart;

    if (multiLineCommentStart != NULL)
        delete[] multiLineCommentStart;

    if (multiLineCommentEnd != NULL)
        delete[] multiLineCommentEnd;
} // Destructor


void Tokenizer::setWhitespaceChars(const char *chars, int numChars)
{
    replaceChars(&whitespaceChars, chars, &numWhitespaceChars, numChars);
} // Tokenizer::setWhitespaceChars


void Tokenizer::setIgnoreWhitespace(bool onOff)
{
    ignoreWhitespace = onOff;
} // Tokenizer::setIgnoreWhitespace


void Tokenizer::setSingleLineComment(const char *str)
{
    replaceString(&singleLineCommentStart, str);
} // Tokenizer::setSingleLineComment


void Tokenizer::setIgnoreSingleLineComments(bool onOff)
{
    ignoreSingleLineComments = onOff;
} // Tokenizer::setIgnoreSingleLineComment


void Tokenizer::setMultiLineComment(const char *begin, const char *end)
{
    replaceString(&multiLineCommentStart, str);
    replaceString(&multiLineCommentEnd, str);
} // Tokenizer::setMultiLineComment


void Tokenizer::setIgnoreMultiLineComments(bool onOff)
{
    ignoreMultiLineComments = onOff;
} // Tokenizer::setIgnoreMultiLineComment


void Tokenizer::setQuoteChars(const char *chars, int numChars)
{
    replaceChars(&quoteChars, chars, &numQuoteChars, numChars);
} // Tokenizer::setQuoteChars


void Tokenizer::setEscapeChar(char ch)
{
    escapeChar = ch;
} // Tokenizer::setEscapeChar


void Tokenizer::setEscaping(bool onOff)
{
    isEscaping = onOff;
} // Tokenizer::setEscaping


void Tokenizer::setConvertNumbers(bool onOff)
{
    convertNumbers = onOff;
} // Tokenizer::setConvertNumbers


void Tokenizer::pushBack(void)
{
    pushedBack = true;
} // Tokenizer::pushBack


int Tokenizer::currentLine(void)
{
    return(lineNum);
} // Tokenizer::currentLine


#define BUFINCREASE 30

void Tokenizer::addToTokenBuffer(char ch)
{
    if (bufferIndex == bufferAllocSize)
    {
        int newAllocSize = bufferAllocSize + BUFINCREASE;
        char *tempBuf = new char[newAllocSize];
        memcpy(tempBuf, tokenBuffer, bufferIndex);
        memset(tempBuf + bufferIndex, '\0', BUFINCREASE);
        delete[] tokenBuffer;
        tokenBuffer = tempBuf;
        bufferAllocSize = newAllocSize;
    } // if

    tokenBuffer[bufferIndex++] = ch;
} // Tokenizer::addToTokenBuffer


void Tokenizer::emptyTokenBuffer(void)
{
    if (bufferIndex > 0)
    {
        memset(tokenBuffer, '\0', bufferIndex);
        bufferIndex = 0;
    } // if
} // Tokenizer::emptyTokenBuffer


bool Tokenizer::isWhitespaceChar(int ch)
{
    return(
            (whitespaceChars != NULL) &&
            (memchr(whitespaceChars, ch, numWhitespaceChars) != NULL)
          );
} // Tokenizer::isWhitespaceChar


// !!! FIXME: i18n?
bool Tokenizer::isNumberChar(int ch)
{
    return( (ch >= '0') && (ch <= '9') );
} // Tokenizer::isNumberChar


bool Tokenizer::isQuoteChar(int ch)
{
    return(
            (quoteChars != NULL) &&
            (memchr(quoteChars, ch, numQuoteChars) != NULL)
          );
} // Tokenizer::isQuoteChar


bool Tokenizer::isNewlineChar(int ch)
{
    return((ch == '\r') || (ch == '\n'));
} // Tokenizer::isNewlineChar


Tokenizer::tokentype Tokenizer::tokenizeNumber(void) throw (IOException *)
{
    int ch;
    bool decimalPoint = false;

    while (true)
    {
        ch = in->readChar();
        if (!isNumberChar(ch))
        {
            if (ch != '.')
                break;
            else
            {
                if (decimalPoint)
                    break;
                else
                    decimalPoint = true;
            } // else
        } // if

        addToTokenBuffer((char) ch);
    } // while

    in->pushBack();

    if (convertNumbers)
        num = atof(tokenBuffer);

    return(TT_NUMBER);
} // Tokenizer::tokenizeNumber


Tokenizer::tokentype Tokenizer::tokenizeNewline(void) throw (IOException *)
{
    int ch = in->readChar();
    if (ch == '\r')  // may be a DOS endline.
    {
        if (in->readChar() != '\n')  // Hmm...must be a MacOS endline.
            in->pushBack();
    } // if

    return(TT_NEWLINE);
} // Tokenizer::tokenizeNewline


Tokenizer::tokentype Tokenizer::tokenizeWhitespace(void) throw (IOException *)
{
    if (ignoreWhitespace)
    {
        while (isWhitespaceChar(in->readChar())) { /* do nothing... */ }
        in->pushBack();
        return(nextToken());
    } // if

    else  // keep the whitespace...
    {
        int ch;
        while (isWhitespaceChar(ch = in->readChar()))
            addToTokenBuffer((char) ch);
        in->pushBack();
        return(TT_WHITESPACE);
    } // else
} // Tokenizer::tokenizeWhitespace


Tokenizer::tokentype Tokenizer::tokenizeLiteralString(void)
                                    throw (IOException *)
{
    int quoteChar = in->readChar();

    addToTokenBuffer(quoteChar);

    while (true)
    {
        int ch = in->readChar();
        if (ch == TOBYEOF)  // uhoh.
        {
            addToTokenBuffer(quoteChar);
            in->pushBack();
            break;
        } // if

        addToTokenBuffer(ch);
        if (ch == quoteChar)
        {
            if ((!isEscaping) || (tokenBuffer[bufferIndex - 1] != escapeChar))
                break;
        } // if
    } // while

    return(TT_LITERALSTRING);
} // Tokenizer::tokenizeWord


Tokenizer::tokentype Tokenizer::tokenizeWord(void) throw (IOException *)
{
    return(TT_WORD);  // !!! FIXME: Write this!
} // Tokenizer::tokenizeWord


Tokenizer::tokentype Tokenizer::nextToken(void) throw (IOException *)
{
    if (pushedBack)
    {
        pushedBack = false;
        return(ttype);
    } // if

    emptyTokenBuffer();

    char ch = in->readChar();
    in->pushBack();

    if (isWhitespaceChar(ch))
        return(tokenizeWhitespace());
    else if (isNumberChar(ch))
        return(tokenizeNumber());
    else if (isQuoteChar(ch))
        return(tokenizeLiteralString());
    else if (isNewlineChar(ch))
        return(tokenizeNewline());
    else if (ch == TOBYEOF)
        return(TT_EOF);
    else
        return(tokenizeWord());
} // Tokenizer::nextToken


// end of Tokenizer.cpp ...

