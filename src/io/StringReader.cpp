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

#include "io/StringReader.h"

StringReader::StringReader(const char *_str)
    : str(new char[strlen(_str) + 1]),
      nextChar(str)
{
    assert(str != NULL);
    strcpy(str, _str);
} // Constructor


StringReader::~StringReader(void)
{
    delete[] str;
} // Destructor


bool StringReader::isOpened(void)
{
    return(nextChar != NULL);
} // FileReader::isOpened


bool StringReader::isEOF(void)
{
    return((nextChar != NULL) && (*nextChar != '\0'));
} // StringReader::isEOF


int StringReader::readCharImpl(void) throw (IOException *)
{
    if (nextChar == NULL)
        throw(new IOException(TOBYI18N_FILE_NOT_OPEN));

    if (*nextChar == '\0')
        return(TOBYEOF);

    char retval = *nextChar;
    nextChar++;

    return(retval);
} // StringReader::readCharImpl

// end of StringReader.cpp ...


