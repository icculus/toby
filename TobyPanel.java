/*
 * The Main panel for TOBY, containing all the other components...
 *
 *   Copyright (c) Lighting and Sound Technologies, 1997.
 *    Written by Ryan C. Gordon.
 */

import java.awt.*;
import javax.swing.*;

public final class TobyPanel extends JComponent implements LayoutManager
{
    private TurtleSpace tspace = null;
    private TobyInputArea tia = null;
    private VarViewer varViewer = null;
    private JScrollPane varScrollPane = null;
    private Toby frame = null;

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

        tia = new TobyInputArea(frame);
        add(tia);

        varViewer = new VarViewer();
        varScrollPane = new JScrollPane(varViewer,
                                        JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                                        JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        add(varScrollPane);

        tspace.addSourceWatcher((TobyMenuBar) frame.getJMenuBar());
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


    public TobyPanel(Toby frame)
    {
        this.frame = frame;
        setLayout(this);
        buildComponents();
        setVisible(true);
        System.gc();        // clean up everything that setup left behind.
    } // Constructor


    public Toby getFrame()
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

        tspace.setSize(widthHalf, height);
        tspace.setLocation(insets.left, insets.top);
                
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

