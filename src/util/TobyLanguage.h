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



   /***********************************************************************
    * Please see docs/subsystems/i18n.txt for information on this voodoo. *
    ***********************************************************************/



#ifndef _INCLUDE_TOBYLANGUAGE_H_
#define _INCLUDE_TOBYLANGUAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (defined CALLED_FROM_TOBYLANGUAGE)
#error CALLED_FROM_TOBYLANGUAGE should not be defined in your program.
#endif

#if (!defined TOBYI18N)  /* TobyLanguage.cpp defines this, too. */
#define TOBYI18N(sym, str) extern char *sym;
#endif

#define CALLED_FROM_TOBYLANGUAGE


/****************************
 * add your langfiles here. *
 ****************************/

#if #TOBYI18N_DEFAULT (english)
#include "i18n/english.tobylang"
#elif #TOBYI18N_DEFAULT (french)
#include "i18n/french.tobylang"
#else
#error Unknown default i18n langfile.
#endif

/*
 * This is always english by default, since it makes standard Toby programs
 *  incompatible if you change anything in this file.
 */
#include "i18n/tobykeywords.tobylang"


#undef CALLED_FROM_TOBYLANGUAGE
#undef TOBYI18N

#ifdef __cplusplus
}
#endif

#endif  /* !defined _INCLUDE_TOBYLANGUAGE_H_ */

/* end of TobyLanguage.h ... */

