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

#include "util/TobyCollection.h"


TobyCollection::TobyCollection(void) : list(NULL)
{
} // Constructor


TobyCollection::~TobyCollection(void)
{
    TobyLinkedList *next = NULL;

    while (list != NULL)
    {
        next = list->next;
        delete list;
        list = next;
    } // while
} // Destructor


void TobyCollection::insertElement(TobyObject *elem, int pos)
{
    int count = 0;

    for (TobyLinkedList *i = list; i != NULL; i = i->next)
    {
        if (count == pos)
        {
            TobyLinkedList *node = new TobyLinkedList;
            assert(node != NULL);

            node->obj = elem;
            node->next = i;
            node->prev = i->prev;
            i->prev = node;
            if (node->prev != NULL)
                node->prev->next = node;

            return;
        } // if

        count++;
    } // for
} // TobyCollection::insertElement


TobyObject *TobyCollection::elementAt(int pos)
{
    int count = 0;

    for (TobyLinkedList *i = list; i != NULL; i = i->next)
    {
        if (count == pos)
            return(i->obj);

        count++;
    } // for

    return(NULL);
} // TobyCollection::insertElement


TobyObject *TobyCollection::remove(int pos)
{
    int count = 0;

    for (TobyLinkedList *i = list; i != NULL; i = i->next)
    {
        if (count == pos)
        {
            TobyObject *retval = i->obj;
            if (i->prev != NULL)
                i->prev->next = i->next;

            if (i->next != NULL)
                i->next->prev = i->prev;

            delete i;
            return(retval);
        } // if
        count++;
    } // for

    return(NULL);
} // TobyCollection::remove


bool TobyCollection::isEmpty(void)
{
    return(list == NULL);
} // TobyCollection::isEmpty


int TobyCollection::size(void)
{
    // O(n), not O(1).
    int retval = 0;

    for (TobyLinkedList *i = list; i != NULL; i = i->next)
        retval++;

    return(retval);
} // TobyCollection::size

// end of TobyCollection.cpp ...

