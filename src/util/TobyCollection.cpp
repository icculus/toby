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


// !!! FIXME : This whole thing seems really inefficient.


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


bool TobyCollection::insertElement(void *elem, int pos)
{
    int count = 0;
    TobyLinkedList *prev = NULL;

    for (TobyLinkedList *i = list; i != NULL; i = i->next)
    {
        if (count == pos)
        {
            TobyLinkedList *node = new TobyLinkedList;
            node->obj = elem;
            node->next = i->next;
            node->prev = i;
            i->next = node;
            return(true);
        } // if

        prev = i;
        count++;
    } // for

        // check end of list addition.
        // !!! FIXME: Duplicate code.
    if (count == pos)
    {
        TobyLinkedList *node = new TobyLinkedList;
        assert(node != NULL);

        node->obj = elem;
        node->next = NULL;
        node->prev = prev;
        if (prev == NULL)
            list = node;
        else
            prev->next = node;
        return(true);
    } // if

    return(false);
} // TobyCollection::insertElement


// !!! really inefficient.
void TobyCollection::addElement(void *elem)
{
    insertElement(elem, size());
} // TobyCollection::addElement


void *TobyCollection::elementAt(int pos)
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


void *TobyCollection::remove(int pos)
{
    int count = 0;

    for (TobyLinkedList *i = list; i != NULL; i = i->next)
    {
        if (count == pos)
        {
            void *retval = i->obj;
            if (i->prev != NULL)
                i->prev->next = i->next;
            else
                list = i->next;

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
    // O(1) not O(n).
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


void TobyCollection::swapElements(int pos1, int pos2)
{
    int count = 0;
    TobyLinkedList *elem1 = NULL;
    TobyLinkedList *elem2 = NULL;

    for (TobyLinkedList *i = list; i != NULL; i = i->next)
    {
        if (count == pos1)
            elem1 = i;
        else if (count == pos2)
            elem2 = i;

        if ((elem1 != NULL) && (elem2 != NULL))
        {
            void *tmp = elem1->obj;
            elem1->obj = elem2->obj;
            elem2->obj = tmp;
            return;
        } // if

        count++;
    } // for
} // TobyCollection::swapElement

// end of TobyCollection.cpp ...

