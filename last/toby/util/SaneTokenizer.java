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
 *    @author Ryan C. Gordon. (icculus@lokigames.com)
 */

package last.toby.util;

import java.io.StreamTokenizer;
import java.io.IOException;
import java.io.Reader;
import java.util.Vector;

import last.toby.util.TobyLanguage;
import last.toby.exceptions.ParseException;

public class SaneTokenizer extends StreamTokenizer
{
    protected boolean numsAreWords = false;
    protected boolean quotedAreWords = false;
    protected boolean charsAreWords = false;
    protected int qChar = (int) '"';
    protected Reader reader = null;

        // This is so garbage collection/processing is minimized...
    protected String[] oneStringArray = new String[1];

        // internal state copies to aid our hacks...
    protected boolean pushBackRequested = false;
    protected String svalOrig = null;
    protected double nvalOrig = 0.00;
    protected int ttypeOrig = 0;


    public SaneTokenizer(Reader reader)
    {
        super(reader);
        this.reader = reader;
        quoteChar(qChar);

        pushBackRequested = false;
        svalOrig = sval;
        nvalOrig = nval;
        ttypeOrig = ttype;
    } // Constructor

    public void close() throws IOException
    {
        reader.close();
        reader = null;
    } // close

    public void finalize() throws Throwable
    {
        close();
        super.finalize();
    } // finalize

    public void quoteChar(int ch)
    {
        super.quoteChar(ch);
        qChar = ch;
    } // quoteChar


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

        /**
         * Since we do so much messing with the superclass's state,
         *  we need to reimplement pushBack so that our hacks
         *  don't conflict. The sad thing is that this implementation
         *  is probably almost identical to the superclass, but with
         *  a different set of data...
         */
    public void pushBack()
    {
        pushBackRequested = true;
    } // pushBack (override from superclass.)


