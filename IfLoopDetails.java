/**
 * This class maintains details about an instance of an IF block
 *  during the interpretation of TOBY code...
 *
 *    Copyright (c) 1997-1999 Lighting and Sound Technologies.
 *     Written by Ryan C. Gordon.
 */

public class IfLoopDetails extends LoopDetails
{
    public boolean isElse;

    public IfLoopDetails(int srcLine, boolean isElse)
    {
        super(srcLine);
        this.isElse = isElse;
    } // Constructor
} // IfLoopDetails

// end of IfLoopDetails.java ...


