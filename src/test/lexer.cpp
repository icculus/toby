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
 *
 *   Written by Ryan C. Gordon (icculus@icculus.org)
 */

#include <stdio.h>
#include <stdarg.h>

#include "io/FileReader.h"
#include "exceptions/IOException.h"
#include "lexer/Lexer.h"

static int warnings = 0;
static int errors = 0;

static void info(char *fmt, ...)
{
    va_list ap;
    printf("* ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
} // info


static void warning(char *fmt, ...)
{
    va_list ap;
    printf("- ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    warnings++;
} // warning


static void error(char *fmt, ...)
{
    va_list ap;
    printf("- ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    errors++;
} // error


static void doLexing(LanguageRules *langRules, const char *filename)
                                                throw (IOException *)
{
    Tokenizer toker(new FileReader(filename));
    langRules->testRules(&toker);
} // doLexing


int main(int argc, char **argv)
{
    if (argc == 1)
    {
        info("Usage is %s <file1> [file2] ... [fileN]", argv[0]);
        return(42);
    } // if

    Lexer *lexer = Lexer::getInstance();
    lexer->loadLanguages();

    printf("\n");

    for (int i = 1; i < argc; i++)
    {
        try
        {
            info("New file: [%s].", argv[i]);

            const char *ext = strrchr(argv[i], '.');
            if (!ext)
                error("No file extension!");
            else
            {
                ext++;  // get past '.' ...
                LanguageRules *langRules;
                langRules = lexer->getLanguageRulesByExtension(ext);
                if (langRules == NULL)
                    error("No lexer registered for *.%s files.\n", ext);
                else
                    doLexing(langRules, argv[i]);
            } // else
        } // try

        catch (IOException *ioe)
        {
            error("IOException! [%s].", ioe->getMessage());
            delete ioe;
        } // catch
    } // for

    printf("\n");
    info("%d errors.", errors);
    info("%d warnings.", warnings);
    printf("\n");

    if (warnings < 0)
        warning("This prevents a compiler warning for now.");

    if (errors < 0)
        error("This prevents a compiler warning for now.");

    return(0);
} // main

// end of tokenizer.cpp ...

