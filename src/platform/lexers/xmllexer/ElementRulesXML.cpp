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

#include "platform/lexers/xmllexer/ElementRulesXML.h"

LexerRules *ElementRulesXML::buildRules(XMLNode *node)
{
    assert(strcmp(node->getTag(), "element") == 0);  // just in case.

    const char *_name = node->getAttributeValue("name");
    assert(_name != NULL);

    int iRollback = 0;
    const char *_rollback = node->getAttributeValue("rollback");
    if (_rollback != NULL)
        iRollback = atoi(_rollback);

    TobyCollection *children = node->getChildren();
    int max = children->getSize();
    LexerRules **rulesList = NULL;
    if (max > 0)
    {
        rulesList = new LexerRules *[max];
        for (int i = 0; i < max; i++)
        {
            XMLNode *kid = (XMLNode *) children->elementAt(i);
            rulesList[i] = XMLLexer::buildRules(kid);
        } // for
    } // if

    return(new ElementRulesXML(_name, iRollback, max, rulesList));
} // ElementRulesXML::buildRules


ElementRulesXML::ElementRulesXML(const char *_name, int rollback,
                                 int numKids, LexerRules **kids)
    : ElementRules(0, numKids, kids),
      name(NULL)
{
    assert(_name != NULL);
    name = new char[strlen(_name) + 1];
    strcpy(name, _name);
} // Constructor


ElementRulesXML::~ElementRulesXML(void)
{
    if (name != NULL)
        delete[] name;
} // Destructor


const char *ElementRulesXML::getName(void)
{
    return(name);  // READ. ONLY.
} // ElementRules::getName


    // C++ code to declare the module-scope variable that will hold the
    //  constructed ElementRules, and a declaration of a module-scope function
    //  that will do the constructing.
const char *ElementRulesXML::outputDeclarations(void)
{
    TobyString str;

    str.append("static ElementRules *element_");
    str.append(name);
    str.append(" = NULL;\n");
    str.append("static ElementRules *buildElement_");
    str.append(name);
    str.append("(void);\n\n");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ElementRulesXML::outputDeclarations


    // C++ code that defines the module-scope function that constructs the
    //  element, and stores a copy in the module-scope variable we declared
    //  in outputDeclarations()...
const char *ElementRulesXML::outputDefinitions(void)
{
    TobyString str;

    str.append("static ElementRules *buildElement_");
    str.append(name);
    str.append("(void)\n{\n");

    str.append("\n#ifdef DEBUG");
    str.append("    assert(element_");
    str.append(name);
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

    str.append("\n    element_");
    str.append(name);
    str.append(" = new ElementRules(");
    str.append(rollback);
    str.append(", ");
    str.append(numChildren);
    str.append(", kids);\n");

    str.append("return(element_");
    str.append(name);
    str.append(");\n} // buildElement_");
    str.append(name);
    str.append("\n\n");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ElementRulesXML::outputDefinitions


const char *ElementRulesXML::outputConstructor(void)
{
    TobyString str("buildElement_");
    str.append(name);
    str.append("()");
    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // ElementRulesXML::outputConstructor

// end of ElementRulesXML.cpp ...

