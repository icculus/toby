/*
 * TOBY -- An abstract interpreter engine and system for learning.
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

#include "platform/renderers/gtkrenderer/GTKTurtleSpaceRenderer.h"


// !!! make these into instance members!
static volatile bool window_still_not_displayed = true;
static volatile bool quit_event_caught = false;

static gboolean on_gtkcanvas_expose(GtkWidget       *widget,
                                    GdkEventExpose  *event,
                                    gpointer         user_data)
{
    GTKTurtleSpaceRenderer *r = (GTKTurtleSpaceRenderer *) user_data;
    assert(r != NULL);

    gint _x = event->area.x;
    gint _y = event->area.y;
    gint _w = event->area.width;
    gint _h = event->area.height;

    GdkGC *_gc = gdk_gc_new(widget->window);
    GdkPixmap *p = r->getOffscreenPixmap();
    if (p == NULL)
    {
        gdk_rgb_gc_set_foreground(_gc, 0x000000);
        gdk_draw_rectangle(widget->window, _gc, TRUE, _x, _y, _w, _h);
    } // if
    else
    {
        gdk_draw_pixmap(widget->window, _gc, p, _x, _y, _x, _y, _w, _h);
    } // else

    gdk_gc_unref(_gc);

    window_still_not_displayed = false;

    return(FALSE);
} // on_gtkcanvas_expose


GTKTurtleSpaceRenderer::GTKTurtleSpaceRenderer(void)
    : gc(NULL), offscreen_pixmap(NULL),
      canvas(gtk_drawing_area_new())
{
    gtk_widget_ref(canvas);
    gdk_rgb_init();
    gtk_widget_set_default_colormap(gdk_rgb_get_cmap());
    gtk_widget_set_default_visual(gdk_rgb_get_visual());

    gtk_signal_connect(GTK_OBJECT(canvas), "expose_event",
                        GTK_SIGNAL_FUNC(on_gtkcanvas_expose), this);
} // Constructor


GTKTurtleSpaceRenderer::~GTKTurtleSpaceRenderer(void)
{
    if (canvas != NULL)
    {
        gtk_widget_hide(canvas);
        gtk_widget_unref(canvas);
    } // if

    if (offscreen_pixmap != NULL)
    {
        gdk_pixmap_unref(offscreen_pixmap);
    } // if
} // Destructor


bool GTKTurtleSpaceRenderer::resize(int w, int h)
{
    _D(("GTK canvas resize: (%d, %d).\n", w, h));

    if (canvas == NULL)
        return(false);

    GdkPixmap *pix = gdk_pixmap_new(canvas->window, w, h, -1);
    if (pix == NULL)
        return(false);

    GtkStyle *_style = gtk_style_copy(gtk_widget_get_style(canvas));
    memcpy(&(_style->bg[GTK_STATE_NORMAL]), &_style->black, sizeof (GdkColor));
    gtk_widget_set_style(canvas, _style);

    gtk_widget_set_usize(canvas, w, h);
    gtk_widget_show(canvas);

    GdkGC *_gc = gdk_gc_new(canvas->window);
    gdk_rgb_gc_set_foreground(_gc, 0x00000000);
    gdk_draw_rectangle(pix, _gc, TRUE, 0, 0, w, h);

    if (offscreen_pixmap != NULL)
    {
        // !!! move old image to new pixmap. gdk_draw_pixmap(
        gdk_pixmap_unref(offscreen_pixmap);
    } // if

    gdk_gc_unref(_gc);
    offscreen_pixmap = pix;

    while ((window_still_not_displayed) || (gtk_events_pending()))
        gtk_main_iteration();

    while ((canvas->allocation.width != w) || (canvas->allocation.height != h))
        gtk_main_iteration();

    return(true);
} // GTKTurtleSpaceRenderer::resize


void GTKTurtleSpaceRenderer::notifyGrabbed(void)
{
    gc = gdk_gc_new(canvas->window);
    assert(gc != NULL);

    while (gtk_events_pending())
        gtk_main_iteration();
} // GTKTurtleSpaceRenderer::notifyGrabbed


void GTKTurtleSpaceRenderer::notifyUngrabbed(void)
{
    if (gc != NULL)
    {
        gdk_gc_destroy(gc);
        gc = NULL;
    } // if

    while (gtk_events_pending())
        gtk_main_iteration();
} // GTKTurtleSpaceRenderer::notifyUngrabbed


double GTKTurtleSpaceRenderer::getTurtleSpaceWidth(void)
{
    assert(canvas != NULL);
    return((double) canvas->allocation.width);
} // GTKTurtleSpaceRenderer::getTurtleSpaceWidth


double GTKTurtleSpaceRenderer::getTurtleSpaceHeight(void)
{
    assert(canvas != NULL);
    return((double) canvas->allocation.height);
} // GTKTurtleSpaceRenderer::getTurtleSpaceHeight


double GTKTurtleSpaceRenderer::getDesiredTurtleWidth(void)
{
    return(canvas->allocation.width * 0.02);
} // GTKTurtleSpaceRenderer::getDesiredTurtleWidth


double GTKTurtleSpaceRenderer::getDesiredTurtleHeight(void)
{
    return(canvas->allocation.height * 0.02);
} // GTKTurtleSpaceRenderer::getDesiredTurtleHeight


inline guint32 GTKTurtleSpaceRenderer::constructPixelValue(float r, float g,
                                                           float b, float a)
{
        // !!! check this.
    return(
            //(((int) (a * 255.0)) << 24) |
            (((int) (r * 255.0)) << 16) |
            (((int) (b * 255.0)) <<  8) |
            (((int) (g * 255.0))      )
          );
} // FrameBufferTurtleSpaceRenderer::constructPixelValue


void GTKTurtleSpaceRenderer::renderString(double x, double y, double angle,
                                          float r, float b, float g, float a,
                                          const char *str)
{
} // GTKTurtleSpaceRenderer::renderString


void GTKTurtleSpaceRenderer::renderLine(double _x1, double _y1,
                                        double _x2, double _y2,
                                        float r, float b,
                                        float g, float a)
{

    assert(canvas != NULL);
    assert(offscreen_pixmap != NULL);
    assert(gc != NULL);

    guint32 pval = constructPixelValue(r, g, b, a);

    gdk_rgb_gc_set_foreground(gc, pval);

    int x1 = TobyGeometry::roundDoubleToInt(_x1);
    int y1 = TobyGeometry::roundDoubleToInt(_y1);
    int x2 = TobyGeometry::roundDoubleToInt(_x2);
    int y2 = TobyGeometry::roundDoubleToInt(_y2);

    gdk_draw_line(offscreen_pixmap, gc, x1, y1, x2, y2);
    gdk_draw_line(canvas->window, gc, x1, y1, x2, y2);
} // GTKTurtleSpaceRenderer::renderLine


void GTKTurtleSpaceRenderer::renderTurtle(Turtle *t) throw (ExecException *)
{
} // GTKTurtleSpaceRenderer::renderTurtle


void GTKTurtleSpaceRenderer::blankTurtle(Turtle *t) throw (ExecException *)
{
} // GTKTurtleSpaceRenderer::blankTurtle


void GTKTurtleSpaceRenderer::cleanup(void) throw (ExecException *)
{
    assert(canvas != NULL);
    assert(gc != NULL);

    gdk_rgb_gc_set_foreground(gc, 0x00000000);
    gdk_draw_rectangle(canvas->window, gc, TRUE, 0, 0,
                       canvas->allocation.width, canvas->allocation.height);

    while (gtk_events_pending())
        gtk_main_iteration();
} // GTKTurtleSpaceRenderer::cleanup


GtkWidget *GTKTurtleSpaceRenderer::getDrawingAreaWidget(void)
{
    return(canvas);
} // GTKTurtleSpaceRenderer::getDrawingAreaWidget


GdkPixmap *GTKTurtleSpaceRenderer::getOffscreenPixmap(void)
{
    return(offscreen_pixmap);
} // GTKTurtleSpaceRenderer::getOffscreenPixmap


static gint on_gtk_window_delete(GtkWidget *widget,
                          GdkEvent  *event,
                          gpointer   data )
{
    quit_event_caught = true;
    return(FALSE);
} // on_gtk_window_delete


TurtleSpaceRenderer *__platformBuildStandaloneRenderer(char *winTitle,
                                                       int *argc, char ***argv)
{
    window_still_not_displayed = true;
    quit_event_caught = false;

    gtk_init(argc, argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    assert(window != NULL);

        // prevent user from resizing window...
    gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, TRUE);

    gtk_window_set_title(GTK_WINDOW(window), winTitle);

    GTKTurtleSpaceRenderer *retval = new GTKTurtleSpaceRenderer();
    assert(retval != NULL);
    GtkWidget *darea = retval->getDrawingAreaWidget();
    assert(darea != NULL);

    gtk_signal_connect(GTK_OBJECT(window), "delete_event",
                        GTK_SIGNAL_FUNC(on_gtk_window_delete), darea);

    gtk_widget_realize(window);
    gtk_container_add(GTK_CONTAINER(window), darea);
    gtk_widget_realize(darea);
    gtk_widget_show(darea);
    gtk_widget_show(window);

    while (gtk_events_pending())
        gtk_main_iteration();

    return(retval);
} // __platformBuildStandaloneRenderer


bool __platformRendererDoEvents(void)
{
    while (gtk_events_pending())
        gtk_main_iteration();

    return(!quit_event_caught);
} // __platformRendererDoEvents

// end of GTKTurtleSpaceRenderer.cpp ...

