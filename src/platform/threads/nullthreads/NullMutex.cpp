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

#include "platform/threads/nullthreads/NullMutex.h"


NullMutex::NullMutex(void)
{
    _D(("NullMutex::NullMutex() called!\n"));
} // Constructor


NullMutex::~NullMutex(void)
{
    _D(("NullMutex::~NullMutex() called!\n"));
} // Destructor


void NullMutex::request(void)
{
    _D(("NullMutex::release() called!\n"));
} // NullMutex::request


void NullMutex::release(void)
{
    _D(("NullMutex::release() called!\n"));
} // NullMutex::request



// eine kleine platform glue...

TobyMutex *TobyMutex::getInstance(void)
{
    return(new NullMutex());
} // TobyMutex::getInstance

// end of NullMutex.cpp ...

