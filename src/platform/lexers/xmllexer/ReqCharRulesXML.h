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

#ifndef _INCLUDE_REQCHARRULESXML_H_
#define _INCLUDE_REQCHARRULESXML_H_

#include "util/TobyObject.h"
#include "platform/lexers/xmllexer/XMLLexer.h"
#include "lexer/ReqCharRules.h"
#include "xml/XMLNode.h"
#include "util/TobyLoader.h"

/*
 * A subclass of ReqCharRules that constructs itself based on an XML node,
 *  and can output a string containing the C++ code needed to construct
 *  its parent.
 *
 *     Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */
class ReqCharRulesXML : public ReqCharRules
{
public:
    virtual ~ReqCharRulesXML(void);
    virtual const char *outputConstructor(void);
    static LexerRules *buildRules(XMLNode *node);

private:
        // Use buildRules(), which calls this.
    ReqCharRulesXML(ReqCharRules::chartype _ctype, char _specific);
    ReqCharRules::chartype ctype;
}; // class ReqCharRulesXML

#endif // !defined _INCLUDE_REQCHARRULESXML_H_

// end of ReqCharRulesXML.h ...

