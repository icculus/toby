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

#ifndef _INCLUDE_TOBYOBJECT_H_
#define _INCLUDE_TOBYOBJECT_H_

#include <stdio.h>   // ...gotta have my printf.  :)
#include <stdlib.h>  // make sure NULL is always defined.
#include <assert.h>  // make sure assert() is always defined.

// !!! This needs to change.
typedef signed long    toby_sint32;
typedef unsigned long  toby_uint32;
typedef signed short   toby_sint16;
typedef unsigned short toby_uint16;
typedef signed char    toby_sint8;
typedef unsigned char  toby_uint8;


#if (defined DEBUG)
#define _D(x) printf x;
#else
#define _D(x)
#endif


/*
 * This is a common base class for all other Toby class.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class TobyObject
{

    // !!! assert(0) in the copy constructor.

}; // class TobyObject

#endif  // !defined _INCLUDE_TOBYOBJECT_H_

// end of TobyObject.h ...

