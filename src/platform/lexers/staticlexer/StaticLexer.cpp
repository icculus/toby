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

#include "platform/lexers/staticlexer/StaticLexer.h"

Lexer *Lexer::getInstance(void)
{
    return(new StaticLexer());
} // StaticLexer::getInstance


void StaticLexer::prepareLanguage(LanguageRules *lang)
{

    if (lang == NULL)
    {
        _D(("StaticLexer: Failed to built %s lexer!\n", lang->getName()));
        return;
    } // if

    if (lang->resolve(lang) == false)
    {
        _D(("StaticLexer: Failed to resolve %s lexer!\n", lang->getName()));
        delete lang;
        return;
    } // if

    _D(("StaticLexer: Successfully built %s lexer!\n", lang->getName()));
    registerLanguage(lang);
} // prepareLanguage


void StaticLexer::loadLanguages(void)
{
#if (defined TOBYPARSER_TOBY)
    _D(("StaticLexer: About to build lexer for Toby language...\n"));
    LexerRules *staticlang_toby(void);
    prepareLanguage((LanguageRules *) staticlang_toby());
#endif
} // StaticLexer::loadLanguages

// end of StaticLexer.cpp ...


