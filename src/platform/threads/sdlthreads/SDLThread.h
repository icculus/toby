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

#ifndef _INCLUDE_SDLTHREAD_H_
#define _INCLUDE_SDLTHREAD_H_

#include "SDL.h"
#include "SDL_thread.h"
#include "util/TobyThread.h"

/*
 * A SDL-based implementation of TobyThreads. This covers most Unixes,
 *  and other POSIX-compliant systems.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class SDLThread : public TobyThread
{
public:
    SDLThread(void *(*runfunc)(void *), void *args);
    virtual ~SDLThread(void);
    virtual void waitForTermination(void);
    virtual bool isTerminated(void);

private:
    SDL_Thread *realThread;
    bool terminated;
}; // class SDLThread

#endif // !defined _INCLUDE_SDLTHREAD_H_

// end of SDLThread.h ...

