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

#include "io/TobyReader.h"

TobyReader::TobyReader(void)
    : doingPushBack(false),
      lastChar(0)
{
} // Constructor


void TobyReader::pushBack(void)
{
    doingPushBack = true;
} // TobyReader::pushBack

bool TobyReader::isPushedBack(void)
{
    return(doingPushBack);
} // TobyReader::isPushedBack

TobyString *TobyReader::readLine(void) throw (IOException *)
{
    TobyString *retval = new TobyString();
    int ch = 0;

    while (true)
    {
        ch = readChar();
        if (ch == '\r')
        {
            ch = readChar();
            if ((ch != '\n') && (ch != EOF))  // it's a MacOS endline.
                pushBack();
            ch = '\n';
        } // if 

        if ((ch == '\n') || (ch == EOF))
            break;
        retval->append((char) ch);
    } // while

    _D(("TobyReader::readLine(): got [%s].\n", retval->c_str()));
    return(retval);
} // TobyReader::readLine


int TobyReader::readChar(void) throw (IOException *)
{
    if (doingPushBack)
    {
        doingPushBack = false;
        _D(("TobyReader::readChar(): got (pushback) '%c'.\n",(char)lastChar));
        return(lastChar);
    } // if

    lastChar = readCharImpl();
    _D(("TobyReader::readChar(): got '%c'.\n", (char) lastChar));
    return(lastChar);
} // TobyReader::readChar


// end of TobyReader.cpp ...

