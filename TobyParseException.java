/**
 *  A simple class filled with details of a parsing error.
 *
 *  Copyright (c) Lighting and Sound Technologies, 1997.
 *   Written by Ryan C. Gordon.
 */


public class TobyParseException extends Exception
{
        // Constants...
    public final static int LINENUM_UNDEFINED = -1;


        // Instance variables...
    public String procName = null;
    public int errLine = LINENUM_UNDEFINED;
    public boolean terminated = false;


        // Methods...

    public TobyParseException(String err)
    /**
     *  Contructor; Just here to pass a String to Exception.<init>...
     *
     *    params : err == String for Exception contructor...
     *   returns : Constructor; void.
     */
    {
        super(err);
    } // Constructor


    public static void throwException(String err, String pName, int lineNum)
                                         throws TobyParseException
    /**
     *  Call this for convenient TPE throwing...
     *
     *     params : err  == error message.
     *              proc == reference to procedure object that choked.
     *    returns : technically, void. Always throws an exception, though.
     *     throws : TobyParseException == hey, why else did you call this?
     */
    {
        TobyParseException tpe = new TobyParseException(err);

        tpe.errLine = lineNum;
        tpe.procName = pName;
        throw(tpe);
    } // throwException (overloaded; included line number.)


    public static void throwException(String err, String pName)
                                         throws TobyParseException
    /**
     *  Call this for convenient TPE throwing...
     *
     *     params : err  == error message.
     *              proc == reference to procedure object that choked.
     *    returns : technically, void. Always throws an exception, though.
     *     throws : TobyParseException == hey, why else did you call this?
     */
    {
        throwException(err, pName, LINENUM_UNDEFINED);
    } // throwException (overloaded; no line number.)

} // TobyParseException

// end of TobyParseException.java ...


