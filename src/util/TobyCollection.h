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
 *
 *     Written by Ryan C. Gordon. (icculus@linuxgames.com)
 */

// internally, this is just a linked list.
typedef struct TobyLinkedListStruct
{
    TobyObject *obj;
    struct TobyLinkedListStruct *prev;
    struct TobyLinkedListStruct *next;
} TobyLinkedList;

class TobyCollection : public TobyObject
{
public:
    TobyCollection(void);
    virtual ~TobyCollection(void);
    virtual void insertElement(TobyObject *elem, int pos);
    virtual TobyObject *elementAt(int pos);
    virtual TobyObject *remove(int pos);
    virtual bool isEmpty(void);
    virtual int size(void);

protected:
    TobyLinkedList *list;
}; // class TobyCollection

#endif  // !defined _INCLUDE_TOBYCOLLECTION_H_

// end of TobyCollection.h ...
