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

#include "platform/lexers/xmllexer/ReqWordCharsRulesXML.h"

unsigned long ReqWordCharsRulesXML::nextReqWordCharsID;


LexerRules *ReqWordCharsRulesXML::buildRules(XMLNode *node)
{
    assert(strcmp(node->getTag(), "reqwordchars") == 0);  // just in case.
    size_t max;
    LexerRules **rulesList = XMLLexer::buildBasicChildRules(node, &max);
    return(new ReqWordCharsRulesXML(max, rulesList));
} // buildRules


ReqWordCharsRulesXML::ReqWordCharsRulesXML(size_t numKids, LexerRules **kids)
    : ReqWordCharsRules(numKids, kids),
      reqWordCharsID(nextReqWordCharsID)
{
    nextReqWordCharsID++;
} // Constructor


ReqWordCharsRulesXML::~ReqWordCharsRulesXML(void)
{
} // Destructor


    // C++ code to declare the module-scope variable that will hold the
    //  constructed ReqWordCharsRules, and a declaration of a module-scope function
    //  that will do the constructing.
const char *ReqWordCharsRulesXML::outputDeclarations(void)
{
    TobyString str;

    str.append("static ReqWordCharsRules *reqwordchars_");
    str.append(reqWordCharsID);
    str.append(" = NULL;\n");
    str.append("static ReqWordCharsRules *buildReqWordChars_");
    str.append(reqWordCharsID);
    str.append("(void);\n\n");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ReqWordCharsRulesXML::outputDeclarations


    // C++ code that defines the module-scope function that constructs the
    //  element, and stores a copy in the module-scope variable we declared
    //  in outputDeclarations()...
const char *ReqWordCharsRulesXML::outputDefinitions(void)
{
    TobyString str;

    str.append("static ReqWordCharsRules *buildReqWordChars_");
    str.append(reqWordCharsID);
    str.append("(void)\n{\n");

    str.append("\n#ifdef DEBUG");
    str.append("    assert(reqwordchars_");
    str.append(reqWordCharsID);
    str.append(" == NULL);\n#endif\n\n");

    str.append("    LexerRules **kids = new LexerRules *[");
    str.append(numChildren);
    str.append("];\n");

    for (size_t i = 0; i < numChildren; i++)
    {
        str.append("    kids[");
        str.append(i);
        str.append("] = ");
        const char *kidConstructor = children[i]->outputConstructor();
        str.append(kidConstructor);
        delete[] kidConstructor;
        str.append(";\n");
    } // for

    str.append("\n    reqwordchars_");
    str.append(reqWordCharsID);
    str.append(" = new ReqWordCharsRules(");
    str.append(numChildren);
    str.append(", kids);\n");

    str.append("return(reqwordchars_");
    str.append(reqWordCharsID);
    str.append(");\n} // buildReqWordChars_");
    str.append(reqWordCharsID);
    str.append("\n\n");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ReqWordCharsRulesXML::outputDefinitions


const char *ReqWordCharsRulesXML::outputConstructor(void)
{
    TobyString str("buildReqWordChars_");
    str.append(reqWordCharsID);
    str.append("()");
    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ReqWordCharsRulesXML::outputConstructor

// end of ReqWordCharsRulesXML.cpp ...

