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

package last.toby.parsers;
import last.toby.util.TobyLanguage;
import last.toby.util.BugSquasher;
import last.toby.interpreter.GlobalLogicContext;
import last.toby.exceptions.ParseException;
import java.io.Reader;

    /**
     *  This gives us a common interface to controlling various parsers.
     *
     *   @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public abstract class Parser
{
    protected long totalParseTime = -1;
    protected long totalLinkTime = -1;

        /**
         * If debugging is enabled, then println debug info to stdout.
         *
         *   @param funcName name of method where debugging info lies.
         *   @param msg Message to print.
         */
    protected void _D(String funcName, String msg)
    {
        BugSquasher.println(funcName, msg);
    } // _D


        /**
         * Works like a C++ assertion.
         *
         *   assert(2 > 4) will throw a ParseException, since it's false.
         *   assert(2 < 4) will not.
         *
         *   @param expression true or false expression.
         *   @throw ParseException if (expression) is true.
         */
    public static void assert(boolean expression) throws ParseException
    {
        BugSquasher.assert(expression);
    } // assert

        /**
         * A simple bubble sort to alphabetize an array of strings.
         *
         *   @param array Array to sort.
         *  @return same reference to (array) that was passed as a
         *          param, sorted.
         */
    public static String[] alphabetizeArrayElements(String[] array)
    {
        int i;
        int j;
        int max;
        String tmp;
        boolean sorted;

        if (array == null)      // safety check.
            return(null);

        max = array.length - 1;

        do
        {
            sorted = true;
            for (i = 0; i < max; i++)
            {
                if (array[i].compareTo(array[i + 1]) > 0)
                {
                    sorted = false;
                    tmp = array[i];
                    array[i] = array[i + 1];
                    array[i + 1] = tmp;
                } // if
            } // for
        } while (sorted == false);

        return(array);
    } // alphabetizeArrayElements


        /**
         * Find out if (forThis) is in (table). The strings contained in table
         *  must be indexed in alphabetic order. This makes the searches faster.
         *
         *     params : see above.
         *    returns : index in table, if there. (-1) if (forThis) isn't there.
         */
    public static int searchAlphabeticArray(String[] table, String forThis)
    {
        int i;
        int rc;
        int max = table.length;

        if (forThis != null)   // bullet proofing.
        {
            for (i = 0; i < max; i++)
            {
                rc = forThis.compareTo(table[i]);
                if (rc == 0)
                    return(i);
                else if (rc < 0)  // alphabetically past search string?
                    return(-1);
            } // for
        } // if

        return(-1);   // not found. Bail.
    } // searchAlphabeticArray


        /**
         * Find out if (forThis) is in (table).
         *
         *     params : see above.
         *    returns : index in table, if there. (-1) if (forThis) isn't there.
         */
    public static int searchArray(String[] table, String forThis)
    {
        int i;
        int max = table.length;

        if (forThis != null)
        {
            for (i = 0; i < max; i++)
            {
                if (table[i] != null)
                {
                    if (table[i].compareTo(forThis) == 0)
                        return(i);
                } // if
            } // for
        } // if
        return(-1);
    } // searchArray


        /**
         * Find out if (forThis) is in (table). (forThis) must not be just
         *  an identical string, but an identical reference.
         *
         *     params : see above.
         *    returns : index in table, if there. (-1) if (forThis) isn't there.
         */
    public static int searchArrayForReference(String[] table, String forThis)
    {
        int i;

        for (i = 0; i < table.length; i++)
        {
            if (table[i] == forThis)
                return(i);
        } // for

        return(-1);
    } // searchArrayForReference




        /**
         * Call this to parse a collection of code into a LogicContext tree.
         *  (code) may be an entire program or just a single procedure or
         *  whatever else.
         *
         *   @param code The code to parse. This is usually a StringReader,
         *               but may come directly from disk or network.
         *   @param screen An object representing the screen. This is
         *                 language/platform, etc dependent, but in the
         *                 usual/intended case, it's an implementor of
         *                 last.toby.turtlespace.TurtleSpace.
         *  @return A new ModuleLogicContext containing procedures, global
         *          vars, and other attributes resulting from the parsing of
         *          (code).
         *   @throw ParseException on syntax errors, etc...
         */
    public final GlobalLogicContext parseCode(Reader code, Object screen)
                                                throws ParseException
    {
//        _D("parseCode", "Starting parsing phase...");
        long parseStartTime = System.currentTimeMillis();
        GlobalLogicContext retVal = parseCodeImpl(code);
        this.totalParseTime = System.currentTimeMillis() - parseStartTime;

        if (retVal != null)
        {
            retVal.setScreen(screen);

//            _D("parseCode", "Starting linking phase...");
            long linkStartTime = System.currentTimeMillis();
            retVal.link(retVal);
            this.totalLinkTime = System.currentTimeMillis() - linkStartTime;
        } // if

//        _D("parseCode", "Total parse time: (" + this.totalParseTime + ") ms.");
//        _D("parseCode", "Total link  time: (" + this.totalLinkTime  + ") ms.");

        return(retVal);
    } // parseCode


        /**
         * Get the total time that was spent during the last parsing,
         *  in milliseconds.
         *
         *   @return total parse time, in milliseconds.
         */
    public final long getTotalParseTime()
    {
        return(this.totalParseTime);
    } // getTotalParseTime


        /**
         * Get the total time that was spent during the last linking,
         *  in milliseconds.
         *
         *   @return total link time, in milliseconds.
         */
    public final long getTotalLinkTime()
    {
        return(this.totalLinkTime);
    } // getTotalLinkTime

        /**
         * This is the entry point for a given parser to do its work.
         *
         *    @param The entire module's source code as one big string.
         *            Newlines included.
         *   @return a GlobalLogicContext containing the parsed code's
         *           logic structure.
         */
    public abstract GlobalLogicContext parseCodeImpl(Reader code)
                                                     throws ParseException;
} // Parser

// end of Parser.java ...



