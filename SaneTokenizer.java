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
 *  This class extends StreamTokenizer, with a few small improvements.
 *
 *    Written by Ryan C. Gordon.
 */

import java.io.StreamTokenizer;
import java.io.IOException;
import java.io.Reader;

public class SaneTokenizer extends StreamTokenizer
{
   protected boolean numsAreWords = false;
   protected boolean quotedAreWords = false;
   protected boolean charsAreWords = false;
   protected int qChar = (int) '"';

    public void quoteChar(int ch)
    {
        super.quoteChar(ch);
        qChar = ch;
    } // quoteChar


    public SaneTokenizer(Reader reader)
    {
        super(reader);
        quoteChar(qChar);
    } // Constructor


    public void parseNumsAsWords(boolean newValue)
    {
       numsAreWords = newValue;
    } // parseNumsAsWords


    public void parseQuotedAsWords(boolean newValue)
    {
        quotedAreWords = newValue;
    } // parseQuotedAsWords


    public void parseCharsAsWords(boolean newValue)
    {
        charsAreWords = newValue;
    } // parseCharsAsWords


    public int nextToken() throws IOException
    /**
     * If the user has called parseCharsAsWords(true), and
     *  StreamTokenizer.nextToken() sets the ttype field to anything
     *  other than the quote char, TT_WORD, TT_NUMBER, TT_EOL, or
     *  TT_EOF, then this.ttype is converted to a String, placed in
     *  this.sval, and this.ttype is set to TT_WORD. This functionality
     *  is turned off again by calling parseCharsAsWords(false), and is
     *  off by default.
     *
     * If the user has called parseNumsAsWords(true), then TT_NUMBER
     *  values are converted to Strings, placed in this.sval, and
     *  this.ttype is set to TT_WORD. This functionality is turned off
     *  again by calling parseNumsAsWords(false), and is off by default.
     *
     * If the user has called parseQuotedAsWords(true), and
     *  StreamTokenizer.nextToken sets the ttype value to the quote char,
     *  then this.ttype is set to TT_WORD. this.sval remains unchanged.
     *  This functionality is turned off again by calling
     *  parseQuotedAsWords(false), and is off by default.
     *
     * Other values are unchanged.
     *
     *   overrides : StreamTokenizer.nextToken();
     *      params : void.
     *     returns : see above.
     *      throws : IOException == see StreamTokenizer.nextToken() ...
     */
    {
        super.nextToken();

        switch (ttype)
        {
            case TT_EOF:      // end of file, and end of line: leave 'em.
            case TT_EOL:
                break;

            case TT_WORD:    // Full words, or quoted text: it's cool.
                break;

            case TT_NUMBER:  // Numeric data.
                if (numsAreWords)
                {
                    sval = Double.toString(nval);
                    ttype = TT_WORD;
                } // if
                break;

            default:         // single char token...
                if (ttype == qChar)
                {
                    if (quotedAreWords)
                        ttype = TT_WORD;
                    sval = qChar + sval + qChar;
                } // if

                else if (charsAreWords)
                {
                    sval = new Character((char) ttype).toString();
                    ttype = TT_WORD;
                } // else if
                break;
        } // switch

        return(ttype);
    } // nextToken (overrides StreamTokenizer.nextToken()...)

} // SaneTokenizer

// end of SaneTokenizer.java ...

