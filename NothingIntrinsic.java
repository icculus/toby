/**
 * The NothingIntrinsic is somewhat equivalent to C's <em>void</em>.
 *
 * Don't instantiate this class; use NothingIntrinsic.nothing
 *  for your needs.
 *
 *    Copyright (c) 1997-1999 Lighting and Sound Technologies.
 *     Written by Ryan C. Gordon.
 */

public class NothingIntrinsic extends Intrinsic
{
    public static NothingIntrinsic nothing = new NothingIntrinsic();


    private NothingIntrinsic()
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


