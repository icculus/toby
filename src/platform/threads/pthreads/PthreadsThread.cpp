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

#include "platform/threads/pthreads/PthreadsThread.h"

typedef struct __S_THREADENTRYARGS
{
    void *args;
    void *(*runfunc)(void *args);
    bool *terminatedFlag;
} ThreadEntryArgs;


static void *threadEntry(void *_args)
{
    ThreadEntryArgs *args = (ThreadEntryArgs *) _args;
    void *retval = args->runfunc(args->args);
    *(args->terminatedFlag) = true;
    delete args;
    return(retval);
} // threadEntry


PthreadsThread::PthreadsThread(void *(*runfunc)(void *), void *args)
    : TobyThread(runfunc, args),
      terminated(false)
{
    ThreadEntryArgs *_args = new ThreadEntryArgs;
    _args->args = args;
    _args->runfunc = runfunc;
    _args->terminatedFlag = &terminated;

    if (pthread_create(&realThread, NULL, threadEntry, _args) != 0)
    {
        assert(0);
    } // if
} // Constructor


PthreadsThread::~PthreadsThread(void)
{
    void *rc = NULL;

    if (pthread_equal(pthread_self(), realThread))
        pthread_exit(&rc);

    else
    {
        if (terminated)
            pthread_join(realThread, &rc);
        else
        {
            pthread_detach(realThread);
            pthread_kill(realThread, SIGKILL);
        } // else
    } // else
} // Destructor


void PthreadsThread::waitForTermination(void)
{
    void *rc = NULL;
    pthread_join(realThread, &rc);
} // PthreadsThread::waitForTermination


bool PthreadsThread::isTerminated(void)
{
    return(terminated);
} // PthreadsThread::isTerminated


TobyThread *__platformBuildThread(void *(*runfunc)(void *), void *args)
{
    return(new PthreadsThread(runfunc, args));
} // __platformBuildThread


void __platformThreadYieldCurrent(void)
{

    usleep(1000);

// doesn't do anything?
#if 0
#if (defined _POSIX_PRIORITY_SCHEDULING)
    sched_yield();
#else
#warning There is no sched_yield() on this platform...
#endif
#endif
} // __platformThreadYieldCurrent

// end of PthreadsThread.cpp ...

