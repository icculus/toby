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
    delete[] *origString;

    if (newString == NULL)
        *origString = NULL;
    else
    {
        size_t numChars = strlen(newString) + 1;
        *origString = new char[numChars];
        memcpy(*origString, newString, numChars);
    } // else

    return(*origString);
} // replaceString


static char *replaceChars(char **origChars, const char *newChars,
                           size_t *numCharsOut, size_t numChars)
{
    assert(origChars != NULL);
    assert(numCharsOut != NULL);

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
    ignoreWhitespace(false),
    singleLineCommentStart(NULL),
    ignoreSingleLineComments(false),
    multiLineCommentStart(NULL),
    multiLineCommentEnd(NULL),
    ignoreMultiLineComments(false),
    convertNumbers(true),
    numsAreWords(false),
    ignoreNewlines(false),
    lineNum(0),
    lastChar(0),
    backBufferSize(0),
    caseSensitive(true),
    wordIndex(0)
{
    assert(in != NULL);
    memset(backBuffer, '\0', sizeof (backBuffer));
    setQuoteChars("\"\'", 2);
    setWhitespaceChars("\t ", 2);
    setSingleLineComment("//");
    setMultiLineComment("/*", "*/");
} // Constructor


Tokenizer::~Tokenizer(void)
{
    delete in;
    delete[] tokenBuffer;
    delete[] quoteChars;
    delete[] whitespaceChars;
    delete[] singleLineCommentStart;
    delete[] multiLineCommentStart;
    delete[] multiLineCommentEnd;
} // Destructor


void Tokenizer::setWhitespaceChars(const char *chars, size_t numChars)
{
    replaceChars(&whitespaceChars, chars, &numWhitespaceChars, numChars);
} // Tokenizer::setWhitespaceChars


void Tokenizer::setIgnoreWhitespace(bool onOff)
{
    ignoreWhitespace = onOff;
} // Tokenizer::setIgnoreWhitespace


void Tokenizer::setIgnoreNewlines(bool onOff)
{
    ignoreNewlines = onOff;
} // Tokenizer::setIgnoreNewlines


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
    replaceString(&multiLineCommentStart, begin);
    replaceString(&multiLineCommentEnd, end);
} // Tokenizer::setMultiLineComment


void Tokenizer::setIgnoreMultiLineComments(bool onOff)
{
    ignoreMultiLineComments = onOff;
} // Tokenizer::setIgnoreMultiLineComment


void Tokenizer::setQuoteChars(const char *chars, size_t numChars)
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


void Tokenizer::setNumbersAreWords(bool onOff)
{
    numsAreWords = onOff;
} // Tokenizer::setConvertNumbers


void Tokenizer::setCaseSensitive(bool onOff)
{
    caseSensitive = onOff;
} // Tokenizer::setCaseSensitive


bool Tokenizer::getCaseSensitive(void)
{
    return(caseSensitive);
} // Tokenizer::getCaseSensitive


void Tokenizer::pushBack(void)
{
    pushedBack = true;
} // Tokenizer::pushBack


long Tokenizer::currentLine(void)
{
    return(lineNum);
} // Tokenizer::currentLine


#define BUFINCREASE 30

void Tokenizer::extendTokenBuffer(void)
{
    size_t newAllocSize = bufferAllocSize + BUFINCREASE;
    char *tempBuf = new char[newAllocSize];
    if (bufferIndex > 0)
        memcpy(tempBuf, tokenBuffer, bufferIndex);
    
    memset(tempBuf + bufferIndex, '\0', BUFINCREASE);

    if (tokenBuffer != NULL)
        delete[] tokenBuffer;

    tokenBuffer = tempBuf;
    bufferAllocSize = newAllocSize;
} // Tokenizer::extendTokenBuffer


inline void Tokenizer::addToTokenBuffer(char ch)
{
    if (bufferIndex == bufferAllocSize)
        extendTokenBuffer();

    tokenBuffer[bufferIndex++] = ch;
} // Tokenizer::addToTokenBuffer


inline void Tokenizer::emptyTokenBuffer(void)
{
    if (bufferIndex > 0)
    {
        memset(tokenBuffer, '\0', bufferIndex);
        bufferIndex = 0;
    } // if
} // Tokenizer::emptyTokenBuffer


inline int Tokenizer::nextChar(void) throw (IOException *)
{
    if (backBufferSize)
    {
        backBufferSize--;
        lastChar = backBuffer[backBufferSize];
    } // if
    else
    {
        lastChar = in->readChar();
    } // else

    return(lastChar);
} // Tokenzier::nextChar


inline void Tokenizer::pushBackChar(void)
{
    assert(backBufferSize < sizeof (backBuffer));
    backBuffer[backBufferSize] = lastChar;
    backBufferSize++;
} // Tokenzier::nextChar


