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

#include "util/XMLTree.h"

        // Construct an XML object that will read XML data from (xmlStream).
        //  Note that the parsed tree isn't available until you get a
        //  successful return from parseTree(). Do not delete (xmlStream);
        //  TobyXML will do that. This means you shouldn't make it a local
        //  object, either.
XMLTree::XMLTree(TobyReader *xmlStream)
    : in(xmlStream),
      root(NULL)
{
} // Constructor


XMLTree::~XMLTree(void)
{
    if (in != NULL)
        delete in;

    if (root != NULL)
        delete root;
} // Destructor


XMLNode *XMLTree::getRootNode(void)
{
    return(root);
} // XMLTree::getRootNode


bool XMLTree::doParse(Tokenizer *toker) throw (IOException *)
{
    
} // XMLTree::doParse


bool XMLTree::parseXMLThrowException(void) throw (IOException *)
{
    if (in == NULL)
        return(false);

    Tokenizer toker(in);
    in = NULL;  // When toker deconstructs, it will take (in) with it.

        // We initially ignore whitespace, but when filling in the
        //  text field of an XMLNode, we'll pay attention to it.
        //  Note that we consider newline chars to be whitespace, so that
        //  we don't have to handle TT_NEWLINE separately.
    toker.setWhitespaceChars(" \t\r\n", 4);
    toker.setIgnoreWhitespace(true);
    toker.setSingleLineComment(NULL);
    toker.setIgnoreSingleLineComments(true);
    toker.setMultiLineComment("<!--", "-->");
    toker.setIgnoreMultiLineComments(true);
    toker.setQuoteChars("\"\'", 2);
    toker.setEscapeChar('\\');
    toker.setEscaping(true);
    toker.setConvertNumbers(false);
    toker.setNumbersAreWords(true);

    return(doParse(&toker));  // go to town.
} // XMLTree::parseXMLThrowException


bool XMLTree::parseXML(void)
{
    bool retval = false;

    try
    {
        retval = parseXMLThrowException();
    } // try
    catch (IOException *ioe)
    {
        _D(("XMLTree::parseXML() caught an IOException. [%s].\n",
            ioe->getMessage()));
        delete ioe;
        retval = false;
    } // catch

    return(retval);
} // XMLTree:parseXML

// end of XMLTree.cpp ...

