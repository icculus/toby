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

#include "platform/lexers/xmllexer/TokenizerRulesXML.h"

static void unescapeAttrDefault(XMLNode *node, char **str, size_t *len,
                                const char *attrname, const char *defval)
{
    const char *val = node->getAttributeDefault(attrname, defval);

    if (val == NULL)
    {
        *str = NULL;
        *len = 0;
        return;
    } // if

    *len = strlen(val);
    *str = new char[*len + 1];

    char *dest = *str;
    for (const char *src = val; *src != '\0'; src++, dest++)
    {
        if (*src == '\\')
        {
            src++;
            if (*src == 'b')
                *dest = '\b';
            else if (*src == 'a')
                *dest = '\a';
            else if (*src == 't')
                *dest = '\t';
            else if (*src == 'n')
                *dest = '\n';
            else if (*src == 'r')
                *dest = '\r';
            else if (*src == '0')
                *dest = '\0';
            else if (*src == '\\')
                *dest = '\\';
            else
                *dest = *src;
            (*len)--;
        } // if
        else
        {
            *dest = *src;
        } // else
    } // for

    *dest = '\0';
} // unescapeAttrDefault


static void unescapeAttrValue(XMLNode *node, char **str, size_t *len,
                              const char *attrname)
{
    unescapeAttrDefault(node, str, len, attrname, NULL);
} // unescapeAttrValue


LexerRules *TokenizerRulesXML::buildRules(XMLNode *node)
{
    assert(strcmp(node->getTag(), "tokenizer") == 0);  // just in case.

    bool _caseSensitive = node->getBoolAttribute("casesensitive", true);
    bool _ignoreWhitespace = node->getBoolAttribute("ignorewhitespace", true);
    bool _ignoreNewlines = node->getBoolAttribute("ignorenewlines", true);
    bool _ignoreSingle = node->getBoolAttribute("ignoresinglelinecomments", true);
    bool _ignoreMulti = node->getBoolAttribute("ignoremultilinecomments", true);
    bool _escaping = node->getBoolAttribute("escaping", true);
    bool _cvtNums = node->getBoolAttribute("convertnumstowords", false);
    const char *_singleCmt = node->getAttributeValue("singlelinecomment");
    const char *_multiCmt1 = node->getAttributeValue("multilinecommentstart");
    const char *_multiCmt2 = node->getAttributeValue("multilinecommentend");
    char *_escChar = NULL;
    size_t _escCharLen = 0;
    char *_quoteChars = NULL;
    size_t _quoteCharsLen = 0;
    char *_wspace = NULL;
    size_t _wspaceLen = 0;

    unescapeAttrDefault(node, &_escChar, &_escCharLen, "escapechar", "\\");
    unescapeAttrValue(node, &_quoteChars, &_quoteCharsLen, "quotechars");
    unescapeAttrDefault(node, &_wspace, &_wspaceLen, "whitespacechars", " \t");

    TokenizerRulesXML *retval;
    retval = new TokenizerRulesXML(_caseSensitive, _wspace, _wspaceLen,
                                   _ignoreWhitespace, _ignoreNewlines,
                                   _singleCmt, _ignoreSingle,
                                   _multiCmt1, _multiCmt2, _ignoreMulti,
                                   _quoteChars, _quoteCharsLen, _escaping,
                                   (_escChar == NULL) ? '\\' : _escChar[0],
                                   _cvtNums);

    if (_escChar != NULL)
        delete[] _escChar;

    if (_quoteChars != NULL)
        delete[] _wspace;

    if (_wspace != NULL)
        delete[] _wspace;

    return(retval);
} // TokenizerRulesXML::buildRules


TokenizerRulesXML::TokenizerRulesXML(bool _caseSensitive,
                                     const char *_whitespaceChars,
                                     size_t _numWhitespaceChars,
                                     bool _ignoreWhitespace,
                                     bool _ignoreNewlines,
                                     const char *_singleLineComment,
                                     bool _ignoreSingleLineComments,
                                     const char *_multiLineCommentStart,
                                     const char *_multiLineCommentEnd,
                                     bool _ignoreMultiLineComments,
                                     const char *_quoteChars,
                                     size_t _numQuoteChars,
                                     bool _escaping,
                                     char _escapeChar,
                                     bool _convertNumsToWords)
    : TokenizerRules(_caseSensitive,
                     _whitespaceChars,
                     _numWhitespaceChars,
                     _ignoreWhitespace,
                     _ignoreNewlines,
                     _singleLineComment,
                     _ignoreSingleLineComments,
                     _multiLineCommentStart,
                     _multiLineCommentEnd,
                     _ignoreMultiLineComments,
                     _quoteChars,
                     _numQuoteChars,
                     _escaping,
                     _escapeChar,
                     _convertNumsToWords)
{
} // Constructor


TokenizerRulesXML::~TokenizerRulesXML(void)
{
} // Destructor


static void appendEscapedString(TobyString *str, const char *text, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (text[i] == '\0')
            str->append("\\0");
        else if (text[i] == '\n')
            str->append("\\n");
        else if (text[i] == '\r')
            str->append("\\r");
        else if (text[i] == '\t')
            str->append("\\t");
        else if (text[i] == '\a')
            str->append("\\a");
        else if (text[i] == '\b')
            str->append("\\b");
        else
            str->append(text[i]);
    } // for
} // appendEscapedStringWithLen


static void appendEscapedString(TobyString *str, const char *text)
{
    appendEscapedString(str, text, strlen(text));
} // appendEscapedString


const char *TokenizerRulesXML::outputConstructor(void)
{
    TobyString str("new TokenizerRules(");
    str.append(caseSensitive);
    str.append(", \"");
    appendEscapedString(&str, whitespaceChars, numWhitespaceChars);
    str.append("\", ");
    str.append(numWhitespaceChars);
    str.append(", ");
    str.append(ignoreWhitespace);
    str.append(", ");
    str.append(ignoreNewlines);
    str.append(", \"");
    appendEscapedString(&str, singleLineComment);
    str.append("\", ");
    str.append(ignoreSingleLineComments);
    str.append(", \"");
    appendEscapedString(&str, multiLineCommentStart);
    str.append("\", \"");
    appendEscapedString(&str, multiLineCommentEnd);
    str.append("\", ");
    str.append(ignoreMultiLineComments);
    str.append(", \"");
    appendEscapedString(&str, quoteChars, numQuoteChars);
    str.append("\", ");
    str.append(numQuoteChars);
    str.append(", ");
    str.append(escaping);
    str.append(", \'");
    appendEscapedString(&str, &escapeChar, 1);
    str.append("\', ");
    str.append(convertNumsToWords);
    str.append(")");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // TokenizerRulesXML::outputConstructor

// end of TokenizerRulesXML.cpp ...

