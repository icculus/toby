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

#include "util/TobyString.h"

TobyString::TobyString(void) : str(NULL)
{
    assignCStr("");
} // Constructor

TobyString::TobyString(const char *_str) : str(NULL)
{
    assignCStr("");
} // Constructor

TobyString::TobyString(const TobyString *_str) : str(NULL)
{
    assignCStr(_str->c_str());
} // Constructor

TobyString::~TobyString(void)
{
    if (str != NULL)
        free(str);
} // Destructor

const char *TobyString::c_str(void) const
{
    return(str);   // READ. ONLY.
} // TobyString::c_str

#if 0
// !!!
char *TobyString::operator char *(void)
{
    return((const char *) str);
} // TobyString::operator char *
#endif

void TobyString::assignCStr(const char *_str)
{
    if (_str == NULL)
        _str = "";

    str = (char *) realloc(str, strlen(_str) + 1);
    strcpy(str, _str);
} // assignCStr


bool TobyString::replaceFormatToken(int n, const char *val)
{
    assert(val != NULL);
    assert((n >= 0) && (n <= 9));

    char token[3] = {'%', '0' + n, '\0'};
    char *ptr;

    ptr = strstr(str, token);
    if (ptr == NULL)
        return(false);

    char *buf = new char[strlen(str) + strlen(val) + 1];
    *ptr = '\0';  // chop string at "%n".
    strcpy(buf, str);
    strcat(buf, val);
    strcat(buf, ptr + 2);
    assignCStr(buf);
    delete[] buf;

    return(true);
} // TobyString::replaceFormatToken


bool TobyString::replaceFormatToken(int n, int val)
{
    char buf[128];
    snprintf(buf, sizeof (buf), "%d", val);
    return(replaceFormatToken(n, buf));
} // TobyString::replaceFormatToken

// end of TobyString.cpp ...

