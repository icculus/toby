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

#include "platform/clocks/sdlclock/SDLClock.h"

SDLClock *SDLClock::instance = NULL;

SDLClock::SDLClock(void)
{
    assert(instance == NULL);
    SDL_Init(0);
} // Constructor


SDLClock::~SDLClock(void)
{
    instance = NULL;
} // Destructor


TobyClock *SDLClock::getInstance(void)
{
    if (instance == NULL)
        instance = new SDLClock();

    return(instance);
} // SDLClock::getInstance


toby_uint32 SDLClock::getTicks(void)
{
    return((toby_uint32) SDL_GetTicks());
} // SDLClock::getTicks


TobyClock *__platformGetSingletonClock(void)
{
    return(SDLClock::getInstance());
} // __platformGetSingletonClock

// end of SDLClock.cpp ...


