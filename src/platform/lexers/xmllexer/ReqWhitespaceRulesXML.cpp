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

#include "platform/lexers/xmllexer/ReqWhitespaceRulesXML.h"


LexerRules *ReqWhitespaceRulesXML::buildRules(XMLNode *node)
{
    assert(strcmp(node->getTag(), "reqwhitespace") == 0);  // just in case.
    return(new ReqWhitespaceRulesXML());
} // ReqWhitespaceRulesXML::buildRules


ReqWhitespaceRulesXML::ReqWhitespaceRulesXML(void)
{
} // Constructor


ReqWhitespaceRulesXML::~ReqWhitespaceRulesXML(void)
{
} // Destructor


const char *ReqWhitespaceRulesXML::outputConstructor(void)
{
    TobyString str("new ReqWhitespaceRules()");
    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ReqWhitespaceRulesXML::outputConstructor

// end of ReqWhitespaceRulesXML.cpp ...