        /**
         * Same as super.lineno(), but if the last token read is
         *  the endline, consider the current line to be the one
         *  that is delimited by that endline. Normally, the
         *  endline increments the line count, but I want lineno()
         *  to signify the line number of the last read token.
         */
    public int lineno()
    {
        int retVal = super.lineno();
        if ((ttype == TT_EOL) || (ttype == TT_EOF))
            retVal--;
        return(retVal);
    } // lineno (override from superclass.)

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
         *      params : void.
         *     returns : see above.
         *      throws : IOException == see StreamTokenizer.nextToken() ...
         */
    public int nextToken() throws IOException
    {
        if (pushBackRequested == true)
        {
            ttype = ttypeOrig;
            nval = nvalOrig;
            sval = svalOrig;
            pushBackRequested = false;
        } // if
        else
        {
            super.nextToken();
            ttypeOrig = ttype;
            nvalOrig = nval;
            svalOrig = sval;
        } // else

        switch (ttype)
        {
            case TT_EOF:      // end of file, leave it.
            case TT_EOL:      // end of line, leave it.
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
    } // nextToken (override from superclass.)


        /**
         * Take the blank lines out of a Stream.
         *
         * The next call to toker.nextToken() will grab a non-EOL
         *  token, which may be an EOF, word, etc...
         */
    public void removeBlankLines() throws IOException
    {
        do
        {
            nextToken();
        } while (ttype == TT_EOL);

        pushBack();     // put last non-EOL token back in Stream.
    } // removeBlankLines


        /**
         * Take all words to the end of line (or end of file, if there is
         *  no end of line remaining), and place them into a new Vector of
         *  strings. If (numAreWords) is false, then numbers are placed
         *  into the Vector as java.lang.Double objects. Otherwise, they
         *  are converted to strings first. (quotedAreWords) and
         *  (charsAreWords) are also treated as such, with the single chars
         *  and quote characters being placed in a java.lang.Character.
         *
         *    @return Vector of tokens to end of line.
         *     @throw IOException on stream read failure.
         */
    public Vector vectorToEndOfLine() throws IOException
    {
        Vector retVal = new Vector();
        boolean getOut = false;

        do
        {
            nextToken();

            if (ttype == TT_WORD)
                retVal.addElement(sval);
            else if (ttype == TT_NUMBER)
                retVal.addElement(new Double(nval));
            else if ((ttype == TT_EOL) || (ttype == TT_EOF))
                getOut = true;
            else
                retVal.addElement(new Character((char) ttype));
        } while (getOut == false);

        pushBack();
        return(retVal);
    } // vectorToEndOfLine


        /**
         * Take all words and place them into a new Vector of
         *  strings, until we hit a token that is byte-equivalent to
         *  (delimit). Hitting an EOL or EOF is considered a ParseException.
         *  This check can be case sensitive or not, depending on the
         *  value of (caseSensitive).
         *
         * The delimiting token is pushed back, and will be the first token
         *  returned by the following nextToken() call.
         *
         * If (numAreWords) is false, then numbers are placed into
         *  the Vector as java.lang.Double objects. Otherwise, they are
         *  converted to strings first. (quotedAreWords) and (charsAreWords)
         *  are also treated as such, with the single chars and quote
         *  characters being placed in a java.lang.Character.
         *
         *    @return Vector of tokens.
         *     @throw ParseException on EOL and EOF.
         *     @throw IOException on stream read failure.
         */
    public Vector vectorToTokenDelimiter(String delimit, boolean caseSensitive)
                                         throws ParseException, IOException
    {
        this.oneStringArray[0] = delimit;
        return(vectorToTokenDelimiters(this.oneStringArray, caseSensitive));
    } // vectorToTokenDelimiter


        /**
         * Take all words and place them into a new Vector of
         *  strings, until we hit a token that is byte-equivalent to one of
         *  the elements in (delimit). Hitting an EOL or EOF is considered a
         *  ParseException. This check can be case sensitive or not, depending
         *  on the value of (caseSensitive).
         *
         * The delimiting token is pushed back, and will be the first token
         *  returned by the following nextToken() call.
         *
         * If (numAreWords) is false, then numbers are placed into
         *  the Vector as java.lang.Double objects. Otherwise, they are
         *  converted to strings first. (quotedAreWords) and (charsAreWords)
         *  are also treated as such, with the single chars and quote
         *  characters being placed in a java.lang.Character.
         *
         *    @return Vector of tokens.
         *     @throw ParseException on EOL and EOF.
         *     @throw IOException on stream read failure.
         */
    public Vector vectorToTokenDelimiters(String[] delimit,
                                          boolean caseSensitive)
                                         throws ParseException, IOException
    {
        Vector retVal = new Vector();
        boolean getOut = false;
        int max = delimit.length;
        int i;

        do
        {
            nextToken();

            if (ttype == TT_WORD)
            {
                for (i = 0; (i < max) && (getOut == false); i++)
                {
                    if (caseSensitive)
                        getOut = sval.equals(delimit[i]);
                    else
                        getOut = sval.equalsIgnoreCase(delimit[i]);
                } // for
    
                if (getOut == false)
                    retVal.addElement(sval);
            } // if

            else if (ttype == TT_NUMBER)
                retVal.addElement(new Double(nval));
            else if ((ttype == TT_EOL) || (ttype == TT_EOF))
                ParseException._throw(TobyLanguage.SYNTAX_ERROR);
            else
                retVal.addElement(new Character((char) ttype));
        } while (getOut == false);

        pushBack();
        return(retVal);
    } // vectorToTokenDelimiters


    public Vector vectorToDelimiterOrEnd(String delimit, boolean caseSensitive)
                                         throws ParseException, IOException
    {
// !!! This is almost identical to the above method.
        Vector retVal = new Vector();
        boolean getOut = false;

        do
        {
            nextToken();

            if (ttype == TT_WORD)
            {
                if (caseSensitive)
                    getOut = sval.equals(delimit);
                else
                    getOut = sval.equalsIgnoreCase(delimit);

                if (getOut == false)
                    retVal.addElement(sval);
            } // if

            else if (ttype == TT_NUMBER)
                retVal.addElement(new Double(nval));
            else if ((ttype == TT_EOL) || (ttype == TT_EOF))
                getOut = true;
            else
                retVal.addElement(new Character((char) ttype));
        } while (getOut == false);

        pushBack();
        return(retVal);
    } // vectorToDelimiterOrEnd


        /**
         * This just calls vectorToTokenDelimiter(), with case
         *  sensitivity always turned off.
         *
         *    @param delimit Token delimiter. See superclass.
         */
    public Vector vectorToTokenDelimiter(String delimit)
                                         throws ParseException, IOException
    {
        return(vectorToTokenDelimiter(delimit, false));
    } // vectorToTokenDelimiter


        /**
         * This just calls vectorToTokenDelimiters(), with case
         *  sensitivity always turned off.
         *
         *    @param delimit Token delimiter. See superclass.
         */
    public Vector vectorToTokenDelimiters(String[] delimits)
                                         throws ParseException, IOException
    {
        return(vectorToTokenDelimiters(delimits, false));
    } // vectorToTokenDelimiter


        /**
         * This just calls vectorToDelimiterOrEnd(), with case
         *  sensitivity always turned off.
         *
         *    @param delimit Token delimiter. See superclass.
         */
    public Vector vectorToDelimiterOrEnd(String delimit)
                                         throws ParseException, IOException
    {
        return(vectorToDelimiterOrEnd(delimit, false));
    } // vectorToDelimiterOrEnd


        /**
         * Get a word token. Anything else (EOF, EOL, number, etc.) will be
         *  considered a ParseException at this point. Note that methods such
         *  as parseCharsAsWords(), parseNumsAsWords(), etc. change the
         *  rules of what is considered a "word token".
         *
         *   @param String of the retrieved word token. It will also be
         *          stored to this.sval.
         *   @throw ParseException if a non-word token is retrieved.
         *   @throw IOException on stream read failure.
         */
    public String getWord() throws ParseException, IOException
    {
        int lineNum = lineno();
        String retVal = null;
        int rc = nextToken();

        if (rc == TT_WORD)
            retVal = sval;
        else
            ParseException._throw(TobyLanguage.EXPECTED_TOKEN, lineNum);

        return(retVal);
    } // getWord


        /**
         *  Make sure next token is the end of the line in the tokenizer, and
         *   throw if not. Returns without a throw, and the last-read-token
         *   being the EOL, if it is.  (EOF is considered an EOL, too.)
         *
         *    @throw TobyParseException If not end of line.
         *    @throw IOException If read error.
         */
    public void verifyEndLine() throws ParseException, IOException
    {
        nextToken();

        if ( (ttype != TT_EOL) && (ttype != TT_EOF) )
            ParseException._throw(TobyLanguage.EXPECTED_TOKEN, lineno());
    } // verifyEndLine

} // SaneTokenizer

// end of SaneTokenizer.java ...

