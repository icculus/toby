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

#include "platform/threads/sdlthreads/SDLThread.h"


SDLThread::SDLThread(void)
{
} // Constructor


SDLThread::~SDLThread(void)
{
} // Destructor


void SDLThread::start(void *(*runfunc)(void *))
{
} // SDLThread::start


void SDLThread::waitForTermination(void)
{
} // SDLThread::waitForTermination


bool SDLThread::isTerminated(void)
{
    return(true);
} // SDLThread::isTerminated


TobyThread *__platformBuildThread(void)
{
    return(new SDLThread());
} // __platformBuildThread


void __platformThreadYieldCurrent(void)
{
    SDL_Delay(1);
} // __platformThreadYieldCurrent

// end of SDLThread.cpp ...

