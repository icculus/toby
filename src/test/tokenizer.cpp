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
 * This is a standalone program to test the Tokenizer class.
 *   Written by Ryan C. Gordon (icculus@clutteredmind.org)
 */

#include <stdio.h>

#include "io/FileReader.h"
#include "io/Tokenizer.h"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("USAGE: %s <file1> [file2] ... [fileN]\n", argv[0]);
        return(42);
    } // if

    for (int i = 1; i < argc; i++)
    {
        try
        {
            printf("+ NEW FILE: [%s].\n", argv[i]);

            Tokenizer toker(new FileReader(argv[i]));

            do
            {
                Tokenizer::tokentype rc = toker.nextToken();
                if (rc != toker.ttype)
                {
                    printf("- Return value (%d) does NOT match"
                           " toker.ttype (%d).  ?! <-- BUG.\n",
                            rc, toker.ttype);
                    break;
                } // if

                switch (toker.ttype)
                {
                    case Tokenizer::TT_NONE:
                        printf("* TT_NONE.  ?!  <-- BUG.\n");
                        break;

                    case Tokenizer::TT_WORD:
                        printf("* TT_WORD. [%s].\n", toker.str);
                        break;

                    case Tokenizer::TT_NUMBER:
                        printf("* TT_NUMBER. [%s] (%f).\n", toker.str, toker.num);
                        break;

                    case Tokenizer::TT_NEWLINE:
                        printf("* TT_NEWLINE.\n");
                        break;

                    case Tokenizer::TT_WHITESPACE:
                        printf("* TT_WHITESPACE. [%s].\n", toker.str);
                        break;

                    case Tokenizer::TT_SINGLELINECOMMENT:
                        printf("* TT_SINGLELINECOMMENT. [%s].\n", toker.str);
                        break;

                    case Tokenizer::TT_MULTILINECOMMENT:
                        printf("* TT_MULTILINECOMMENT. [%s].\n", toker.str);
                        break;

                    case Tokenizer::TT_EOF:
                        printf("* TT_EOF.\n");
                        break;

                    case Tokenizer::TT_LITERALSTRING:
                        printf("* TT_LITERALSTRING. [%s].\n", toker.str);
                        break;

                    default:
                        printf("- Unknown token (%d).  ?!  <-- BUG.\n", rc);
                        break;
                } // switch
            } while (toker.ttype != Tokenizer::TT_EOF);
        } // try

        catch (IOException *ioe)
        {
            printf("- IOException! [%s].\n", ioe->getMessage());
            delete ioe;
        } // catch
    } // for

    return(0);
} // main

// end of tokenizer.cpp ...

