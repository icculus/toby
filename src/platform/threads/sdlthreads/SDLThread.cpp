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

typedef struct __S_THREADENTRYARGS
{
    void *args;
    void *(*runfunc)(void *args);
    bool *terminatedFlag;
} ThreadEntryArgs;


static int threadEntry(void *_args)
{
    ThreadEntryArgs *args = (ThreadEntryArgs *) _args;
    void *retval = args->runfunc(args->args);
    *(args->terminatedFlag) = true;
    delete args;
    return((int) retval);
} // threadEntry


SDLThread::SDLThread(void *(*runfunc)(void *), void *args)
    : TobyThread(runfunc, args),
      terminated(false)
{
    ThreadEntryArgs *_args = new ThreadEntryArgs;
    _args->args = args;
    _args->runfunc = runfunc;
    _args->terminatedFlag = &terminated;

    realThread = SDL_CreateThread(threadEntry, _args);
    assert(realThread != NULL);
} // Constructor


SDLThread::~SDLThread(void)
{
    SDL_KillThread(realThread);
    int rc = 0;

    if (SDL_ThreadID() == SDL_GetThreadID(realThread))
        SDL_KillThread(realThread);  // woo boy.
    else
    {
        if (terminated)
            SDL_WaitThread(realThread, &rc);
        else
            SDL_KillThread(realThread);
    } // else
} // Destructor


void SDLThread::waitForTermination(void)
{
    int rc = 0;
    SDL_WaitThread(realThread, &rc);
} // SDLThread::waitForTermination


bool SDLThread::isTerminated(void)
{
    return(terminated);
} // SDLThread::isTerminated


TobyThread *TobyThread::getInstance(void *(*runfunc)(void *), void *args)
{
    return(new SDLThread(runfunc, args));
} // TobyThread::getInstance


void TobyThread::yieldCurrent(void)
{
    SDL_Delay(1);
} // TobyThread::yieldCurrent

// end of SDLThread.cpp ...

