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

#include "platform/lexers/xmllexer/PickOneRulesXML.h"

unsigned long PickOneRulesXML::nextPickOneID;


LexerRules *PickOneRulesXML::buildRules(XMLNode *node)
{
    assert(strcmp(node->getTag(), "pickone") == 0);  // just in case.

    size_t max;
    LexerRules **rulesList = XMLLexer::buildBasicChildRules(node, &max);
    if (rulesList == NULL)
        return(NULL);

    return(new PickOneRulesXML(max, rulesList));
} // PickOneRulesXML::buildRules


PickOneRulesXML::PickOneRulesXML(size_t numKids, LexerRules **kids)
    : PickOneRules(numKids, kids),
      pickOneID(nextPickOneID)
{
    nextPickOneID++;
} // Constructor


PickOneRulesXML::~PickOneRulesXML(void)
{
} // Destructor


    // C++ code to declare the module-scope variable that will hold the
    //  constructed PickOneRules, and a declaration of a module-scope function
    //  that will do the constructing.
const char *PickOneRulesXML::outputDeclarations(void)
{
    TobyString str;

    str.append("static PickOneRules *buildPickOne_");
    str.append(pickOneID);
    str.append("(void);\n\n");

    for (size_t i = 0; i < numChildren; i++)
    {
        const char *kidDeclarations = children[i]->outputDeclarations();
        str.append(kidDeclarations);
        delete[] kidDeclarations;
    } // for

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // PickOneRulesXML::outputDeclarations


    // C++ code that defines the module-scope function that constructs the
    //  element, and stores a copy in the module-scope variable we declared
    //  in outputDeclarations()...
const char *PickOneRulesXML::outputDefinitions(void)
{
    TobyString str;

    for (size_t i = 0; i < numChildren; i++)
    {
        const char *kidDefinitions = children[i]->outputDefinitions();
        str.append(kidDefinitions);
        delete[] kidDefinitions;
    } // for

    str.append("static PickOneRules *buildPickOne_");
    str.append(pickOneID);
    str.append("(void)\n{\n");

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

    str.append("    return(new PickOneRules(");
    str.append(numChildren);
    str.append(", kids));\n} // buildPickOne_");
    str.append(pickOneID);
    str.append("\n\n");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // PickOneRulesXML::outputDefinitions


const char *PickOneRulesXML::outputConstructor(void)
{
    TobyString str("buildPickOne_");
    str.append(pickOneID);
    str.append("()");
    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // PickOneRulesXML::outputConstructor


const char *PickOneRulesXML::outputResolutions(void)
{
    TobyString str;
    for (size_t i = 0; i < numChildren; i++)
    {
        const char *kidResolutions = children[i]->outputResolutions();
        str.append(kidResolutions);
        delete[] kidResolutions;
    } // for

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // PickOneRulesXML::outputResolutions

// end of PickOneRulesXML.cpp ...

