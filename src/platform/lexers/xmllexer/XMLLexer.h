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

#ifndef _INCLUDE_XMLLEXER_H_
#define _INCLUDE_XMLLEXER_H_

#include "lexer/Lexer.h"
#include "xml/XMLNode.h"
#include "xml/XMLTree.h"
#include "platform/lexers/xmllexer/LanguageRulesXML.h"
#include "platform/lexers/xmllexer/ElementRulesXML.h"
#include "platform/lexers/xmllexer/TokenizerRulesXML.h"
#include "platform/lexers/xmllexer/RepeatRulesXML.h"
#include "platform/lexers/xmllexer/PickOneRulesXML.h"
#include "platform/lexers/xmllexer/ReqWordRulesXML.h"
#include "platform/lexers/xmllexer/ReqEOFRulesXML.h"
#include "platform/lexers/xmllexer/ReqNewlineRulesXML.h"
#include "platform/lexers/xmllexer/ReqLiteralStringRulesXML.h"
#include "platform/lexers/xmllexer/ReqWhitespaceRulesXML.h"
#include "platform/lexers/xmllexer/ReqSingleLineCommentRulesXML.h"
#include "platform/lexers/xmllexer/ReqMultiLineCommentRulesXML.h"
#include "platform/lexers/xmllexer/ReqNumberRulesXML.h"
#include "platform/lexers/xmllexer/ReqWordCharsRulesXML.h"
#include "platform/lexers/xmllexer/ReqCharRulesXML.h"
#include "platform/lexers/xmllexer/ReqElementRulesXML.h"

class ElementRulesXML;  // need a forward reference.

/*
 * An entry point for generating, from XML metadata, the LexerRules tree
 *  needed to lex a language
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class XMLLexer : public Lexer
{
public:
    static LexerRules *buildRules(XMLNode *node);
    static LexerRules **buildBasicChildRules(XMLNode *node, size_t *count);
    LexerRules **buildLanguages(XMLTree *tree);

    static void addElement(ElementRulesXML *elem);
    static ElementRulesXML *getElementByName(const char *name);

    virtual void loadLanguages(void);

protected:
    static TobyCollection *elements;
}; // class XMLLexer

#endif // !defined _INCLUDE_XMLLEXER_H_

// end of XMLLexer.h ...

