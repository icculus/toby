/** !!! comment. copyright. */

public abstract class Intrinsic extends Throwable
{
        // instance variables...
    private String identifier;
    protected String value;


    public Intrinsic(String identifier, String initValue)
                                   throws TobyParseException
    {
        this.identifier = identifier;
        setValue(initValue);
    } // Constructor


    public Intrinsic(String identifier)
    {
        this.identifier = identifier;
    } // Constructor


    public String getIdentifier()
    {
        return(identifier);
    } // getIdentifier


    public boolean equals(Object obj)
    {
        boolean retVal;

        if (obj instanceof Intrinsic)
            retVal = value.equals(((Intrinsic) obj).getValue());
        else
            retVal = super.equals(obj);

        return(retVal);
    } // equals


    public String toString()
    {  // !!!
        return("Intrinsic(" + identifier  + " == " + getValue() + ")");
    } // toString


    public String getValue()
    {
        return(value);
    } // getValue

    public abstract void setValue(String newValue) throws TobyParseException;
    public abstract String getType();
} // Intrinsic

// end of Intrinsic.java ...

