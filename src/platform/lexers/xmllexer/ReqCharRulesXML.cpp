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

#include "platform/lexers/xmllexer/ReqCharRulesXML.h"

LexerRules *ReqCharRulesXML::buildRules(XMLNode *node)
{
    assert(strcmp(node->getTag(), "reqchar") == 0);  // just in case.

    const char *type = node->getAttributeDefault("type", "alphanumeric");
    const char *str = node->getAttributeValue("char");
    char _specific = ((str) ? str[0] : '\0');
    ReqCharRules::chartype _ctype = CTYPE_ALPHANUMERIC;

    if (strcmp(type, "alphanumeric") == 0)
        _ctype = CTYPE_ALPHANUMERIC;
    else if (strcmp(type, "letter") == 0)
        _ctype = CTYPE_LETTER;
    else if (strcmp(type, "number") == 0)
        _ctype = CTYPE_NUMBER;
    else if (strcmp(type, "specific") == 0)
        _ctype = CTYPE_SPECIFIC;
    else if (strcmp(type, "end") == 0)
        _ctype = CTYPE_END;
    else
    {
        assert(false);
    } // else

    return(new ReqCharRulesXML(_ctype, _specific));
} // ReqCharRulesXML::buildRules


ReqCharRulesXML::ReqCharRulesXML(ReqCharRules::chartype _ctype, char _specific)
    : ReqCharRules(_ctype, _specific),
      ctype(_ctype)
{
} // Constructor


ReqCharRulesXML::~ReqCharRulesXML(void)
{
} // Destructor


const char *ReqCharRulesXML::outputConstructor(void)
{
    TobyString str("new ReqCharRules(");

    switch (ctype)
    {
        case CTYPE_ALPHANUMERIC:
            str.append("ReqCharRules::CTYPE_ALPHANUMERIC");
            break;
        case CTYPE_NUMBER:
            str.append("ReqCharRules::CTYPE_NUMBER");
            break;
        case CTYPE_LETTER:
            str.append("ReqCharRules::CTYPE_LETTER");
            break;
        case CTYPE_SPECIFIC:
            str.append("ReqCharRules::CTYPE_SPECIFIC");
            break;
        case CTYPE_END:
            str.append("ReqCharRules::CTYPE_END");
            break;
        default:
            assert(false);
    } // switch

    str.append(", (char) ");
    str.append((long) specific);
    str.append(")");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ReqCharRulesXML::outputConstructor

// end of ReqCharRulesXML.cpp ...

