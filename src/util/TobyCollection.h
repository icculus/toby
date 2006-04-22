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

#ifndef _INCLUDE_TOBYCOLLECTION_H_
#define _INCLUDE_TOBYCOLLECTION_H_

#include "util/TobyObject.h"

/*
 * This is a base class for linear collections: lists, stacks, etc.
 *  For speed, these classes are NOT thread safe! If you are going to have
 *  two threads modifying a collection at the same time, rethink your design,
 *  and then add TobyMutexes to your calling code.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */

// internally, this is just a linked list.
typedef struct __S_TobyLinkedList
{
    void *obj;
    struct __S_TobyLinkedList *prev;
    struct __S_TobyLinkedList *next;
} TobyLinkedList;

class TobyCollection : public TobyObject
{
public:
    TobyCollection(void);
    virtual ~TobyCollection(void);
    virtual void addElement(void *elem);
    virtual bool insertElement(void *elem, size_t pos);
    virtual void *elementAt(size_t pos);
    virtual void *remove(size_t pos);
    virtual bool isEmpty(void);
    virtual void clear(void);
    virtual size_t getSize(void);
    virtual void swapElements(size_t pos1, size_t pos2);

protected:
    TobyLinkedList *list;
}; // class TobyCollection

#endif  // !defined _INCLUDE_TOBYCOLLECTION_H_

// end of TobyCollection.h ...

