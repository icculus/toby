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

package last.toby.interpreter;
import last.toby.exceptions.*;
import last.toby.util.TobyLanguage;

    /**
     *  !!! write me.
     *   @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public class StringIntrinsic extends Intrinsic
{
    protected String data = null;

    public StringIntrinsic(String str)
    {
        this.data = str;
    } // Constructor

    public StringIntrinsic()
    {
        this("");
    } // Constructor

    public void setValue(String newVal) throws FlowException
    {
        this.data = newVal;
    } // setValue

    public void setValue(Intrinsic newVal) throws FlowException
    {
        this.data = newVal.getStringValue();
    } // setValue

    public String getStringValue() throws FlowException
    {
        return(this.data);
    } // getValue

    public int compare(Intrinsic intr) throws FlowException
    {
        String compareStr = intr.getStringValue();
        return(compareStr.compareTo(data));
    } // compare

    public boolean isEqual(Intrinsic intr) throws FlowException
    {
        return(this.compare(intr) == 0);
    } // isEqual

    public boolean isConstantValue()
    {
        return(true);
    } // isConstantValue

    protected Intrinsic getCopyImpl()
    {
        return(new StringIntrinsic(this.data));
    } // getCopyImpl

    protected void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        // no link, here.
    } // linkImpl

} // StringIntrinsic

// end of StringIntrinsic.java ...

