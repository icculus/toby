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

#include "platform/lexers/xmllexer/XMLLexer.h"


    // (tag) is the XML tag in question.
    // (func) should return a pointer to an appropriate LexerRules, built
    //  based on the details of (node). The passed node should be correct
    //  (only <element> nodes should be passed to ElementRulesXML), but it
    //  couldn't hurt to assert this fact. The rules in their entirety need
    //  to be be built. That is, an ElementRulesXML class should build all
    //  it's child rules, too, preferrably by passing their nodes through
    //  XMLLexer::buildRules(). This makes the buildRules() method more
    //  complex, but keeps the parent class as lean and fast as possible,
    //  so all the overhead goes away if we generate the rules as C++
    //  with outputConstructor(), etc.
typedef struct
{
    const char *tag;
    LexerRules *(*func)(XMLNode *node);
} RulesTagHandlers;

static RulesTagHandlers handlers[] =
{
    { "language", LanguageRulesXML::buildRules },
    { "element", ElementRulesXML::buildRules },
    { NULL, NULL }
};


LexerRules *XMLLexer::buildRules(XMLNode *node)
{
    const char *tag = node->getTag();

    for (int i = 0; handlers[i].tag != NULL; i++)
    {
        if (strcmp(tag, handlers[i].tag) == 0)
            return(handlers[i].func(node));
    } // for

    assert(false);
    return(NULL);
} // XMLLexer::buildRules


LexerRules **XMLLexer::buildLanguages(XMLTree *tree)
{
    XMLNode *root = tree->getRootNode();
    TobyCollection *children = root->getChildren();
    int max = children->getSize();
    int langCount = 0;
    LexerRules **retval = NULL;

    retval = new LexerRules *[max + 1];
    for (int i = 0; i < max; i++)
    {
        XMLNode *node = (XMLNode *) children->elementAt(i);
        if (strcmp(node->getTag(), "language") == 0)
        {
            retval[langCount] = buildRules(node);
            if (retval[langCount] != NULL)
                langCount++;
        } // if
    } // for

    retval[langCount] = NULL;
    return(retval);
} // XMLLexer::buildLanguages


Lexer *Lexer::getInstance(void)
{
    return(new XMLLexer());
} // Lexer::getInstance


void XMLLexer::loadLanguages(void)
{
    // !!! WRITE ME!
} // XMLLexer::loadLanguages


// end of XMLLexer.cpp ...

