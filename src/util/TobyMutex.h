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

#ifndef _INCLUDE_TOBYMUTEX_H_
#define _INCLUDE_TOBYMUTEX_H_

#include "util/TobyObject.h"

/*
 *  An abstraction over platform-specific threading mutexes.
 *
 *     Written by Ryan C. Gordon. (icculus@linuxgames.com)
 */
class TobyMutex : public TobyObject
{
public:
    TobyMutex(void) {}
    virtual ~TobyMutex(void) {}
    virtual void request(void) = 0;
    virtual void release(void) = 0;
}; // class TobyMutex


// This gets filled in along with the platform-specific subclass...
TobyMutex *__platformBuildMutex(void);

#endif // !defined _INCLUDE_TOBYMUTEX_H_

// end of TobyMutex.h ...

