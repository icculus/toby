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

#include "lexer/Lexer.h"

Lexer::Lexer(void)
    : languages(new TobyCollection())
{
} // Constructor


Lexer::~Lexer(void)
{
    if (languages != NULL)
    {
        while (!languages->isEmpty())
            delete (LanguageRules *) languages->remove(0);
        delete languages;
    } // if
} // Destructor


void Lexer::registerLanguage(LanguageRules *lang)
{
    if (languages == NULL)
        return;

    languages->addElement(lang);
} // Lexer::registerLanguage


LanguageRules *Lexer::getLanguageRulesByName(const char *name)
{
    if (languages == NULL)
        return(NULL);

    int max = languages->getSize();
    for (int i = 0; i < max; i++)
    {
        LanguageRules *lang = (LanguageRules *) languages->elementAt(i);
        if (strcmp(lang->getName(), name) == 0)
            return(lang);
    } // for

    return(NULL);
} // Lexer::getLanguageRulesByName


LanguageRules *Lexer::getLanguageRulesByExtension(const char *ext)
{
    if (languages == NULL)
        return(NULL);

    int max = languages->getSize();
    for (int i = 0; i < max; i++)
    {
        LanguageRules *lang = (LanguageRules *) languages->elementAt(i);
        if (strcmp(lang->getFileExtension(), ext) == 0)
            return(lang);
    } // for

    return(NULL);
} // Lexer::getLanguageRulesByExtension


// end of Lexer.cpp ...

