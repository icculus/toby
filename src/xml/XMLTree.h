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

#ifndef _INCLUDE_XMLTREE_H_
#define _INCLUDE_XMLTREE_H_

#include "util/TobyObject.h"
#include "util/TobyString.h"
#include "xml/XMLNode.h"
#include "exceptions/IOException.h"
#include "io/TobyReader.h"
#include "io/Tokenizer.h"

/*
 *  A quick-and-dirty XML parser.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class XMLTree : public TobyObject
{
public:
        // Construct an XML object that will read XML data from (xmlStream).
        //  Note that the parsed tree isn't available until you get a
        //  successful return from parseTree(). Do not delete (xmlStream);
        //  XMLTree will do that. This means you shouldn't make it a local
        //  object, either.
    XMLTree(TobyReader *xmlStream);

        // Destructor.
    virtual ~XMLTree(void);

        // Parse the XML stream. Returns (true) if successfully parsed,
        //  (false) if the stream was not well-formed or there was an
        //  IOException. If you want to catch the IOException yourself,
        //  call parseXMLThrowException() instead.
    virtual bool parseXML(void);

        // Same as above, but allows you to handle IOExceptions.
    virtual bool parseXMLThrowException(void) throw (IOException *);

        // Consider this, and all it's children, to be read only.
    virtual XMLNode *getRootNode(void);

private:
    XMLNode *root;
    TobyReader *in;

    bool parseTagText(XMLNode *node, Tokenizer *toker) throw (IOException *);
    bool parseTag(XMLNode *parent, Tokenizer *toker) throw (IOException *);
    bool parseHeader(Tokenizer *toker) throw (IOException *);
    bool _parseXMLThrowException(void) throw (IOException *);
    bool parseTagAttributes(XMLNode *node, Tokenizer *toker)
                                            throw (IOException *);
}; // class XMLTree

#endif // !defined _INCLUDE_XMLTREE_H_

// end of XMLTree.h ...

