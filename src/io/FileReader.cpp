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


bool FileReader::isOpened(void)
{
    return(fp != NULL);
} // FileReader::isOpened


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


int FileReader::readCharImpl(void) throw (IOException *)
{
    if (fp == NULL)
        throw(new IOException(TOBYI18N_FILE_NOT_OPEN));

    int ch = fgetc(fp);
    if (ch == EOF)
    {
        if (ferror(fp))
            throw(new IOException(strerror(errno)));
        else
            ch = TOBYEOF;
    } // if

    return(ch);
} // FileReader::readChar

// end of FileReader.cpp ...


