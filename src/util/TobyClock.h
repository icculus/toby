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

#ifndef _INCLUDE_TOBYCLOCK_H_
#define _INCLUDE_TOBYCLOCK_H_

#include "util/TobyObject.h"

/*
 *  An abstraction over platform-specific timers.
 *
 *     Written by Ryan C. Gordon. (icculus@linuxgames.com)
 */
class TobyClock : public TobyObject
{
public:
    virtual ~TobyClock(void);

        // This returns the number of milliseconds since the instance was
        //  constructed, which may be somewhat arbitrary by itself, but is
        //  useful when compared against a later call to this method.
    virtual toby_uint32 getTicks(void) = 0;

        // This is how you get a TobyClock object; there is no constructor.
    static TobyClock *getSingleton(void);

protected:
        // This is protected, since the class is a singleton.
    TobyClock(void);

private:
    static TobyClock *instance;
        // This gets filled in along with the platform-specific subclass...
    static TobyClock *getInstance(void);
}; // class TobyClock

#endif // !defined _INCLUDE_TOBYCLOCK_H_

// end of TobyClock.h ...

