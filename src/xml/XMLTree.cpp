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

#include "xml/XMLTree.h"

        // Construct an XML object that will read XML data from (xmlStream).
        //  Note that the parsed tree isn't available until you get a
        //  successful return from parseTree(). Do not delete (xmlStream);
        //  TobyXML will do that. This means you shouldn't make it a local
        //  object, either.
XMLTree::XMLTree(TobyReader *xmlStream)
    : root(NULL),
      in(xmlStream)
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


// !!! FIXME: This is pretty ugly.
bool XMLTree::parseTagText(XMLNode *node, Tokenizer *toker)
                                    throw (IOException *)
{
    bool retval = true;
    TobyString text;

    toker->setIgnoreWhitespace(false);

    while (true)
    {
        toker->nextToken();
        if ((toker->ttype == Tokenizer::TT_WORD) ||
            (toker->ttype == Tokenizer::TT_WHITESPACE))
        {
            if (strcmp(toker->str, "<") == 0)  // end of text?
            {
                toker->pushBack();
                break;
            } // if
            else
            {
                text.append(toker->str);
            } // else
        } // if
        else
        {
            retval = false;
            break;
        } // else
    } // while

    toker->setIgnoreWhitespace(true);

    if (retval)
        node->setText(text.c_str());

    return(retval);
} // XMLTree:parseTagText


bool XMLTree::parseTagAttributes(XMLNode *node, Tokenizer *toker)
                                            throw (IOException *)
{
    TobyCollection *attribs = node->getAttributes();

    while (true)
    {
        if (toker->nextToken() != Tokenizer::TT_WORD)
            return(false);

        if ( (strcmp(toker->str, ">") == 0) || (strcmp(toker->str, "/") == 0) )
        {
            toker->pushBack();
            return(true);
        } // if

        TobyString name(toker->str);

        if (!toker->mustGetWord("="))
            return(false);

        if (toker->nextToken() != Tokenizer::TT_LITERALSTRING)
            return(false);

        TobyString value(toker->str + 1);     // lose first quote char.
        value.substr(0, value.length() - 1);  // lose last quote char.

        XMLAttribute *attr = new XMLAttribute;
        attr->name = new char[name.length() + 1];
        strcpy((char *) attr->name, name.c_str());
        attr->value = new char[value.length() + 1];
        strcpy((char *) attr->value, value.c_str());
        attribs->addElement(attr);
    } // while

    return(false);  // Shouldn't ever hit this point.
} // XMLTree::parseTagAttributes


bool XMLTree::parseTag(XMLNode *parent, Tokenizer *toker) throw (IOException *)
{
    // First token should be the tagname...

    if (toker->nextToken() != Tokenizer::TT_WORD)
        return(false);

    // If anything fails in here, the tree will be deleted, taking this
    //  half-constructed node with it, so we don't have to concern ourselves
    //  with cleanup. Nodes add themselves to their parent at construction,
    //  so there's a reference to delete later...

    XMLNode *node = new XMLNode(parent);
    if (parent == NULL)
        root = node;

    node->setTag(toker->str);
    if (!parseTagAttributes(node, toker))
        return(false);

    // Next chars should be "/>" or just ">" ...

    if (toker->nextToken() != Tokenizer::TT_WORD)
        return(false);

    if (strcmp(toker->str, "/") == 0)  // self-contained tag?
        return(toker->mustGetWord(">"));

    else if (strcmp(toker->str, ">") == 0)  // complete tag?
    {
        parseTagText(node, toker);

        while (true)
        {
                // must be either an endtag, or a child tag...
            if (!toker->mustGetWord("<"))
                return(false);
    
            if (toker->nextToken() != Tokenizer::TT_WORD)
                return(false);

            if (strcmp(toker->str, "/") == 0)  // it's an endtag.
            {
                if (!toker->mustGetWord(node->getTag()))
                    return(false);

                if (!toker->mustGetWord(">"))
                    return(false);

                return(true);  // we're done!
            } // if

            // ...it's a child tag...
            toker->pushBack();
            if (!parseTag(node, toker))
                return(false);
        } // while
    } // else if

    return(false);
} // XMLTree::parseTag


bool XMLTree::parseHeader(Tokenizer *toker) throw (IOException *)
{
#if 1  // !!! FIXME: Do we REALLY want to strive for XML 1.0 compliance, here?
    if ( (!toker->mustGetWord("<")) ||
         (!toker->mustGetWord("?")) ||
         (!toker->mustGetWord("xml")) ||
         (!toker->mustGetWord("version")) ||
         (!toker->mustGetWord("=")) )
    {
        return(false);
    } // if

    if (toker->nextToken() != Tokenizer::TT_LITERALSTRING)
        return(false);

    _D(("XMLTree: Document version=%s ...\n", toker->str));

    if ( (!toker->mustGetWord("encoding")) || (!toker->mustGetWord("=")) )
        return(false);

    if (toker->nextToken() != Tokenizer::TT_LITERALSTRING)
        return(false);

    _D(("XMLTree: Document encoding=%s ...\n", toker->str));

    if ( (!toker->mustGetWord("?")) || (!toker->mustGetWord(">")) )
        return(false);
#endif

    return(true);
} // XMLTree::parseHeader


bool XMLTree::_parseXMLThrowException(void) throw (IOException *)
{
    if (in == NULL)
        return(root != NULL);  // return whatever last call returned.

    _D(("XMLTree: parsing an XML document...\n"));

    Tokenizer toker(in);
    in = NULL;  // When toker deconstructs, it will take (in) with it.

        // We initially ignore whitespace, but when filling in the
        //  text field of an XMLNode, we'll pay attention to it.
        //  Note that we consider newline chars to be whitespace, so that
        //  we don't have to handle Tokenizer::TT_NEWLINE separately.
    toker.setWhitespaceChars(" \t\r\n", 4);
    toker.setIgnoreWhitespace(true);
    toker.setSingleLineComment(NULL);
    toker.setIgnoreSingleLineComments(true);
    toker.setMultiLineComment("<!--", "-->");
    toker.setIgnoreMultiLineComments(true);
    toker.setQuoteChars("\"", 1);
    toker.setEscapeChar('\\');
    toker.setEscaping(true);
    toker.setConvertNumbers(false);
    toker.setNumbersAreWords(true);

    if (!parseHeader(&toker))
    {
        _D(("XMLTree: invalid header on line #%d.\n", toker.currentLine()));
        return(false);
    } // if

    // Must be a valid XML tag at this point...
    if (!toker.mustGetWord("<"))
        return(false);

    if (!parseTag(NULL, &toker))  // This recurses through the XML tree.
    {
        _D(("XMLTree: document is not well-formed on line #%d.\n",
             toker.currentLine()));
        return(false);
    } // if

    return(true);
} // XMLTree::_parseXMLThrowException


    // This just makes sure that (this->root) gets cleaned up on error.
bool XMLTree::parseXMLThrowException(void) throw (IOException *)
{
    bool retval = false;
    try
    {
        retval = _parseXMLThrowException();
        if ((!retval) && (root != NULL))
        {
            delete root;
            root = NULL;
        } // if
    } // try
    catch (IOException *ioe)
    {
        if (root != NULL)
        {
            delete root;
            root = NULL;
        } // if
        throw(ioe);
    } // catch

    return(retval);
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

