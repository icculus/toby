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

#include "platform/lexers/xmllexer/RepeatRulesXML.h"

unsigned long RepeatRulesXML::nextRepeatID;


static int getMinMaxAttr(XMLNode *node, const char *attr, const char *defval)
{
    const char *rc = node->getAttributeDefault(attr, defval);
    if (strcmp(rc, "n") == 0)
        return(-1);

    return(atoi(rc));
} // getMinMaxAttr


LexerRules *RepeatRulesXML::buildRules(XMLNode *node, int _minval, int _maxval)
{
    size_t max;
    LexerRules **rulesList = XMLLexer::buildBasicChildRules(node, &max);
    return(new RepeatRulesXML(_minval, _maxval, max, rulesList));
} // buildRules


LexerRules *RepeatRulesXML::buildRulesRepeatTag(XMLNode *node)
{
    assert(strcmp(node->getTag(), "repeat") == 0);  // just in case.
    int _minval = getMinMaxAttr(node, "min", "0");
    int _maxval = getMinMaxAttr(node, "max", "n");
    return(buildRules(node, _minval, _maxval));
} // RepeatRulesXML::buildRules


LexerRules *RepeatRulesXML::buildRulesOptionalTag(XMLNode *node)
{
    assert(strcmp(node->getTag(), "optional") == 0);  // just in case.
    return(buildRules(node, 0, 1));
} // RepeatRulesXML::buildRulesOptionalTag


RepeatRulesXML::RepeatRulesXML(int _minval, int _maxval, size_t numKids,
                                LexerRules **kids)
    : RepeatRules(_minval, _maxval, numKids, kids),
      repeatID(nextRepeatID)
{
    nextRepeatID++;
} // Constructor


RepeatRulesXML::~RepeatRulesXML(void)
{
} // Destructor


    // C++ code to declare the module-scope variable that will hold the
    //  constructed RepeatRules, and a declaration of a module-scope function
    //  that will do the constructing.
const char *RepeatRulesXML::outputDeclarations(void)
{
    TobyString str;

    str.append("static RepeatRules *repeat_");
    str.append(repeatID);
    str.append(" = NULL;\n");
    str.append("static RepeatRules *buildRepeat_");
    str.append(repeatID);
    str.append("(void);\n\n");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // RepeatRulesXML::outputDeclarations


    // C++ code that defines the module-scope function that constructs the
    //  element, and stores a copy in the module-scope variable we declared
    //  in outputDeclarations()...
const char *RepeatRulesXML::outputDefinitions(void)
{
    TobyString str;

    str.append("static RepeatRules *buildRepeat_");
    str.append(repeatID);
    str.append("(void)\n{\n");

    str.append("\n#ifdef DEBUG");
    str.append("    assert(repeat_");
    str.append(repeatID);
    str.append(" == NULL);\n#endif\n\n");

    str.append("    LexerRules **kids = new LexerRules *[");
    str.append((long) numChildren);
    str.append("];\n");

    for (size_t i = 0; i < numChildren; i++)
    {
        str.append("    kids[");
        str.append((long) i);
        str.append("] = ");
        const char *kidConstructor = children[i]->outputConstructor();
        str.append(kidConstructor);
        delete[] kidConstructor;
        str.append(";\n");
    } // for

    str.append("\n    repeat_");
    str.append(repeatID);
    str.append(" = new RepeatRules(");
    str.append(minRepeats);
    str.append(", ");
    str.append(maxRepeats);
    str.append(", ");
    str.append((long) numChildren);
    str.append(", kids);\n");

    str.append("return(repeat_");
    str.append(repeatID);
    str.append(");\n} // buildRepeat_");
    str.append(repeatID);
    str.append("\n\n");

    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // RepeatRulesXML::outputDefinitions


const char *RepeatRulesXML::outputConstructor(void)
{
    TobyString str("buildRepeat_");
    str.append(repeatID);
    str.append("()");
    char *retval = new char[str.length() + 1];
    strcpy(retval, str.c_str());
    return(retval);
} // RepeatRulesXML::outputConstructor

// end of RepeatRulesXML.cpp ...

