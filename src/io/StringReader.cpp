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
    if (str != NULL)
        delete[] str;
} // Destructor


bool StringReader::isEOF(void)
{
    return((nextChar != NULL) && (*nextChar != '\0'));
} // StringReader::isEOF


char StringReader::readChar(void) throw (IOException *)
{
    if (nextChar == NULL)
        throw(new IOException(TobyLanguage::FILE_NOT_OPEN));

    if (*nextChar == '\0')
        throw(new IOException(TobyLanguage::END_OF_FILE));

    char retval = *nextChar;
    nextChar++;
    return(retval);
} // StringReader::readChar


TobyString *StringReader::readLine(void) throw (IOException *)
{
    if (nextChar == NULL)
        throw(new IOException(TobyLanguage::FILE_NOT_OPEN));

    TobyString *retval = new TobyString();
    char ch = 0;

// !!! FIXME : This won't work with MacOS endlines!

    do
    {
        ch = *nextChar;
        nextChar++;
        if ((ch != '\r') && (ch != '\n') && (ch != '\0'))
            retval->append((char) ch);
    } while ((ch != '\0') && (ch != '\n'));

    return(retval);
} // StringReader::readLine

// end of StringReader.cpp ...


