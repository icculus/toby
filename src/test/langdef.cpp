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
 * This is a standalone program to test that a langdef file is valid.
 *  Ideally, we'd want to write up a DTD, and have some external tool do
 *  this for us, but oh well.
 *
 *   Written by Ryan C. Gordon (icculus@clutteredmind.org)
 */

#include <stdio.h>

#include "io/FileReader.h"
#include "exceptions/IOException.h"
#include "xml/XMLTree.h"
#include "xml/XMLNode.h"

static int warnings = 0;
static int errors = 0;

typedef struct
{
    char *tag;
    char **kids;
} tagslist;

static char *languagekids[] = { "tokenizer", "element", NULL };

static char *elementkids[] =  { "element", "repeat", "optional", "pickone",
                                "reqelement", "reqword", "reqnewline",
                                "reqnumber", "reqliteralstring",
                                "reqmultilinecomment", "reqsinglelinecomment",
                                "reqchar", "reqeof", "reqwhitespace", NULL };

static char *repeatkids[] =   { "repeat", "optional", "pickone",
                                "reqelement", "reqword", "reqnewline",
                                "reqnumber", "reqliteralstring",
                                "reqmultilinecomment", "reqsinglelinecomment",
                                "reqchar", "reqeof", "reqwhitespace", NULL };

static char *nokids[] =       { NULL };

static tagslist tags[] =
{
    { "language", languagekids },
    { "tokenizer", nokids },
    { "element", elementkids },
    { "repeat", repeatkids },
    { "optional", repeatkids },
    { "pickone", repeatkids },
    { "reqelement", nokids },
    { "reqword", nokids },
    { "reqliteralstring", nokids },
    { "reqsinglelinecomment", nokids },
    { "reqmultilinecomment", nokids },
    { "reqchar", nokids },
    { "reqnumber", nokids },
    { "reqnewline", nokids },
    { "reqeof", nokids },
    { NULL, NULL }
};


static void testNode(XMLNode *node)
{
    const char *tag = node->getTag();

    for (int i = 0; tags[i].tag != NULL; i++)
    {
        if (strcmp(tags[i].tag, tag) == 0)
        {
            TobyCollection *children = node->getChildren();
            int max = children->size();
            for (int j = 0; j < max; j++)
            {
                XMLNode *kid = (XMLNode *) children->elementAt(j);
                testNode(kid);
                const char *kidtag = kid->getTag();
                int q;
                for (q = 0; tags[i].kids[q] != NULL; q++)
                {
                    if (strcmp(tags[i].kids[q], kidtag) == 0)
                        break;
                } // for

                if (tags[i].kids[q] == NULL)
                {
                    printf("[%s] is not a valid child of [%s].\n", kidtag, tag);
                    errors++;
                } // if
            } // for
            return;
        } // if
    } // for

    printf("- Unrecognized tag [%s].\n", tag);
    errors++;
} // testNode


static void testLangTree(XMLNode *head)
{
    if (strcmp(head->getTag(), "language") != 0)
    {
        printf("- Toplevel tag must be <language>.\n");
        errors++;
        return;
    } // if

    testNode(head);
} // testLangTree


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
            printf("\n+ NEW FILE: [%s].\n", argv[i]);

            XMLTree tree(new FileReader(argv[i]));

            if (!tree.parseXMLThrowException())
                printf("- Document is not well-formed.\n");
            else
            {
                XMLNode *root = tree.getRootNode();
                assert(root->getTag() == NULL);
                assert(root->getText() == NULL);
                TobyCollection *children = root->getChildren();
                int max = children->size();
                for (int j = 0; j < max; j++)
                    testLangTree((XMLNode *) children->elementAt(j));
            } // else
        } // try

        catch (IOException *ioe)
        {
            printf("- IOException! [%s].\n", ioe->getMessage());
            errors++;
            delete ioe;
        } // catch
    } // for

    printf("\n");
    printf("%d errors.\n", errors);
    printf("%d warnings.\n", warnings);
    printf("\n");

    return(0);
} // main

// end of tokenizer.cpp ...

