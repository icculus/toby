/** !!! comment/copyright */

public class NothingIntrinsic extends Intrinsic
{
    public static NothingIntrinsic nothing = new NothingIntrinsic();


    public NothingIntrinsic()
    {
        super(null);
    } // Constructor


    public String getType()
    {
        return(TobyInterpreter.INTRINSIC_NOTHING);
    } // getType


    public void setValue(String newValue) throws TobyParseException
    {
        if (newValue != null)
        {
            TobyParseException.throwException(TobyInterpreter.PROCERR_BAD_TYPE,
                                              null);
        } // if

        value = null;
    } // setValue

} // NothingIntrinsic

// end of NothingIntrinsic.java ...


