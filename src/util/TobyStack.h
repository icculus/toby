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

#ifndef _INCLUDE_TOBYSTACK_H_
#define _INCLUDE_TOBYSTACK_H_

#include "util/TobyCollection.h"

/*
 * This is a general object stack. First in, last out.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class TobyStack : public TobyCollection
{
public:
//    TobyStack(void);
//    virtual ~TobyStack(void);
    virtual void push(void *elem);
    virtual void *pop(void);
}; // class TobyStack

#endif  // !defined _INCLUDE_TOBYSTACK_H_

// end of TobyStack.h ...

