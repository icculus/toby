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

package last.toby.gui;
import java.awt.*;
import javax.swing.*;
import last.toby.turtlespace.swing.SwingRenderer;
import last.toby.turtlespace.TurtleSpace;
import last.toby.interpreter.VarWatcher;

    /**
     * The Main panel for TOBY, containing all the other components...
     *
     *    @author Ryan C. Gordon. (icculus@lokigames.com)
     */
public final class TobyPanel extends JComponent implements LayoutManager
{
    private TurtleSpace tspace = null;
    private SwingRenderer renderer = null;
    private TobyInputArea tia = null;
    private VarViewer varViewer = null;
    private JScrollPane varScrollPane = null;
    private TobyFrame frame = null;


    public TobyPanel(TobyFrame frame)
    {
        this.frame = frame;
        setLayout(this);
        buildComponents();
        setVisible(true);
        System.gc();        // clean up everything that setup left behind.
    } // Constructor


    private void buildComponents()
    /**
     *  The various components of this panel are built and
     *    plugged in here.
     *
     *     params : panelDims == size of frame window.
     *    returns : void. The panel will have a bunch more stuff, though.
     */
    {
        renderer = new SwingRenderer();
        tspace = new TurtleSpace(renderer);
        add(renderer);

        tia = new TobyInputArea(frame);
        add(tia);

        varViewer = new VarViewer();
        varScrollPane = new JScrollPane(varViewer,
                                        JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                                        JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        add(varScrollPane);
    } // buildComponents


    public TurtleSpace getTurtleSpace()
    /**
     *  Returns this TobyPanel's TurtleSpace.
     *
     *      params : void.
     *     returns : see above.
     */
    {
        return(tspace);
    } // getTurtleSpace


    public VarWatcher getVarWatcher()
    /**
     *  Returns this TobyPanel's variable viewer component.
     *
     *      params : void.
     *     returns : see above.
     */
    {
        return(varViewer);
    } // getVarWatcher


    public TobyInputArea getInputArea()
    /**
     *  Returns this TobyPanel's input area.
     *
     *      params : void.
     *     returns : see above.
     */
    {
        return(tia);
    } // getInputArea


    public TobyFrame getFrame()
    {
        return(frame);
    } // getFrame


        // LayoutManager implementation...

    public void layoutContainer(Container target)
    {
        int width = getWidth();
        int height = getHeight();
        Insets insets = getInsets();
        int widthHalf;

        width -= (insets.left + insets.right);
        widthHalf = width / 2;

        height -= (insets.top + insets.bottom);

        renderer.setSize(widthHalf, height);
        renderer.setLocation(insets.left, insets.top);
                
        tia.setSize(widthHalf, (int) (height * 0.75));
        tia.setLocation(insets.left + widthHalf, insets.top);

        varScrollPane.setSize(widthHalf, ((int) (height * 0.25)));
        varScrollPane.setLocation(insets.left + widthHalf,
                                    (int) (height * 0.75) + insets.top);
    } // layoutContainer


    public Dimension preferredLayoutSize(Container target)
    {
        Dimension d = target.getParent().getSize();
        Insets insets = target.getParent().getInsets();

        d.width  -= (insets.left + insets.right);
        d.height -= (insets.top + insets.bottom);
        return(d);
    } // preferredLayoutSize


    public Dimension minimumLayoutSize(Container target)
    {
        return(preferredLayoutSize(target));
    } // minimumLayoutSize


    public void addLayoutComponent(String name, Component comp) {}
    public void removeLayoutComponent(Component comp) {}
} // Toby

// end of TobyPanel.java ...

