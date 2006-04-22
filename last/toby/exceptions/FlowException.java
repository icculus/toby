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

package last.toby.exceptions;

    /**
     * !!! comment me.
     *
     *   @author Ryan C. Gordon (icculus@icculus.org).
     */
public abstract class FlowException extends java.lang.Exception
{
        // Constants...
    public final static int LINENUM_UNDEFINED = -1;


        // Instance variables...
        // !!! This public stuff should have get and sets.
        // !!! These should be pooled...
    public String procName = null;
    public String msg = null;
    public int exceptionLine = LINENUM_UNDEFINED;

        // Methods...

    protected FlowException()    // no public construction.
    /**
     *  Contructor; Just here to pass a null to Exception.<init>...
     *
     *    params : err == String for Exception contructor...
     *   returns : Constructor; void.
     */
    {
        super((java.lang.String) null);
    } // Constructor


        /**
         * java.lang.Throwable (a superclass of this), will only let you set
         *  a message string at construction time; this will not do for us,
         *  as we recycle FlowExeceptions to reduce memory consumption.
         *  Therefore, we keep our own String that we can modify at runtime.
         *  We also override the getMessage() method, so that it returns our
         *  message, instead of the (null) that we gave Throwable at the time
         *  of construction.
         *
         *   @return message representing details of this FlowException.
         */
    public String getMessage()
    {
        return(this.msg);
    } // getMessage


        /**
         *  Call this for convenient exception throwing setup...
         *
         *   @param e FlowException object to prepare for throwing.
         *   @param err error message.
         *   @param pName Name of related procedure/function.
         *   @param lineNum line of interpreted code where exception occured.
         */
    protected static void initThrow(FlowException e, String err,
                                     String pName, int lineNum)
    {
        e.msg = err;
        e.exceptionLine = lineNum;
        e.procName = pName;
    } // initThrow (overloaded; the works.)


        /**
         *  Call this for convenient exception throwing setup...
         *
         *   @param e FlowException object to prepare for throwing.
         *   @param err error message.
         *   @param pName Name of related procedure/function.
         */
    protected static void initThrow(FlowException e, String err, String pName)
    {
        initThrow(e, err, pName, LINENUM_UNDEFINED);
    } // initThrow (overloaded; no line number.)


        /**
         *  Call this for convenient exception throwing setup...
         *
         *   @param e FlowException object to prepare for throwing.
         *   @param err error message.
         *   @param lineNum line of interpreted code where exception occured.
         */
    protected static void initThrow(FlowException e, String err, int lineNum)
    {
        initThrow(e, err, null, lineNum);
    } // initThrow (overloaded; just line number.)


        /**
         *  Call this for convenient exception throwing setup...
         *
         *   @param e FlowException object to prepare for throwing.
         *   @param err error message.
         */
    protected static void initThrow(FlowException e, String err)
    {
        initThrow(e, err, null, LINENUM_UNDEFINED);
    } // initThrow (overloaded; no line number or procedure.)

} // FlowException

// end of FlowException.java ...