inline bool Tokenizer::isWhitespaceChar(int ch)
{
    return(
            (whitespaceChars != NULL) &&
            (memchr(whitespaceChars, ch, numWhitespaceChars) != NULL)
          );
} // Tokenizer::isWhitespaceChar


// !!! FIXME: i18n!
inline bool Tokenizer::isWordChar(int ch)
{
    if ((ch >= 'a') && (ch <= 'z'))
        return(true);

    if ((ch >= 'A') && (ch <= 'Z'))
        return(true);

    if ((ch >= '0') && (ch <= '9'))
        return(true);

    if (ch == '_')
        return(true);

    return(false);
} // Tokenizer::isWordChar


// !!! FIXME: i18n?
inline bool Tokenizer::isNumberChar(int ch)
{
    return( (ch >= '0') && (ch <= '9') );
} // Tokenizer::isNumberChar


inline bool Tokenizer::isQuoteChar(int ch)
{
    return(
            (quoteChars != NULL) &&
            (memchr(quoteChars, ch, numQuoteChars) != NULL)
          );
} // Tokenizer::isQuoteChar


inline bool Tokenizer::isNewlineChar(int ch)
{
    return((ch == '\r') || (ch == '\n'));
} // Tokenizer::isNewlineChar


inline void Tokenizer::moveTokenBufferToBackBuffer(void)
{
    assert(backBufferSize + bufferIndex < sizeof (backBuffer));

    while (bufferIndex > 0)
    {
        backBuffer[backBufferSize++] = tokenBuffer[--bufferIndex];
        tokenBuffer[bufferIndex] = '\0';
    } // for
} // Tokenizer::moveTokenBufferToBackBuffer


