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

#include "io/FileReader.h"

FileReader::FileReader(const char *filename)
    : fp(fopen(filename, "rb"))
{
} // Constructor


FileReader::~FileReader(void)
{
    if (fp != NULL)
        fclose(fp);
} // Destructor


bool FileReader::isEOF(void)
{
    bool retval = true;
    if (fp != NULL)
    {
        int ch = fgetc(fp);
        if (ch != EOF)
        {
            ungetc(ch, fp);
            retval = false;
        } // if
    } // if

    return(retval);
} // FileReader::isEOF


char FileReader::readChar(void) throw (IOException *)
{
    if (fp == NULL)
        throw(new IOException(TobyLanguage::FILE_NOT_OPEN));

    int ch = fgetc(fp);

    if (ch == EOF)
        throw(new IOException(TobyLanguage::END_OF_FILE));

    return((char) ch);
} // FileReader::readChar


TobyString *FileReader::readLine(void) throw (IOException *)
{
    if (fp == NULL)
        throw(new IOException(TobyLanguage::FILE_NOT_OPEN));

    TobyString *retval = new TobyString();
    int ch = 0;

// !!! FIXME : This won't work with MacOS endlines!

    do
    {
        ch = fgetc(fp);
        if ((ch != '\r') && (ch != '\n') && (ch != EOF))
            retval->append((char) ch);
    } while ((ch != EOF) && (ch != '\n'));

    return(retval);
} // TobyReader::readLine

// end of FileReader.cpp ...


