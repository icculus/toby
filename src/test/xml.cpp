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
 * This is a standalone program to test the XML classes.
 *   Written by Ryan C. Gordon (icculus@clutteredmind.org)
 */

#include <stdio.h>

#include "io/FileReader.h"
#include "exceptions/IOException.h"
#include "xml/XMLTree.h"
#include "xml/XMLNode.h"


    // print (count) copies of (ch) to stdout.
void printMultiChars(char ch, int count)
{
    for (int i = 0; i < count; i++)
        printf("%c", ch);
} // printMultiChars


    // Spit back out the XML as the parser saw it.
void reoutputXML(XMLNode *node, int depth)
{
    int i, max;

    printf("*");
    printMultiChars(' ', (depth + 1) * 2);
    printf("<%s", node->getTag());

    TobyCollection *attribs = node->getAttributes();
    for (i = 0, max = attribs->size(); i < max; i++)
    {
        XMLAttribute *attr = (XMLAttribute *) attribs->elementAt(i);
        printf(" %s=\"%s\"", attr->name, attr->value);
    } // for

    const char *text = node->getText();
    TobyCollection *children = node->getChildren();
    if (text == NULL)
    {
        assert(children->isEmpty());
        printf(" />\n");
    } // if
    else
    {
        printf(">\n*");
        printMultiChars(' ', (depth + 2) * 2);
        printf("%s\n", text);

        for (i = 0, max = children->size(); i < max; i++)
        {
            XMLNode *child = (XMLNode *) children->elementAt(i);
            assert(child->getParent() == node);
            reoutputXML(child, depth + 1);
        } // for

        printf("*");
        printMultiChars(' ', (depth + 1) * 2);
        printf("</%s>\n", node->getTag());
    } // else
} // reoutputXML


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
                    reoutputXML((XMLNode *) children->elementAt(j), 0);
            } // else
        } // try

        catch (IOException *ioe)
        {
            printf("- IOException! [%s].\n", ioe->getMessage());
            delete ioe;
        } // catch
    } // for

    printf("\n");
    return(0);
} // main

// end of tokenizer.cpp ...

