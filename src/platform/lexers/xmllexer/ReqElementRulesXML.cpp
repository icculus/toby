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

#include "platform/lexers/xmllexer/ReqElementRulesXML.h"

unsigned long ReqElementRulesXML::nextReqElementID;


LexerRules *ReqElementRulesXML::buildRules(XMLNode *node)
{
    assert(strcmp(node->getTag(), "reqelement") == 0);  // just in case.

    const char *_name = node->getAttributeValue("name");
    if (_name == NULL)
        return(NULL);

    return(new ReqElementRulesXML(_name));
} // ReqElementRulesXML::buildRules


ReqElementRulesXML::ReqElementRulesXML(const char *_name)
    : name(NULL),
      reqElementID(nextReqElementID++)
{
    if (_name != NULL)
    {
        name = new char[strlen(_name) + 1];
        strcpy(name, _name);
    } // if
} // Constructor


ReqElementRulesXML::~ReqElementRulesXML(void)
{
    delete[] name;
} // Destructor


    // C++ code to declare the module-scope variable that will hold the
    //  constructed ReqElementRules, and a declaration of a module-scope function
    //  that will do the constructing.
const char *ReqElementRulesXML::outputDeclarations(void)
{
    TobyString str;

    str.append("static ReqElementRules *reqelement_");
    str.append(reqElementID);
    str.append(" = NULL;\n");
    str.append("static ReqElementRules *buildReqElement_");
    str.append(reqElementID);
    str.append("(void);\n\n");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ReqElementRulesXML::outputDeclarations


    // C++ code that defines the module-scope function that constructs the
    //  element, and stores a copy in the module-scope variable we declared
    //  in outputDeclarations()...
const char *ReqElementRulesXML::outputDefinitions(void)
{
    TobyString str;

    str.append("static ReqElementRules *buildReqElement_");
    str.append(reqElementID);
    str.append("(void)\n{\n");

    str.append("    assert(reqelement_");
    str.append(reqElementID);
    str.append(" == NULL);\n\n");

    str.append("    reqelement_");
    str.append(reqElementID);
    str.append(" = new ReqElementRules();\n");
    str.append("    return(reqelement_");
    str.append(reqElementID);
    str.append(");\n} // buildReqElement_");
    str.append(reqElementID);
    str.append("\n\n");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ReqElementRulesXML::outputDefinitions


const char *ReqElementRulesXML::outputConstructor(void)
{
    TobyString str("buildReqElement_");
    str.append(reqElementID);
    str.append("()");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ReqElementRulesXML::outputConstructor


const char *ReqElementRulesXML::outputResolutions(void)
{
    TobyString str;

    if (name == NULL)
        str.append(" // There's a <reqelement> tag with no name attribute!\n");
    else
    {
        str.append("    assert(reqelement_");
        str.append(reqElementID);
        str.append(" != NULL);\n");
        str.append("    assert(element_");
        str.append(name);
        str.append(" != NULL);\n");
        str.append("    reqelement_");
        str.append(reqElementID);
        str.append("->setElement(element_");
        str.append(name);
        str.append(");\n");
    } // if

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ReqElementRulesXML::outputResolutions


bool ReqElementRulesXML::resolve(LexerRules *langRules)
{
    if (name != NULL)
        setElement(XMLLexer::getElementByName(name));

    return(ReqElementRules::resolve(langRules));
} // ReqElementRulesXML::resolve

// end of ReqElementRulesXML.cpp ...

