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

#include "xml/XMLNode.h"

XMLNode::XMLNode(XMLNode *_parent)
    : tag(NULL),
      text(NULL),
      attribs(new TobyCollection()),
      parent(_parent),
      children(new TobyCollection())
{
    if (parent != NULL)
    {
        TobyCollection *c = parent->getChildren();
        c->addElement((void *) this);
    } // if
} // Constructor


XMLNode::~XMLNode(void)
{
    delete[] tag;
    delete[] text;

    while (!attribs->isEmpty())
    {
        XMLAttribute *attr = (XMLAttribute *) attribs->remove(0);
        delete[] attr->name;
        delete[] attr->value;
        delete attr;
    } // while
    delete attribs;

    while (!children->isEmpty())
        delete (XMLNode *) children->remove(0);

    delete children;
} // Destructor


void XMLNode::setTag(const char *str)
{
    delete[] tag;

    if (str == NULL)
        tag = NULL;
    else
    {
        tag = new char[strlen(str) + 1];
        strcpy((char *) tag, str);
    } // else
} // XMLNode::setTag


const char *XMLNode::getTag(void)
{
    return(tag);
} // XMLNode::getTag


void XMLNode::setText(const char *str)
{
    delete[] text;

    if (str == NULL)
        text = NULL;
    else
    {
        text = new char[strlen(str) + 1];
        strcpy((char *) text, str);
    } // else
} // XMLNode::setText


const char *XMLNode::getText(void)
{
    return(text);
} // XMLNode::getText


TobyCollection *XMLNode::getAttributes(void)
{
    return(attribs);
} // XMLNode::getAttribs


TobyCollection *XMLNode::getChildren(void)
{
    return(children);
} // XMLNode::getChildren


XMLNode *XMLNode::getParent(void)
{
    return(parent);
} // XMLNode::getParent


const char *XMLNode::getAttributeValue(const char *name)
{
    int max = attribs->getSize();
    for (int i = 0; i < max; i++)
    {
        XMLAttribute *attr = (XMLAttribute *) attribs->elementAt(i);
        if (strcmp(attr->name, name) == 0)
            return(attr->value);
    } // for

    return(NULL);
} // XMLNode::getAttributeValue


const char *XMLNode::getAttributeDefault(const char *name, const char *defval)
{
    const char *retval = getAttributeValue(name);
    if (retval == NULL)
        retval = defval;

    return(retval);
} // XMLNode::getAttributeDefault


bool XMLNode::getBoolAttribute(const char *attr, bool defval)
{
    const char *defstr = (defval) ? "true" : "false";
    const char *str = getAttributeDefault(attr, defstr);

    if (strcmp(str, "true") == 0)
        return(true);
    else if (strcmp(str, "false") == 0)
        return(false);
    else if (strcmp(str, "yes") == 0)
        return(true);
    else if (strcmp(str, "no") == 0)
        return(false);
    else if (strcmp(str, "1") == 0)
        return(true);
    else if (strcmp(str, "0") == 0)
        return(false);
    else if (strcmp(str, "on") == 0)
        return(true);
    else if (strcmp(str, "off") == 0)
        return(false);

    assert(false);   // shouldn't ever hit this line.
    return(false);
} // XMLNode::getBoolAttrib


// end of XMLNode.cpp ...

