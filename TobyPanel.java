/*
 * The Main panel for TOBY, containing all the other components...
 *
 *   Copyright (c) Lighting and Sound Technologies, 1997.
 *    Written by Ryan C. Gordon.
 */

import java.awt.*;


public final class TobyPanel extends Panel implements LayoutManager
{
    private TurtleSpace tspace = null;
    private TobyInputArea tia = null;
    private VarViewer varViewer = null;    

    private void buildComponents()
    /**
     *  The various components of this panel are built and
     *    plugged in here.
     *
     *     params : panelDims == size of frame window.
     *    returns : void. The panel will have a bunch more stuff, though.
     */
    {

        /*
         *  CHANGE TobyInterpreter() TO SUIT YOUR VERSION OF TURTLESPACE!
         */

        tspace = new TobyInterpreter();
        add(tspace);

        tia = new TobyInputArea();
        add(tia);

        varViewer = new VarViewer();
        add(varViewer);

        tspace.addSourceWatcher(tia);
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
     *  Returns this TobyPanel's TurtleSpace.
     *
     *      params : void.
     *     returns : see above.
     */
    {
        return(tia);
    } // getTurtleSpace


    public TobyPanel()
    {
        setLayout(this);
        buildComponents();
        setVisible(true);
        System.gc();        // clean up everything that setup left behind.
    } // Constructor



        // LayoutManager implementation...

    public void layoutContainer(Container target)
    {
        Dimension d = target.getSize();

        d.width /= 2;

        tspace.setSize(d.width, d.height);
        tspace.setLocation(0, 0);
                
        tia.setSize(d.width, (int) (d.height * 0.75));
        tia.setLocation(d.width, 0);

        varViewer.setSize(d.width, (int) (d.height * 0.25));
        varViewer.setLocation(d.width, (int) (d.height * 0.75));
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

