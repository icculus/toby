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

#include "platform/threads/nullthreads/NullThread.h"


NullThread::NullThread(void *(*runfunc)(void *), void *args)
    : TobyThread(runfunc, args)
{
    _D(("NullThread::NullThread() called!\n"));
} // Constructor


NullThread::~NullThread(void)
{
    _D(("NullThread::~NullThread() called!\n"));
} // Destructor


void NullThread::waitForTermination(void)
{
    _D(("NullThread::waitForTermination() called!\n"));
} // NullThread::waitForTermination


bool NullThread::isTerminated(void)
{
    _D(("NullThread::isTerminated() called!\n"));
    return(true);
} // NullThread::isTerminated


TobyThread *__platformBuildThread(void *(*runfunc)(void *), void *args)
{
    return(new NullThread(runfunc, args));
} // __platformBuildThread


void __platformThreadYieldCurrent(void)
{
    _D(("Null version of __platformThreadYieldCurrent() called!\n"));
} // __platformThreadYieldCurrent

// end of NullThread.cpp ...

