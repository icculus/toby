/*
 * TOBY -- A LOGO-like interpreted language, written in 100% pure Java.
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

/**
 *  Extends SaneTokenizer to handle unusual parsing circumstances.
 *
 *      Written by Ryan C. Gordon.
 */

import java.io.IOException;
import java.io.StringReader;

public class TobyTokenizer extends SaneTokenizer
{
    public TobyTokenizer(StringReader strReader)
    {
        super(strReader);
        parseCharsAsWords(true);
        parseQuotedAsWords(false);
        parseNumsAsWords(true);
        eolIsSignificant(true);
        lowerCaseMode(true);
        quoteChar(TobyInterpreter.TOBYCHAR_QUOTE);
        ordinaryChar('/');
        slashSlashComments(true);
        slashStarComments(false);
        wordChars('=', '=');
        wordChars('<', '<');
        wordChars('>', '>');
    } // TobyTokenizer


//    public int nextToken() throws IOException
    /**
     * Overrrides SaneTokenizer's nextToken() method, so that tokens
     *  like "==" don't get chunked into "=" and "=" ...
     *
     *     params : void.
     *    returns : see SaneTokenizer.nextToken(); ...
     */
/*    {
        int retVal = super.nextToken();

        if ((retVal == TT_WORD) &&                 // Handle '=' correctly...
            (sval.equals(TobyInterpreter.OPER_ASSIGNMENT)))
        {
            if (super.nextToken() != TT_WORD)
            {
                pushBack();
                ttype = TT_WORD;
                sval = TobyInterpreter.OPER_ASSIGNMENT;
            } // if
            else
            {
                if (sval.equals(TobyInterpreter.OPER_ASSIGNMENT))
                    sval = TobyInterpreter.OPER_EQUALS;
                else
                {
                    pushBack();
                    sval = TobyInterpreter.OPER_ASSIGNMENT;
                } // else
            } // else
        } // if

        return(retVal);
    } // nextToken
*/

} // TobyTokenizer

// end of TobyTokenizer.java ...


