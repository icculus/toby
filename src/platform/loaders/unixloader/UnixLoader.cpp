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

#include "platform/loaders/unixloader/UnixLoader.h"

UnixLoader::UnixLoader(const char *binaryName)
    : TobyLoader(binaryName),
      dlhandle(NULL)
{
    char *unixName = new char[strlen(binaryName) + 7];
    if (unixName != NULL)
    {
        strcpy(unixName, "lib");
        strcat(unixName, binaryName);
        strcat(unixName, ".so");
        dlhandle = dlopen(unixName, RTLD_LAZY | RTLD_GLOBAL);
        delete[] unixName;
    } /* if */
} // Constructor


UnixLoader::~UnixLoader(void)
{
    if (dlhandle != NULL)
        dlclose(dlhandle);
} // Destructor


void *UnixLoader::getSymbol(const char *sym)
{
    return((dlhandle != NULL) ? dlsym(dlhandle, sym) : NULL);
} // UnixLoader::getSymbol


TobyLoader *TobyLoader::getInstance(const char *name)
{
    return(new UnixLoader(name));
} // TobyLoader::getInstance

// end of UnixLoader.cpp ...


