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

    /**
     *  !!! comment me!
     *
     *  @author Ryan C. Gordon.
     */
public class ConditionalLogicContext extends LogicContext
{
    protected ElseLogicContext elseContext = null;
    protected ExpressionLogicContext condExpr = null;

    public ConditionalLogicContext(int sourceLine, ExpressionLogicContext cond)
    {
        super(sourceLine);
        this.condExpr = cond;
    } // Constructor

    public void setElse(ElseLogicContext elseContext)
    {
        this.elseContext = elseContext;

            // !!! Change?
            // this is scary. For variable scoping, this needs to have
            // a parent, but for execution, this can't be a child.
        this.elseContext.setParent(this);
    } // setElse

    public void linkImpl(GlobalLogicContext glob) throws ParseException
    {
        if (this.elseContext != null)
            this.elseContext.link(glob);

        if (this.condExpr != null)
            this.condExpr.link(glob);
    } // link

    protected void freeResourcesImpl()
    {
        if (this.elseContext != null)
            this.elseContext.freeResources(true);

        if (this.condExpr != null)
            this.condExpr.freeResources(true);

        this.elseContext = null;
        this.condExpr = null;
    } // freeResourcesImpl

    public Intrinsic executeImpl() throws FlowException
    {
        if (this.condExpr.execute().getBooleanValue())
            defaultExecuteImpl();
        else if (this.elseContext != null)
            this.elseContext.execute();

        return(NothingIntrinsic.nothing);
    } // execute

} // ConditionalLogicContext

// end of ConditionalLogicContext.java ...

