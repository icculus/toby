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

#ifndef _INCLUDE_TOBYSTRING_H_
#define _INCLUDE_TOBYSTRING_H_

#include <stdio.h>
#include <string.h>
#include "util/TobyObject.h"

/*
 * This is a relatively minimal string class, for dealing with the
 *  ugliness of string memory management.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class TobyString : public TobyObject
{
public:
    TobyString(void);
    TobyString(const char *_str);
    TobyString(const TobyString *_str);

    virtual ~TobyString(void);

    const char *c_str(void) const;
    void append(float num);
    void append(int num);
    void append(char ch);
    void append(const char *str);

    bool replaceFormatToken(int n, const char *val);
    bool replaceFormatToken(int n, int val);

    void assignCStr(const char *str);

    int length(void);
    const char *substr(int start, int end);

private:
    char *str;
}; // class TobyString

#endif  // !defined _INCLUDE_TOBYSTRING_H_

// end of TobyString.h ...

