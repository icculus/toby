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

#ifndef _INCLUDE_NULLMUTEX_H_
#define _INCLUDE_NULLMUTEX_H_

#include "util/TobyMutex.h"

/*
 * A null implementation of TobyMutex. Use this as a stub to get you going
 *  until you can implement your own platform-specify mutex implementation.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class NullMutex : public TobyMutex
{
public:
    NullMutex(void);
    virtual ~NullMutex(void);
    virtual void request(void);
    virtual void release(void);
}; // class NullMutex

#endif // !defined _INCLUDE_NULLMUTEX_H_

// end of NullMutex.h ...

