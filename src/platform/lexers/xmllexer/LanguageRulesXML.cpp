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

#include "platform/lexers/xmllexer/LanguageRulesXML.h"

// !!! FIXME: Clean this function up.
LexerRules *LanguageRulesXML::buildRules(XMLNode *node)
{
    assert(strcmp(node->getTag(), "language") == 0);  // just in case.

    const char *_name = node->getAttributeValue("name");
    assert(_name != NULL);

    const char *firstName = node->getAttributeValue("firstelement");
    assert(firstName != NULL);

    const char *_ext = node->getAttributeValue("fileextension");
    assert(_ext != NULL);

    LexerRules *firstRule = NULL;
    LexerRules *_tokRule = NULL;
    TobyCollection *children = node->getChildren();
    size_t max = children->getSize();
    LexerRules **rulesList = NULL;
    size_t ruleCount = 0;
    if (max > 0)
    {
        rulesList = new LexerRules *[max];
        for (size_t i = 0; i < max; i++)
        {
            XMLNode *kid = (XMLNode *) children->elementAt(i);
            const char *tag = kid->getTag();
            if (strcmp(tag, "tokenizer") == 0)
                _tokRule = XMLLexer::buildRules(kid);
                // !!! FIXME: Abort if this fails to build?
            else
            {
                rulesList[ruleCount] = XMLLexer::buildRules(kid);
                if (rulesList[ruleCount] == NULL)
                {
                    for (size_t j = 0; j < ruleCount; j++)
                        delete rulesList[j];
                    delete[] rulesList;
                    delete(_tokRule);
                    return(NULL);
                } // if
                else
                {
                    if (strcmp(tag, "element") == 0)
                    {
                        const char *elemName = kid->getAttributeValue("name");
                        if ( (elemName) && (strcmp(elemName, firstName) == 0) )
                            firstRule = rulesList[ruleCount];
                    } // if
                    ruleCount++;
                } // else
            } // else
        } // for
        rulesList[ruleCount] = NULL;
    } // if

    return(
        new LanguageRulesXML(_name, _ext, firstRule, _tokRule,
                             ruleCount, rulesList)
    );
} // LanguageRulesXML::buildRules


LanguageRulesXML::LanguageRulesXML(const char *_name, const char *_ext,
                                   LexerRules *_firstElem,
                                   LexerRules *_tokRule,
                                   int numKids, LexerRules **kids)
    : LanguageRules(_name, _ext, _firstElem, _tokRule, numKids, kids)
{
} // Constructor


LanguageRulesXML::~LanguageRulesXML(void)
{
} // Destructor


    // C++ code to declare the module-scope variable that will hold the
    //  constructed LanguageRules, and a declaration of a module-scope function
    //  that will do the constructing.
const char *LanguageRulesXML::outputDeclarations(void)
{
    TobyString str;

    str.append("static LanguageRules *language_");
    str.append(name);
    str.append(" = NULL;\n");
    str.append("static LanguageRules *buildLanguage_");
    str.append(name);
    str.append("(void);\n\n");

    for (size_t i = 0; i < numChildren; i++)
    {
        const char *kidDeclarations = children[i]->outputDeclarations();
        str.append(kidDeclarations);
        delete[] kidDeclarations;
    } // for

    const char *tokDeclarations = tokRule->outputDeclarations();
    str.append(tokDeclarations);
    delete[] tokDeclarations;

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // LanguageRulesXML::outputDeclarations


    // C++ code that defines the module-scope function that constructs the
    //  element, and stores a copy in the module-scope variable we declared
    //  in outputDeclarations()...
const char *LanguageRulesXML::outputDefinitions(void)
{
    TobyString str;

    for (size_t i = 0; i < numChildren; i++)
    {
        const char *kidDefinitions = children[i]->outputDefinitions();
        str.append(kidDefinitions);
        delete[] kidDefinitions;
    } // for

    const char *tokDefinitions = tokRule->outputDefinitions();
    str.append(tokDefinitions);
    delete[] tokDefinitions;

    str.append("static LanguageRules *buildLanguage_");
    str.append(name);
    str.append("(void)\n{\n");

    str.append("    assert(language_");
    str.append(name);
    str.append(" == NULL);\n\n");

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

    str.append("\n    language_");
    str.append(name);
    str.append(" = new LanguageRules(\"");
    str.append(name);
    str.append("\", \"");
    str.append(extension);
    str.append("\", ");

    if (firstElement == NULL)
        str.append("NULL, ");
    else
    {
        str.append("element_");
        str.append(((ElementRulesXML *) firstElement)->getName());
        str.append(", ");
    } // else

    if (tokRule == NULL)
        str.append("NULL, ");
    else
    {
        str.append(tokRule->outputConstructor());
        str.append(", ");
    } // else

    str.append(numChildren);
    str.append(", kids);\n");

    str.append("    return(language_");
    str.append(name);
    str.append(");\n} // buildLanguage_");
    str.append(name);
    str.append("\n\n");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // LanguageRulesXML::outputDefinitions


const char *LanguageRulesXML::outputConstructor(void)
{
    TobyString str("buildLanguage_");
    str.append(name);
    str.append("()");
    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // LanguageRulesXML::outputConstructor


const char *LanguageRulesXML::outputResolutions(void)
{
    TobyString str;

    const char *tokResolutions = tokRule->outputResolutions();
    str.append(tokResolutions);
    delete[] tokResolutions;

    for (size_t i = 0; i < numChildren; i++)
    {
        const char *kidResolutions = children[i]->outputResolutions();
        str.append(kidResolutions);
        delete[] kidResolutions;
    } // for

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // LanguageRulesXML::outputResolutions


// end of LanguageRulesXML.cpp ...

