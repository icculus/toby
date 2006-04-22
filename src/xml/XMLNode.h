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

#ifndef _INCLUDE_XMLNODE_H_
#define _INCLUDE_XMLNODE_H_

#include "util/TobyObject.h"
#include "util/TobyCollection.h"

/*
 * One node in an XML tree.
 *
 *  XML nodes are pretty straightforward. If you have a chunk of XML like
 *   this:
 *
 *   <p align="center"><b>Hello, world!</b></p>
 *
 *   Then the node with the tag "p" would have one attribute ("align", with a
 *    value of "center", without quotes on either), one child ("b"), and no
 *    text (""). The "b" node has the text "Hello, world!". A node with a
 *    built in endtag ( <mynode attr="xxxx"/> ) will have a text of NULL.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
typedef struct
{
    const char *name;
    const char *value;
} XMLAttribute;


class XMLNode : public TobyObject
{
public:
    XMLNode(XMLNode *_parent);
    virtual ~XMLNode(void);

    void setTag(const char *str);
    const char *getTag(void);
    void setText(const char *str);
    const char *getText(void);
    TobyCollection *getAttributes(void);
    TobyCollection *getChildren(void);
    XMLNode *getParent(void);
    const char *getAttributeValue(const char *name);
    const char *getAttributeDefault(const char *name, const char *defval);
    bool getBoolAttribute(const char *attr, bool defval = false);

protected:
    const char *tag;
    const char *text;
    TobyCollection *attribs;
    XMLNode *parent;
    TobyCollection *children;
}; // class XMLNode

#endif // !defined _INCLUDE_XMLNODE_H_

// end of XMLNode.h ...


