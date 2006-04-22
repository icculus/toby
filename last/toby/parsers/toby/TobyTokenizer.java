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

package last.toby.parsers.toby;

import last.toby.util.*;
import java.io.IOException;
import java.io.Reader;

    /**
     *  Extends SaneTokenizer to handle unusual parsing circumstances.
     *
     *    @author Ryan C. Gordon. (icculus@icculus.org)
     */
public class TobyTokenizer extends SaneTokenizer
{
    public final static char TOBYCHAR_QUOTE   = '"';
    protected boolean replaceReferences = true;

    public TobyTokenizer(Reader strReader)
    {
        super(strReader);
        parseCharsAsWords(true);
        parseQuotedAsWords(true);
        parseNumsAsWords(true);
        eolIsSignificant(true);
        lowerCaseMode(true);
        quoteChar(TOBYCHAR_QUOTE);
        ordinaryChar('/');
        slashSlashComments(true);
        slashStarComments(false);
        wordChars('=', '=');
        wordChars('<', '<');
        wordChars('>', '>');
    } // TobyTokenizer


    public boolean getReferenceReplacePolicy()
    {
        return(replaceReferences);
    } // getReferenceReplacePolicy


    public void setReferenceReplacePolicy(boolean newPolicy)
    {
        replaceReferences = newPolicy;
    } // getReferenceReplacePolicy


    public int nextToken() throws IOException
    {
        int retVal = super.nextToken();

        if (retVal == TT_WORD)
        {
            if (sval.equals(TobyParser.OPER_ASSIGNMENT))
            {
                if ((nextToken() == TT_WORD) &&
                    (sval.equals(TobyParser.OPER_ASSIGNMENT)))
                {
                    sval = TobyParser.OPER_EQUALS;
                } // if
                else
                {
                    pushBack();
                    ttype = TT_WORD;
                    sval = TobyParser.OPER_ASSIGNMENT;
                } // else
            } // if
            else
            {
                if (replaceReferences == true)
                    sval = TobyParser.staticKeywordReference(sval);
            } // else
        } // if
        return(retVal);
    } // nextToken (override from superclass)

} // TobyTokenizer

// end of TobyTokenizer.java ...


