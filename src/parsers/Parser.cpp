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

#include "parsers/Parser.h"

Parser::Parser(void)
    : totalParseTime(0xdeadbeef),
      totalLinkTime(0xdeadbeef)
{
} // Constructor


Parser::~Parser(void)
{
} // Destructor


    // !!! Change this bubble sort with something faster?
TobyCollection *Parser::alphabetizeArrayElements(TobyCollection *array)
{
    int i;
    int max;
    bool sorted;

    if (array == NULL)      // safety check.
        return(NULL);

    max = array->getSize() - 1;

    if (max <= 0)
        return(array);

    do
    {
        sorted = true;
        for (i = 0; i < max; i++)
        {
            char *str1 = (char *) array->elementAt(i);
            char *str2 = (char *) array->elementAt(i + 1);
            if (strcmp(str1, str2) > 0)
            {
                sorted = false;
                array->swapElements(i, i + 1);
            } // if
        } // for
    } while (sorted == false);

    return(array);
} // Parser::alphabetizeArrayElements


// !!! this is a slow algorithm.
int Parser::searchAlphabeticArray(TobyCollection *table, const char *forThis)
{
    if (forThis != NULL)   // bullet proofing.
    {
        int max = table->getSize();
        for (int i = 0; i < max; i++)
        {
            int rc = strcmp(forThis, (char *) (table->elementAt(i)));
            if (rc == 0)
                return(i);
            else if (rc < 0)  // alphabetically past search string?
                return(-1);
        } // for
    } // if

    return(-1);   // not found. Bail.
} // Parser::searchAlphabeticArray


int Parser::searchArray(TobyCollection *table, const char *forThis)
{
    if (forThis != NULL)
    {
        int max = table->getSize();
        for (int i = 0; i < max; i++)
        {
            if (strcmp((char *) table->elementAt(i), forThis) == 0)
                return(i);
        } // for
    } // if

    return(-1);
} // Parser::searchArray


int Parser::searchArrayForReference(TobyCollection *table, const char *str)
{
    int max = table->getSize();

    for (int i = 0; i < max; i++)
    {
        if (table->elementAt(i) == (void *) str)
            return(i);
    } // for

    return(-1);
} // Parser::searchArrayForReference


GlobalLogicContext *Parser::parseCode(TobyReader *code, TobyObject *screen)
                                                throw (ParseException *)
{
    _D(("Parser::parseCode(): Starting parsing phase...\n"));
    TobyClock *clock = TobyClock::getSingleton();
    toby_uint32 parseStartTime = clock->getTicks();
    GlobalLogicContext *retVal = parseCodeImpl(code);
    totalParseTime = clock->getTicks() - parseStartTime;

    if (retVal != NULL)
    {
        // !!! retVal->setScreen(screen);

        _D(("Parser::parseCode(): Starting linking phase...\n"));
        toby_uint32 linkStartTime = clock->getTicks();
        // !!! retVal->link(retVal);
        totalLinkTime = clock->getTicks() - linkStartTime;
    } // if

    _D(("Parser::parseCode(): Total parse time: (%lu) ms.\n", totalParseTime));
    _D(("Parser::parseCode(): Total link  time: (%lu) ms.\n", totalLinkTime));

    return(retVal);
} // Parser::parseCode


toby_uint32 Parser::getTotalParseTime(void)
{
    return(totalParseTime);
} // Parser::getTotalParseTime


toby_uint32 Parser::getTotalLinkTime(void)
{
    return(totalLinkTime);
} // Parser::getTotalLinkTime


// end of Parser.cpp ...

