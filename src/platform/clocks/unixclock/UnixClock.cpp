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

#include "platform/clocks/unixclock/UnixClock.h"

UnixClock::UnixClock(void)
{
    gettimeofday(&construction_time, NULL);
} // Constructor


UnixClock::~UnixClock(void)
{
} // Destructor


toby_uint32 UnixClock::getTicks(void)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    toby_uint32 then_ms = (((toby_uint32) construction_time.tv_sec) * 1000) +
                          (((toby_uint32) construction_time.tv_usec) / 1000);
    toby_uint32 now_ms  = (((toby_uint32) now.tv_sec) * 1000) +
                          (((toby_uint32) now.tv_usec) / 1000);
    return(now_ms - then_ms);
} // UnixClock::getTicks


TobyClock *TobyClock::getInstance(void)
{
    return(new UnixClock());
} // TobyClock::getInstance

// end of UnixClock.cpp ...


