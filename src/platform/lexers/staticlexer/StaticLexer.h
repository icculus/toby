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

#ifndef _INCLUDE_STATICLEXER_H_
#define _INCLUDE_STATICLEXER_H_

#include "lexer/Lexer.h"
#include "lexer/LexerRules.h"

/*
 * Entry point for statically-compiled grammars to register themselves.
 *  These lexers (usually) consist of machine-generated C++.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class StaticLexer : public Lexer
{
public:
    Lexer *getInstance(void);
    virtual void loadLanguages(void);

private:
    void prepareLanguage(LanguageRules *lang);
}; // class Lexer

#endif // !defined _INCLUDE_STATICLEXER_H_

// end of Lexer.h ...

