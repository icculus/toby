/** comment/copyright !!! */

public class BooleanIntrinsic extends Intrinsic
{
    public BooleanIntrinsic(String ident, String initValue)
                               throws TobyParseException
    {
        super(ident, initValue);
    } // Constructor


    public BooleanIntrinsic(String ident)
    {
        super(ident);
        value = TobyInterpreter.KEYWORD_FALSE;
    } // Constructor


    public String getType()
    {
        return(TobyInterpreter.INTRINSIC_BOOLEAN);
    } // getType


    public void setValue(String newValue) throws TobyParseException
    {
        if ((newValue.equals(TobyInterpreter.KEYWORD_TRUE)) ||
            (newValue.equals(TobyInterpreter.KEYWORD_FALSE)))
        {
            value = newValue;
        } // if
        else
        {
            TobyParseException.throwException(TobyInterpreter.PROCERR_BAD_TYPE,
                                              null);
        } // else
    } // setValue

} // BooleanIntrinsic

// end of BooleanIntrinsic.java ...


