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

#include "platform/lexers/xmllexer/ReqWordRulesXML.h"


LexerRules *ReqWordRulesXML::buildRules(XMLNode *node)
{
    assert(strcmp(node->getTag(), "reqword") == 0);  // just in case.

    bool lookup = node->getBoolAttribute("lookup", false);
    const char *_word = node->getAttributeValue("text");
    const char *_dynSymbol = NULL;

    if (_word == NULL)
        return(NULL);

    if (lookup)
    {
        TobyLoader *loader = TobyLoader::getInstance(NULL);
        if (loader == NULL)
            return(NULL);

        _dynSymbol = _word;
        _word = (const char *) loader->getSymbol(_dynSymbol);
        delete loader;
    } // if

    if (_word == NULL)
        return(NULL);

    return(new ReqWordRulesXML(_word, _dynSymbol));
} // ReqWordRulesXML::buildRules


ReqWordRulesXML::ReqWordRulesXML(const char *_word, const char *_dynSymbol)
    : ReqWordRules(_word),
      dynSymbol(NULL)
{
    if (_dynSymbol)
    {
        dynSymbol = new char[strlen(_dynSymbol) + 1];
        strcpy(dynSymbol, _dynSymbol);
    } // if
} // Constructor


ReqWordRulesXML::~ReqWordRulesXML(void)
{
    delete[] dynSymbol;
} // Destructor


const char *ReqWordRulesXML::outputConstructor(void)
{
    TobyString str("new ReqWordRules(");

    if (dynSymbol)
    {
        str.append(dynSymbol);
    } // if
    else
    {
        str.append("\"");
        str.append(word);  // !!! FIXME: Escape this string.
        str.append("\"");
    } // else

    str.append(")");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ReqWordRulesXML::outputConstructor

// end of ReqWordRulesXML.cpp ...

