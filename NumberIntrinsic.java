/** comment/copyright !!! */

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
            value = new Double(newValue).toString();  // !!!
        } // try
        catch (NumberFormatException nfe)
        {
            TobyParseException.throwException(TobyInterpreter.PROCERR_BAD_TYPE,
                                              null);
        } // catch
    } // setValue

} // NumberIntrinsic

// end of NumberIntrinsic.java ...


