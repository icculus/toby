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

#ifndef _INCLUDE_STRINGREADER_H_
#define _INCLUDE_STRINGREADER_H_

#include "io/TobyReader.h"
#include "util/TobyLanguage.h"

/*
 *  StringReaders read from a block of null-terminated characters in
 *   memory as if it were a file.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class StringReader : public TobyReader
{
public:
    StringReader(const char *_str);
    virtual ~StringReader(void);

    virtual bool isEOF(void);
    virtual bool isOpened(void);
    virtual int readCharImpl(void) throw (IOException *);

private:
    char *str;
    char *nextChar;
}; // class StringReader

#endif  // !defined _INCLUDE_STRINGREADER_H_

// end of StringReader.h ...


