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

#include "util/XMLNode.h"

XMLNode::XMLNode(void)
    : tag(NULL),
      text(NULL),
      attribs(new HashTable()),
      parent(NULL),
      children(new TobyCollection())
{
} // Constructor


XMLNode::~XMLNode(void)
{
    if (tag != NULL)
        delete[] tag;

    if (text != NULL)
        delete[] text;

    delete attribs;

    while (!children->isEmpty())
    {
        XMLNode *node = (XMLNode *) children->remove();
        delete node;
    } // while

    delete children;
} // Destructor


void XMLNode::setTag(const char *str)
{
    if (tag != NULL)
        delete[] tag;

    if (str == NULL)
        tag = NULL;
    else
    {
        tag = new char[strlen(str) + 1];
        strcpy(tag, str);
    } // else
} // XMLNode::setTag


const char *XMLNode::getTag(void)
{
    return(tag);
} // XMLNode::getTag


void XMLNode::setText(const char *str)
{
    if (text != NULL)
        delete[] text;

    if (str == NULL)
        text = NULL;
    else
    {
        text = new char[strlen(str) + 1];
        strcpy(text, str);
    } // else
} // XMLNode::setText


const char *XMLNode::getText(void)
{
    return(text);
} // XMLNode::getText


HashTable *XMLNode::getAttribs(void)
{
    return(attribs);
} // XMLNode::getAttribs


TobyCollection *XMLNode::getChildren(void)
{
    return(children);
} // XMLNode::getChildren


TobyCollection *XMLNode::getParent(void)
{
    return(parent);
} // XMLNode::getParent

// end of XMLNode.cpp ...