Tokenizer::tokentype Tokenizer::tokenizeNumber(void) throw (IOException *)
{
    int ch;
    bool decimalPoint = false;

    while (true)
    {
        ch = nextChar();
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

    pushBackChar();

    if (convertNumbers)
        num = atof(tokenBuffer);

    return((numsAreWords) ? TT_WORD : TT_NUMBER);
} // Tokenizer::tokenizeNumber


Tokenizer::tokentype Tokenizer::tokenizeNewline(void) throw (IOException *)
{
    int ch = nextChar();
    if (ch == '\r')  // may be a DOS endline.
    {
        if (nextChar() != '\n')  // Hmm...must be a MacOS endline.
            pushBackChar();
    } // if

    lineNum++;

    if (ignoreNewlines)
        return(nextToken());

    return(TT_NEWLINE);
} // Tokenizer::tokenizeNewline


Tokenizer::tokentype Tokenizer::tokenizeWhitespace(void) throw (IOException *)
{
    int ch;

    if (ignoreWhitespace)
    {
        while (isWhitespaceChar(ch = nextChar()))
        {
            if (ch == '\n')  // !!! FIXME!
                lineNum++;
        } // while
        pushBackChar();
        return(nextToken());
    } // if

    else  // keep the whitespace...
    {
        while (isWhitespaceChar(ch = nextChar()))
        {
            if (ch == '\n')  // !!! FIXME!
                lineNum++;
            addToTokenBuffer((char) ch);
        } // while
        pushBackChar();
        return(TT_WHITESPACE);
    } // else
} // Tokenizer::tokenizeWhitespace


Tokenizer::tokentype Tokenizer::tokenizeLiteralString(void)
                                    throw (IOException *)
{
    int quoteChar = nextChar();
    bool escaped = false;

    addToTokenBuffer(quoteChar);

    while (true)
    {
        int ch = nextChar();
        if (ch == TOBYEOF)  // uhoh.
        {
            addToTokenBuffer(quoteChar);
            pushBackChar();
            break;
        } // if

        addToTokenBuffer(ch);
        if (ch == escapeChar)
        {
            if (isEscaping)
                escaped = !escaped;
        } // else if

        else if (ch == quoteChar)
        {
            if (escaped)
                escaped = false;
            else
                break;  // end of literal string if not escaped.
        } // if

        else
        {
            if (escaped)
                escaped = false;
        } // else
    } // while

    return(TT_LITERALSTRING);
} // Tokenizer::tokenizeLiteralString


Tokenizer::tokentype Tokenizer::tokenizeSingleLineComment(void)
                                            throw (IOException *)
{
    int ch = singleLineCommentStart[0];  // already read this in.
    char *p;
    for (p = singleLineCommentStart; (*p) && (ch == *p); p++)
    {
        addToTokenBuffer(ch);
        ch = nextChar();
    } // for

    if (*p == '\0')  // a single line comment?
    {
        do
        {
            addToTokenBuffer(ch);
        } while (!isNewlineChar(ch = nextChar()));
        pushBackChar();

        return(TT_SINGLELINECOMMENT);
    } // if

    pushBackChar();
    moveTokenBufferToBackBuffer();

    return(TT_NONE);
} // Tokenizer::tokenizeSingleLineComment


Tokenizer::tokentype Tokenizer::tokenizeMultiLineComment(void)
                                            throw (IOException *)
{
    int ch = nextChar();
    char *p;
    for (p = multiLineCommentStart; (*p) && (ch == *p); p++)
    {
        addToTokenBuffer(ch);
        ch = nextChar();
    } // for

    if (*p == '\0')  // a multiline comment?
    {
        char endChar = multiLineCommentEnd[0];

        while (true)
        {
            do
            {
                if (ch == TOBYEOF)  // uhoh.
                {
                    pushBackChar();
                    return(TT_MULTILINECOMMENT);  // oh well.
                } // if

                else if (ch == '\r')
                {
                    if (nextChar() != '\n')
                        pushBackChar();
                    else
                    {
                        addToTokenBuffer('\r');
                        ch = '\n';
                    } // else
                    lineNum++;
                } // else if

                else if (ch == '\n')
                {
                    lineNum++;
                } // else if

                addToTokenBuffer(ch);
            } while ( (ch = nextChar()) != endChar );

            for (p = multiLineCommentEnd; (*p) && (ch == *p); p++)
            {
                addToTokenBuffer(ch);
                ch = nextChar();
            } // for

            pushBackChar();

            if (*p == '\0')  // end of a multiline comment?
                return(TT_MULTILINECOMMENT);
        } // while
    } // if

    // not a multiline comment at all?
    pushBackChar();
    moveTokenBufferToBackBuffer();

    return(TT_NONE);
} // Tokenizer::tokenizeMultiLineComment


Tokenizer::tokentype Tokenizer::tokenizeComment(void) throw (IOException *)
{
    bool checkSingle = true;  // Do the single line comment check?
    int ch = nextChar();

        // fast path.
    if ( !((singleLineCommentStart) && (ch == *singleLineCommentStart)) )
    {
        checkSingle = false;
        pushBackChar();
        if ( !((multiLineCommentStart) && (ch == *multiLineCommentStart)) )
            return(TT_NONE);
    } // if

    if (checkSingle)
    {
        Tokenizer::tokentype rc = tokenizeSingleLineComment();
        if (rc == TT_SINGLELINECOMMENT)
        {
            if (ignoreSingleLineComments)
                return(nextToken());
            else
                return(TT_SINGLELINECOMMENT);
        } // if
    } // if

    if (multiLineCommentStart)
    {
        Tokenizer::tokentype rc = tokenizeMultiLineComment();
        if (rc == TT_MULTILINECOMMENT)
        {
            if (ignoreMultiLineComments)
                return(nextToken());
            else
                return(TT_MULTILINECOMMENT);
        } // if
    } // if

    return(TT_NONE);
} // Tokenizer::tokenizeComment


Tokenizer::tokentype Tokenizer::tokenizeWord(void) throw (IOException *)
{
        // We need to handle comments here, since it might leave word
        //  characters in the token buffer.
    Tokenizer::tokentype rc = tokenizeComment();
    if (rc != TT_NONE)
        return(rc);

    int ch = nextChar();
    addToTokenBuffer(ch);
    if (!isWordChar(ch))
        return(TT_WORD);  // token in itself.

    while (isWordChar(ch = nextChar()))
        addToTokenBuffer(ch);

    pushBackChar();

    return(TT_WORD);
} // Tokenizer::tokenizeWord


Tokenizer::tokentype Tokenizer::_nextToken(void) throw (IOException *)
{
    if (pushedBack)
    {
        pushedBack = false;
        return(ttype);
    } // if

    wordIndex = 0;
    emptyTokenBuffer();

    char ch = nextChar();
    pushBackChar();

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
} // Tokenizer::_nextToken


Tokenizer::tokentype Tokenizer::nextToken(void) throw (IOException *)
{
    ttype = _nextToken();
    str = tokenBuffer;
    return(ttype);
} // Tokenizer::nextToken


bool Tokenizer::mustGetWord(const char *str)
{
    nextToken();
    if (ttype != TT_WORD)
        return(false);

    bool retval;
    if (caseSensitive)
        retval = (strcmp(this->str, str) == 0);
    else
        retval = (strcasecmp(this->str, str) == 0);

    return(retval);
} // mustGetWord


int Tokenizer::getNextWordChar(void)
{
    char retval = str[wordIndex];
    if (retval == '\0')
        retval = -1;
    else
        wordIndex++;

    return(retval);
} // Tokenizer::getNextWordChar


void Tokenizer::pushBackWordChar(void)
{
    if (wordIndex)
        wordIndex--;
} // Tokenizer::pushBackWordChar

// end of Tokenizer.cpp ...

