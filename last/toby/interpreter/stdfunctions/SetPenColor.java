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

package last.toby.interpreter.stdfunctions;

import last.toby.util.TobyLanguage;
import last.toby.turtlespace.TurtleSpace;
import last.toby.interpreter.*;
import last.toby.exceptions.*;

    /**
     *  !!! comment me!
     *  @author Ryan C. Gordon.
     */
public class SetPenColor extends StdFuncLogicContext
{
        /*
         * How I got these numbers:
         *   These color choices are ripped directly from QuickBasic's
         *   COLOR command in text mode. So, (1) is dark blue, (2) is
         *   dark green, etc...Then I used the GIMP color tool
         *   (http://www.gimp.org/), to find the equivalents in RGB format.
         *   GIMP gives each color element as a number from 0 to 255, so to
         *   make it more abstract (0.00f to 1.00f), I used a calculator
         *   (KDE's kCalc, in this case), and just divided each element
         *   by 255. Even though 11 decimal places of precision might be
         *   overkill, it can't hurt, either...
         */
    protected static final float redArray[]   =
    {
        0.00000000000f,  // black         (000)
        0.00000000000f,  // dark blue     (000)
        0.00000000000f,  // dark green    (000)
        0.00000000000f,  // dark cyan     (000)
        0.62745098039f,  // dark red      (160)
        0.77254901961f,  // dark magenta  (197)
        0.50588235294f,  // brown         (129)
        0.78431372549f,  // gray          (200)
        0.35294117647f,  // dark gray     (090)
        0.00000000000f,  // bright blue   (000)
        0.00000000000f,  // bright green  (000)
        0.35686274510f,  // bright cyan   (091)
        1.00000000000f,  // bright red    (255)
        1.00000000000f,  // bright pink   (255)
        1.00000000000f,  // bright yellow (255)
        1.00000000000f   // bright white  (255)
    }; // redArray

    protected static final float greenArray[] =
    {
        0.00000000000f,  // black         (000)
        0.10196078431f,  // dark blue     (026)
        0.60000000000f,  // dark green    (153)
        0.56470588235f,  // dark cyan     (144)
        0.00000000000f,  // dark red      (000)
        0.00000000000f,  // dark magenta  (000)
        0.35294117647f,  // brown         (090)
        0.78431372549f,  // gray          (200)
        0.35294117647f,  // dark gray     (090)
        0.43137254902f,  // bright blue   (200)
        1.00000000000f,  // bright green  (090)
        1.00000000000f,  // bright cyan   (110)
        0.00000000000f,  // bright red    (000)
        0.00000000000f,  // bright pink   (000)
        1.00000000000f,  // bright yellow (255)
        1.00000000000f   // bright white  (255)
    }; // greenArray

    protected static final float blueArray[]  =
    {
        0.00000000000f,  // black         (000)
        0.76862745098f,  // dark blue     (196)
        0.17647058824f,  // dark green    (045)
        0.54117647059f,  // dark cyan     (138)
        0.00000000000f,  // dark red      (000)
        0.79215686275f,  // dark magenta  (202)
        0.06274509803f,  // brown         (016)
        0.78431372549f,  // gray          (200)
        0.35294117647f,  // dark gray     (090)
        1.00000000000f,  // bright blue   (255)
        0.14901960784f,  // bright green  (038)
        1.00000000000f,  // bright cyan   (255)
        0.00000000000f,  // bright red    (000)
        1.00000000000f,  // bright pink   (255)
        0.00000000000f,  // bright yellow (000)
        1.00000000000f   // bright white  (255)
    }; // blueArray


    protected final String getFuncNameImpl()
    {
        return("__$STDFUNC$__setPenColor");
    } // getFuncName

    protected final int getParamCountImpl()
    {
        return(1);
    } // getFuncName

    protected Intrinsic stdFuncExecuteImpl() throws FlowException
    {
        int stdColor = globalContext.localFromTopOfStack(0).getIntValue();

        if ((stdColor < 0) || (stdColor >= redArray.length))
            ExecException._throw(TobyLanguage.BAD_ARGUMENT);

        float r  = redArray[stdColor];
        float g  = greenArray[stdColor];
        float b  = blueArray[stdColor];

//        _D("stdFuncExecuteImpl", "setting pen to stdcolor ("+stdColor+").");
//        _D("stdFuncExecuteImpl", "...that's ("+r+","+g+","+b+")...");

        tspace.setPenColor(r, g, b);
        return(NothingIntrinsic.nothing);
    } // executeImpl
} // SetPenColor

// end of SetPenColor.java ...


