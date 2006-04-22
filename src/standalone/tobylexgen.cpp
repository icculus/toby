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


/*
 * This is a program that takes a list of XML files that define lexers, and
 *  spits out C++ code that would build the lexer at runtime without the
 *  XML file and, more importantly, without the XML-handling bulk. This is
 *  intended for static targets that won't need to add new programming
 *  languages without recompiling, and resource-constrained/embedded targets.
 *
 *   Written by Ryan C. Gordon (icculus@icculus.org)
 */

#include <new>
#include <stdio.h>
#include "xml/XMLTree.h"
#include "io/FileReader.h"
#include "platform/lexers/xmllexer/XMLLexer.h"


static void outputIntro(LanguageRules *lang)
{
    printf(
"/*\n"
" * TOBY -- An abstract interpreter engine and system for learning.\n"
" * Copyright (C) 1999  Ryan C. Gordon.\n"
" *\n"
" * This program is free software; you can redistribute it and/or\n"
" * modify it under the terms of the GNU General Public License\n"
" * as published by the Free Software Foundation; either version 2\n"
" * of the License, or (at your option) any later version.\n"
" *\n"
" * This program is distributed in the hope that it will be useful,\n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
" * GNU General Public License for more details.\n"
" *\n"
" * You should have received a copy of the GNU General Public License\n"
" * along with this program; if not, write to the Free Software\n"
" * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.\n"
" */\n"
"\n"
"/*\n"
" * This is computer-generated code. DO NOT MANUALLY EDIT.\n"
" *\n"
" * This is generated from an XML file that specifies lexing rules for\n"
" *  a given language.\n"
" *\n"
" *    Language : %s\n"
" *    Language's file extension: *.%s\n"
" *\n"
" *   Code generator written by Ryan C. Gordon (icculus@icculus.org)\n"
" */\n"
"\n"
"#include \"util/TobyObject.h\"\n"
"#include \"util/TobyLanguage.h\"\n"
"#include \"lexer/Lexer.h\"\n"
"#include \"lexer/LexerRules.h\"\n"
"#include \"lexer/LanguageRules.h\"\n"
"#include \"lexer/ElementRules.h\"\n"
"#include \"lexer/TokenizerRules.h\"\n"
"#include \"lexer/RepeatRules.h\"\n"
"#include \"lexer/PickOneRules.h\"\n"
"#include \"lexer/ReqWordRules.h\"\n"
"#include \"lexer/ReqEOFRules.h\"\n"
"#include \"lexer/ReqNewlineRules.h\"\n"
"#include \"lexer/ReqLiteralStringRules.h\"\n"
"#include \"lexer/ReqWhitespaceRules.h\"\n"
"#include \"lexer/ReqSingleLineCommentRules.h\"\n"
"#include \"lexer/ReqMultiLineCommentRules.h\"\n"
"#include \"lexer/ReqNumberRules.h\"\n"
"#include \"lexer/ReqWordCharsRules.h\"\n"
"#include \"lexer/ReqCharRules.h\"\n"
"#include \"lexer/ReqElementRules.h\"\n"
"\n"
"\n",
    lang->getName(), lang->getFileExtension());
} // outputIntro


static void outputCode(LanguageRules *lang)
{
    const char *langName = lang->getName();

    outputIntro(lang);

    printf("    // Declarations...\n\n");
    printf("%s", lang->outputDeclarations());
    printf("    // Functions...\n\n");
    printf("%s\n", lang->outputDefinitions());
    printf("    // Call this to build the language...\n\n");
    printf("LexerRules *staticlang_%s(void)\n{\n", langName);
    printf("    LexerRules *retval = %s;\n", lang->outputConstructor());
    printf("    if (retval == NULL)\n");
    printf("        return(NULL);\n\n");
    printf("    // resolve dependencies...\n\n");
    printf("%s", lang->outputResolutions());
    printf("\n    return(retval);\n} // staticlang_%s\n\n", langName);
} // outputCode


int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "USAGE: %s <file1> [file2] ... [fileN]\n", argv[0]);
        return(42);
    } // if

    XMLLexer xmlLexer;

    for (int i = 1; i < argc; i++)
    {
        fprintf(stderr, "\n+ NEW FILE: [%s].\n", argv[i]);

        try
        {
            XMLTree tree(new FileReader(argv[i]));

            if (!tree.parseXMLThrowException())
                fprintf(stderr, "- Document (%s) is not well-formed.\n", argv[i]);
            else
            {
                LexerRules **langs = xmlLexer.buildLanguages(&tree);
                for (size_t i = 0; langs[i] != NULL; i++)
                {
                    outputCode((LanguageRules *) langs[i]);
                    delete langs[i];
                } // for
                delete[] langs;
            } // else
        } // try

        catch (IOException *ioe)
        {
            fprintf(stderr, "- IOException! [%s].\n", ioe->getMessage());
            delete ioe;
        } // catch
    } // for

    printf("\n");
    return(0);

} // main

// end of tobylexgen.cpp ...

