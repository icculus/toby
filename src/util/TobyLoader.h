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

#ifndef _INCLUDE_TOBYLOADER_H_
#define _INCLUDE_TOBYLOADER_H_

#include "util/TobyObject.h"

/*
 *  An abstraction over platform-specific dynamic loading.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class TobyLoader : public TobyObject
{
public:

        // Load a dynamic binary. (binaryName) is the basename of the
        //  binary to load ("mybinary" will become "libmybinary.so" on
        //  Linux, and "mybinary.dll" on win32). (binaryname) can be
        //  NULL, which means use the application's binary. Use this
        //  instead of the new operator, since we need to generate a
        //  platform-dependent object.
    static TobyLoader *getInstance(const char *name);

    virtual ~TobyLoader(void) {}

        // return a pointer to the symbol named (sym). NULL if not found.
    virtual void *getSymbol(const char *sym) = 0;

protected:
    TobyLoader(const char *binaryName) {}
}; // class TobyLoader

#endif // !defined _INCLUDE_TOBYLOADER_H_

// end of TobyLoader.h ...

