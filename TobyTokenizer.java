/**
 *  Extends SaneTokenizer to handle unusual parsing circumstances.
 *
 *     Copyright (c) Lighting and Sound Technologies, 1997.
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


