/**
 * The NumberIntrinsic represent numeric variables in TOBY. The numbers
 *  are stored as Java doubles, so it can hold real and whole numbers
 *  with a lot of precision.
 *
 *    Copyright (c) 1997-1999 Lighting and Sound Technologies.
 *     Written by Ryan C. Gordon.
 */

public class NumberIntrinsic extends Intrinsic
{
    public NumberIntrinsic(String ident, String initValue)
                               throws TobyParseException
    {
        super(ident, initValue);
    } // Constructor


    public NumberIntrinsic(String ident)
    {
        super(ident);
        value = "0.0";
    } // Constructor


    public String getType()
    {
        return(TobyInterpreter.INTRINSIC_NUMBER);
    } // getType


    public void setValue(String newValue) throws TobyParseException
    {
        try
        {
                // Wrapping it in a Double class checks for a valid number...
            value = new Double(newValue).toString();
        } // try
        catch (NumberFormatException nfe)
        {
            TobyParseException.throwException(TobyInterpreter.PROCERR_BAD_TYPE,
                                              null);
        } // catch
    } // setValue

} // NumberIntrinsic

// end of NumberIntrinsic.java ...


