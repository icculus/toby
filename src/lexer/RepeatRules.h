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

#ifndef _INCLUDE_REPEATRULES_H_
#define _INCLUDE_REPEATRULES_H_

#include "lexer/LexerRules.h"
#include "exceptions/IOException.h"
#include "io/Tokenizer.h"

/*
 *  Repeat lexer metarule: a logical grouping of subrules that keeps
 *   evaluating until one of those subrules fails.
 *
 *     Written by Ryan C. Gordon. (icculus@icculus.org)
 */
class RepeatRules : public LexerRules
{
public:
        // The (kids) array is assumed to be built specifically for
        //  a given instance (and either by computer-generated code or
        //  by the XML parser), so we don't make a copy, and just keep the
        //  original reference. This reference will be deleted at destruction,
        //  so don't delete it yourself!
    RepeatRules(int _minval, int _maxval, size_t numKids, LexerRules **kids);

    virtual ~RepeatRules(void);
    virtual bool testRules(Tokenizer *toker) throw (IOException *);
    virtual bool resolve(LexerRules *langRules);

protected:
    int minRepeats;
    int maxRepeats;
    size_t numChildren;
    LexerRules **children;
}; // class RepeatRules

#endif // !defined _INCLUDE_REPEATRULES_H_

// end of RepeatRules.h ...

