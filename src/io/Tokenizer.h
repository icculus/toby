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

#ifndef _INCLUDE_TOKENIZER_H_
#define _INCLUDE_TOKENIZER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/TobyObject.h"
#include "io/TobyReader.h"
#include "exceptions/IOException.h"


// !!! FIXME : What to do about Unicode/wide chars?


/*
 * The Tokenizer takes a stream from a TobyReader and does some basic
 *  parsing, removing comments and splitting the stream up into tokens of
 *  various types.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */

class Tokenizer : public TobyObject
{
public:
    typedef enum
    {
        TT_NONE = -1,
        TT_WORD = 0,
        TT_NUMBER,
        TT_NEWLINE,
        TT_WHITESPACE,
        TT_SINGLELINECOMMENT,
        TT_MULTILINECOMMENT,
        TT_EOF,
        TT_LITERALSTRING,
        TT_TOTAL
    } tokentype;

    tokentype ttype;
    char *str;
    double num;

        // Constructor: Pass this a TobyReader of the stream you'd like
        //  tokenized.
    Tokenizer(TobyReader *reader);

        // The destructor will close and delete the TobyReader you passed in,
        //  so you don't need to keep a reference to it, if you don't want to.
    virtual ~Tokenizer(void);

        // Define the characters that are considered to be whitespace. If
        //  you want to specify newlines as whitespace, then be sure to
        //  list both '\r' and '\n', so that it will give consistent results
        //  on Win32, Unix, and MacOS. Note that if you don't specify newlines
        //  in here, then the tokenizer will return them as TT_NEWLINE.
        //  Pass an array of (numChars) characters. If you don't want
        //  anything registered as whitespace, call this method as
        //  setWhitespaceChars(NULL, 0), in which case they'll register as
        //  valid tokens. Default is "\t "  (that's TAB and SPACE).
    virtual void setWhitespaceChars(const char *chars, int numChars);

        // Ignore whitespace. Default is true. If you don't ignore whitespace,
        //  then nextToken() will place each block of it (not each individual
        //  in the str field and return TT_WHITESPACE. Be sure to define your
        //  whitespace characters with setWhitespaceChars(), too!
    virtual void setIgnoreWhitespace(bool onOff);

        // Define the sequence of chars that signifies a single line comment.
        //  Default is "//". Set it to NULL to disable single line comments.
        //  The next token after a single line comment is a newline or EOF.
    virtual void setSingleLineComment(const char *str);

        // Ignore single line comments. If this is disabled, nextToken() will
        //  return TT_SINGLELINECOMMENT and store the comment (including the
        //  comment starting characters) in the (str) field. If enabled,
        //  nextToken() will just pretend the comment didn't exist, and return
        //  the next non-ignored token.
    virtual void setIgnoreSingleLineComments(bool onOff);

        // Define the sequence of chars that signifies a multiline comment.
        //  Default is "/*" and "*/". Set either to NULL to disable multiline
        //  comments. The nextToken() call that comes across a multiline
        //  comment will throw the comment away and return the next valid
        //  token after them.
    virtual void setMultiLineComment(const char *begin, const char *end);

        // Ignore multiline comments. If this is disabled, nextToken() will
        //  return TT_MULTILINECOMMENT and store the comment (including the
        //  comment delimiting characters and newlines) in the (str) field. If
        //  enabled, nextToken() will just pretend the comment didn't exist,
        //  and return the next non-ignored token. Default is true.
    virtual void setIgnoreMultiLineComments(bool onOff);

        // Define the characters that delimits a literal string. Pass an
        //  array of (numChars) characters. So, if you call this with
        //  setQuoteChars("\"\'", 2), then "a string" and 'another string'
        //  will both look like literal strings to the tokenizer, but
        //  `third string` and "forth string' won't.
        //  Default quotes are just the double quote ('\"').
        // If you don't want to tokenize literal strings in blocks, just call
        //  this method as setQuoteChars(NULL, 0);
    virtual void setQuoteChars(const char *chars, int numChars);

        // Define an escape character. Default is '\\'. This is used by the
        //  literal string handler: If you have "he said, \"woot\"", then the
        //  tokenizer will give you ["he said, \"] as a token, and probably
        //  [woot.] followed by [\] and [""] if you didn't use '\\' as an
        //  escape char. Don't forget to call setEscaping(true), too!
    virtual void setEscapeChar(char ch);

        // Tell the Tokenizer to accept the escape character.
        //  Default is true.
    virtual void setEscaping(bool onOff);

        // Convert numbers from a string token to a double before returning
        //  from nextToken(). The string version will also be available; this
        //  just determines if the (num) field is calculated.
    virtual void setConvertNumbers(bool onOff);

        // When a token that would be reported as TT_NUMBER is encountered,
        //  report it as TT_WORD instead. setConvertNumbers can still be
        //  configured to update the num field independent of this setting.
        //  Default is false.
    virtual void setNumbersAreWords(bool onOff);

        // !!! write me!
    virtual tokentype nextToken(void) throw (IOException *);

        // Make the next call to nextToken() return the same thing as
        //  the last call. This only lets you push back once between calls
        //  to nextToken()!
    virtual void pushBack(void);

        // Get the current line number (option base 0) in the stream.
    virtual int currentLine(void);


protected:

        // Append another character to (tokenBuffer). The buffer is resized
        //  (and thus, the pointer can change) if needed.
    inline void addToTokenBuffer(char ch);

        // Reset (bufferIndex) to 0, and null out the current contents of
        //  (tokenBuffer).
    inline void emptyTokenBuffer(void);
    inline bool isWhitespaceChar(int ch);
    inline bool isNumberChar(int ch);
    inline bool isQuoteChar(int ch);
    inline bool isNewlineChar(int ch);
    inline bool isWordChar(int ch);  // NumberChars are true here, too.

    TobyReader *in;
    char *tokenBuffer;
    int bufferAllocSize;
    int bufferIndex;

    inline int nextChar(void) throw (IOException *);
    inline void pushBackChar(void);
    inline void moveTokenBufferToBackBuffer(void);

private:
    bool pushedBack;

    char *quoteChars;
    int numQuoteChars;
    char escapeChar;
    bool isEscaping;

    char *whitespaceChars;
    int numWhitespaceChars;
    bool ignoreWhitespace;

    char *singleLineCommentStart;
    bool ignoreSingleLineComments;

    char *multiLineCommentStart;
    char *multiLineCommentEnd;
    bool ignoreMultiLineComments;

    bool convertNumbers;
    bool numsAreWords;

    int lineNum;

    int lastChar;
    unsigned int backBufferSize;
    int backBuffer[256];

    void extendTokenBuffer(void);

    tokentype tokenizeSingleLineComment(void) throw (IOException *);
    tokentype tokenizeMultiLineComment(void) throw (IOException *);
    tokentype tokenizeWhitespace(void) throw (IOException *);
    tokentype tokenizeNumber(void) throw (IOException *);
    tokentype tokenizeLiteralString(void) throw (IOException *);
    tokentype tokenizeComment(void) throw (IOException *);
    tokentype tokenizeNewline(void) throw (IOException *);
    tokentype tokenizeWord(void) throw (IOException *);
    tokentype _nextToken(void) throw (IOException *);
}; // class Tokenizer

#endif  // !defined _INCLUDE_TOKENIZER_H_

// end of Tokenizer.h ...

