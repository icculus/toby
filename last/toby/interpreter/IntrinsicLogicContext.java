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
import last.toby.util.*;
import last.toby.exceptions.*;

    /**
     *  !!! comment me!
     *  @author Ryan C. Gordon.
     */
public class IntrinsicLogicContext extends ExpressionLogicContext
{
    protected Intrinsic value = null;

    public IntrinsicLogicContext(int sourceLine, Intrinsic value)
    {
        super(sourceLine);
        this.value = value;
    } // Constructor

    public boolean verifyNewChildType(NodeTree child)
    {
        return(false);    // no kids allowed.
    } // verifyNewChildType

    protected void freeResourcesImpl()
    {
        this.value = null;
    } // freeResourcesImpl

    protected Intrinsic simplify() throws ParseException
    {
        return(null);
    } // simplify

    public Intrinsic executeImpl() throws FlowException
    {
//        _D("executeImpl", "returning a " + value.getClass().getName() + "...");
        return((Intrinsic) value);
    } // execute

    public void linkImpl(GlobalLogicContext glob) throws ParseException
    {
//        _D("linkImpl", "value is a " + value.getClass().getName() + "...");
        if (value != null)
            value.link(glob);
    } // link

} // IntrinsicLogicContext

// end of IntrinsicLogicContext.java ...

