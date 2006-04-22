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

#ifndef _INCLUDE_FILEREADER_H_
#define _INCLUDE_FILEREADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "io/TobyReader.h"
#include "util/TobyLanguage.h"

/*
 *  FileReaders wrap ANSI-C file i/o in a TobyReader interface.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class FileReader : public TobyReader
{
public:
    FileReader(const char *filename);
    virtual ~FileReader(void);

    virtual bool isEOF(void);
    virtual bool isOpened(void);
    virtual int readCharImpl(void) throw (IOException *);

private:
    FILE *fp;
}; // class FileReader

#endif  // !defined _INCLUDE_FILEREADER_H_

// end of FileReader.h ...


