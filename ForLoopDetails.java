/**
 * This class maintains details about an instance of a FOR loop
 *  during the interpretation of TOBY code...
 *
 *    Copyright (c) 1997-1999 Lighting and Sound Technologies.
 *     Written by Ryan C. Gordon.
 */

public class ForLoopDetails extends LoopDetails
{
    public Intrinsic var;
    public double step;

    public ForLoopDetails(int srcLine)
    {
        super(srcLine);
    } // Constructor

} // ForLoopDetails

// end of ForLoopDetails.java ...

