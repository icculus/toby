/*
 * Toby -- A programming language for learning.
 * Copyright (C) 2007  Ryan C. Gordon.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

#if PLATFORM_MACOSX
#include <Carbon/Carbon.h>
#endif

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/wfstream.h>
#include <wx/aboutdlg.h>
#include <wx/splitter.h>
#include <wx/filename.h>
#include <wx/print.h>
#include <wx/printdlg.h>

#include "toby_app.h"

// define something.
#if ((!TOBY_WX_BUILD_STANDALONE) && (!TOBY_WX_BUILD_IDE))
#error Nothing to build here.
#elif ((TOBY_WX_BUILD_STANDALONE) && (!TOBY_WX_BUILD_IDE))
#define TOBY_WX_STANDALONE_DEFAULT true
#else  // Both IDE and standalone available, or just IDE.
#define TOBY_WX_STANDALONE_DEFAULT false
#endif


// Interfaces ...

// TurtleSpace is the canvas we draw on...it manages a backing store for
//  repaints, if necessary for a given wxWidgets target, and handles
//  the canvas if the window resizes.
class TurtleSpace : public wxWindow
{
public:
    TurtleSpace(wxWindow *parent);
    virtual ~TurtleSpace();
    inline void startRun(bool runForPrinting);
    inline void stopRun();
    inline const wxFont *getFont() const { return &this->font; }
    inline wxBitmap *getBacking() const;
    inline void scaleXY(lua_Number &x, lua_Number &y) const;

    bool drawString(lua_Number x, lua_Number y, const wxString &str,
                    lua_Number angle, int r, int g, int b);
    void drawLine(lua_Number x1, lua_Number y1, lua_Number x2, lua_Number y2,
                  int r, int g, int b);
    void drawTurtle(const Turtle *turtle, void *data);
    void cleanup(int r, int g, int b, bool force=false);

    // wxWidgets event handlers...
    void onResize(wxSizeEvent &evt);
    void onErase(wxEraseEvent &evt);
    void onPaint(wxPaintEvent &evt);

private:
    inline void nukeDC(wxMemoryDC **dc) { delete *dc; *dc = NULL; }
    inline wxMemoryDC *getBackingDC() const { return this->backingDC; }
    inline void constructBackingDC();
    inline void calcOffset(int &xoff, int &yoff) const;
    inline void clipDC(wxDC *dc, int xoff, int yoff) const;

    const wxFont font;
    int clientW;  // width of physical window (changes on resize event).
    int clientH;  // height of physical window (changes on resize event).
    int backingW;  // width of backing store (changes on startRun()).
    int backingH;  // height of backing store (changes on startRun()).
    wxBitmap *backing;
    wxMemoryDC *backingDC;
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TurtleSpace, wxWindow)
    EVT_SIZE(TurtleSpace::onResize)
    EVT_PAINT(TurtleSpace::onPaint)
    EVT_ERASE_BACKGROUND(TurtleSpace::onErase)
END_EVENT_TABLE()


// Printing support class...
class TobyPrintout: public wxPrintout
{
public:
    TobyPrintout(const wxChar *title = _T("Toby")) : wxPrintout(title) {}
    virtual bool OnPrintPage(int page);
    virtual bool HasPage(int page);
    virtual void GetPageInfo(int *minPg, int *maxPg, int *selFrom, int *selTo);
};


enum TobyMenuCommands
{
    // start with standard menu items, since using the wxIDs will map them
    //  to sane things in the platform's UI (gnome icons in GTK+, moves the
    //  about and quit items to the Apple menu on Mac OS X, etc).
    MENUCMD_About = wxID_ABOUT,
    MENUCMD_Quit = wxID_EXIT,
    MENUCMD_Open = wxID_OPEN,
    MENUCMD_New = wxID_NEW,
    MENUCMD_Save = wxID_SAVE,
    MENUCMD_SaveAs = wxID_SAVEAS,
    MENUCMD_PageSetup = wxID_PAGE_SETUP,
    MENUCMD_PrintPreview = wxID_PRINT_SETUP,
    MENUCMD_Print = wxID_PRINT,

    // non-standard menu items go here.
    MENUCMD_Run = wxID_HIGHEST,
    MENUCMD_RunForPrinting,
    MENUCMD_Stop,
    MENUCMD_Step,
    MENUCMD_SaveAsImage,
    MENUCMD_Cleanup,
    MENUCMD_Website,
    MENUCMD_License,
};

// TobyFrame is a standard wxFrame, but adds an interface for getting
//  the actual TurtleSpace canvas and other highlevel wankery...there are two
//  subclasses of this: one is a standalone window that only contains a
//  turtlespace, and one is a full layout of controls for editing, debugging,
//  etc.
// So to get the TurtleSpace to draw on from any arbitrary place, you'd do
//  this:  TurtleSpace *ts = wxGetApp().getTobyFrame()->getTurtleSpace();
class TobyFrame : public wxFrame
{
public:
    TobyFrame();
    virtual ~TobyFrame();
    TurtleSpace *getTurtleSpace() const { return this->turtleSpace; }
    static const wxPoint getPreviousPos();
    static const wxSize getPreviousSize();
    void openFile(const wxString &path);
    void startRun();
    void stopRun();
    inline void pumpEvents();
    inline void repaintTurtlespace();
    inline void runProgram(bool printing, bool _breakAtStart=false);
    void toggleWidgetsRunnable(bool readyToRun);
    inline void requestQuit();
    inline bool isQuitting() const { return this->quitting; }

    // subclasses fill in these.
    virtual void pauseReached(int line, int stopped, int bp, int ticks) {}
    virtual void toggleWidgetsRunnableImpl(bool readyToRun) {}
    virtual bool shouldVetoClose() { return false; }
    virtual void closeImpl() {}
    virtual void openFileImpl(char *buf) = 0;
    virtual char *getProgramImpl() = 0;
    virtual void onContinueImpl() {}

    // wxWidgets event handlers...
    void onIdle(wxIdleEvent &evt);
    void onClose(wxCloseEvent &evt);
    void onResize(wxSizeEvent &evt);
    void onMove(wxMoveEvent &evt);
    void onMenuQuit(wxCommandEvent &evt);
    void onMenuPageSetup(wxCommandEvent &evt);
    void onMenuPrintPreview(wxCommandEvent &evt);
    void onMenuPrint(wxCommandEvent &evt);
    void onMenuAbout(wxCommandEvent &evt);
    void onMenuWebsite(wxCommandEvent &evt);
    void onMenuLicense(wxCommandEvent &evt);
    void onMenuOpen(wxCommandEvent &evt);
    void onMenuSaveAsImage(wxCommandEvent &evt);
    void onMenuCleanup(wxCommandEvent &evt);
    void onMenuRun(wxCommandEvent &evt);
    void onMenuStop(wxCommandEvent &evt);
    void onMenuStep(wxCommandEvent &evt);
    void onMenuRunForPrinting(wxCommandEvent &evt);

protected:
    void updateTitleBar();
    wxStopWatch profileStopwatch;
    TurtleSpace *turtleSpace;
    wxString filename;
    bool modified;
    int nonMaximizedX;
    int nonMaximizedY;
    int nonMaximizedWidth;
    int nonMaximizedHeight;
    bool quitting;
    bool runForPrinting;
    bool breakAtStart;
    char *execProgram;
    wxMenu *fileMenu;
    wxMenu *runMenu;
    wxMenu *helpMenu;
    wxMenuBar *menuBar;

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TobyFrame, wxFrame)
    EVT_IDLE(TobyFrame::onIdle)
    EVT_CLOSE(TobyFrame::onClose)
    EVT_SIZE(TobyFrame::onResize)
    EVT_MOVE(TobyFrame::onMove)
    EVT_MENU(MENUCMD_Open, TobyFrame::onMenuOpen)
    EVT_MENU(MENUCMD_Quit, TobyFrame::onMenuQuit)
    EVT_MENU(MENUCMD_SaveAsImage, TobyFrame::onMenuSaveAsImage)
    EVT_MENU(MENUCMD_PageSetup, TobyFrame::onMenuPageSetup)
    EVT_MENU(MENUCMD_PrintPreview, TobyFrame::onMenuPrintPreview)
    EVT_MENU(MENUCMD_Print, TobyFrame::onMenuPrint)
    EVT_MENU(MENUCMD_About, TobyFrame::onMenuAbout)
    EVT_MENU(MENUCMD_Website, TobyFrame::onMenuWebsite)
    EVT_MENU(MENUCMD_License, TobyFrame::onMenuLicense)
    EVT_MENU(MENUCMD_Cleanup, TobyFrame::onMenuCleanup)
    EVT_MENU(MENUCMD_Run, TobyFrame::onMenuRun)
    EVT_MENU(MENUCMD_RunForPrinting, TobyFrame::onMenuRunForPrinting)
    EVT_MENU(MENUCMD_Stop, TobyFrame::onMenuStop)
    EVT_MENU(MENUCMD_Step, TobyFrame::onMenuStep)
END_EVENT_TABLE()


#if TOBY_WX_BUILD_STANDALONE
// This is a TobyFrame that only provides a TurtleSpace, basic menu items,
//  and no other UI.
class TobyStandaloneFrame : public TobyFrame
{
public:
    TobyStandaloneFrame();
    virtual ~TobyStandaloneFrame() { delete[] this->sourceCode; }
    virtual void openFileImpl(char *prog);
    virtual char *getProgramImpl();

private:
    char *sourceCode;
};
#endif


#if TOBY_WX_BUILD_IDE
// This is a TobyFrame that provides text editing and debugging facilities.
class TobyIDEFrame : public TobyFrame
{
public:
    TobyIDEFrame();
    virtual void openFileImpl(char *prog);
    virtual char *getProgramImpl();
    virtual bool shouldVetoClose();
    virtual void toggleWidgetsRunnableImpl(bool readyToRun);
    virtual void pauseReached(int line, int stopped, int bp, int ticks);
    virtual void closeImpl();
    virtual void onContinueImpl();

    // wxWidgets event handlers...
    void onMenuOpen(wxCommandEvent &evt);
    void onMenuSave(wxCommandEvent &evt);
    void onMenuSaveAs(wxCommandEvent &evt);
    void onTextModified(wxCommandEvent &evt);
    void onCallstackSelected(wxCommandEvent &evt);

private:
    void updateVariablesCtrl(int frame);
    wxSplitterWindow *topSplit;
    wxSplitterWindow *ideSplit;
    wxSplitterWindow *dbgSplit;
    wxTextCtrl *textCtrl;
    wxListBox *callstackCtrl;
    wxListBox *variablesCtrl;
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TobyIDEFrame, TobyFrame)
    EVT_MENU(MENUCMD_Open, TobyIDEFrame::onMenuOpen)
    EVT_MENU(MENUCMD_Save, TobyIDEFrame::onMenuSave)
    EVT_MENU(MENUCMD_SaveAs, TobyIDEFrame::onMenuSaveAs)
    EVT_TEXT(wxID_ANY, TobyIDEFrame::onTextModified)  // !!! FIXME: not ID_ANY!
    EVT_LISTBOX(wxID_ANY, TobyIDEFrame::onCallstackSelected)  // !!! FIXME: not ID_ANY!
END_EVENT_TABLE()
#endif


class TobyLicenseDialog : public wxDialog
{
public:
    TobyLicenseDialog(wxWindow *parent);
    void onOK(wxCommandEvent &evt);

    enum
    {
        dlgFlags = wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX |
                   wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxCAPTION
    };

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TobyLicenseDialog, wxDialog)
    EVT_BUTTON(wxID_OK, TobyLicenseDialog::onOK)
END_EVENT_TABLE()


// This is the the Application itself.
class TobyWxApp : public wxApp
{
public:
    TobyWxApp() : mainWindow(NULL), printData(NULL) { /* no-op. */ }
    virtual bool OnInit();
    virtual int OnExit();
    TobyFrame *getTobyFrame() const { return this->mainWindow; }
    inline wxPrintData *getPrintData();
    inline long getTicks() { return this->processStopwatch.Time(); }

private:
    TobyFrame *mainWindow;
    wxPrintData *printData;
    wxStopWatch processStopwatch;
};

DECLARE_APP(TobyWxApp)




// implementations of callbacks during execution of Toby programs...

long TOBY_getTicks(void)
{
    return wxGetApp().getTicks();
} // TOBY_getTicks


void TOBY_yieldCPU(int ms)
{
    ::wxMilliSleep(ms);
} // TOBY_yieldCPU


void TOBY_startRun()
{
    wxGetApp().getTobyFrame()->startRun();
} // TOBY_startRun


void TOBY_stopRun()
{
    wxGetApp().getTobyFrame()->stopRun();
} // TOBY_stopRun


void TOBY_pumpEvents()
{
    wxGetApp().getTobyFrame()->pumpEvents();
} // TOBY_pumpEvents


void TOBY_putToScreen()
{
    wxGetApp().getTobyFrame()->repaintTurtlespace();
} // TOBY_putToScreen


int TOBY_drawString(lua_Number x, lua_Number y, const char *utf8str,
                    lua_Number angle, int r, int g, int b)
{
    const wxString wxstr(utf8str, wxConvUTF8);
    TurtleSpace *tspace = wxGetApp().getTobyFrame()->getTurtleSpace();
    return tspace->drawString(x, y, wxstr, angle, r, g, b) ? 1 : 0;
} // TOBY_drawString


void TOBY_drawLine(lua_Number x1, lua_Number y1, lua_Number x2, lua_Number y2,
                   int r, int g, int b)
{
    wxGetApp().getTobyFrame()->getTurtleSpace()->drawLine(x1,y1,x2,y2,r,g,b);
} // TOBY_drawLine


void TOBY_drawTurtle(const Turtle *turtle, void *data)
{
    wxGetApp().getTobyFrame()->getTurtleSpace()->drawTurtle(turtle, data);
} // TOBY_drawTurtle


void TOBY_pauseReached(int line, int fullstop, int breakpoint, int ticks)
{
    wxGetApp().getTobyFrame()->pauseReached(line, fullstop, breakpoint, ticks);
} // TOBY_pauseReached


void TOBY_cleanup(int r, int g, int b)
{
    wxGetApp().getTobyFrame()->getTurtleSpace()->cleanup(r, g, b);
} // TOBY_cleanup


void TOBY_messageBox(const char *msg)
{
    ::wxMessageBox(wxString(msg, wxConvUTF8), wxString(wxT("Toby")));
} // TOBY_messageBox




// The rest of the application...

TurtleSpace::TurtleSpace(wxWindow *parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition,
               wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
    , font(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)
    , clientW(1)
    , clientH(1)
    , backingW(1)
    , backingH(1)
    , backing(NULL)
    , backingDC(NULL)
{
    // no-op
} // TurtleSpace::TurtleSpace


TurtleSpace::~TurtleSpace()
{
    delete this->backingDC;
    delete this->backing;
} // TurtleSpace::~TurtleSpace


void TurtleSpace::constructBackingDC()
{
    wxASSERT(this->backingDC == NULL);
    this->backingDC = new wxMemoryDC(*this->backing);
    this->backingDC->SetFont(this->font);
} // TurtleSpace::constructBackingDC


wxBitmap *TurtleSpace::getBacking() const
{
    wxASSERT(this->backingDC == NULL);
    return this->backing;
} // TurtleSpace::getBacking


void TurtleSpace::scaleXY(lua_Number &x, lua_Number &y) const
{
    // !!! FIXME: Fixed point?
    x = (((lua_Number) this->backingW) * (x / N(1000)));
    y = (((lua_Number) this->backingH) * (y / N(1000)));
} // TurtleSpace::scaleXY


void TurtleSpace::calcOffset(int &xoff, int &yoff) const
{
    xoff = (this->clientW - this->backingW) / 2;
    yoff = (this->clientH - this->backingH) / 2;
} // TurtleSpace::calcOffset


void TurtleSpace::clipDC(wxDC *dc, int xoff, int yoff) const
{
    const int w = this->backingW;
    const int h = this->backingH;
    if ((w < this->clientW) || (h < this->clientH))
    {
        dc->DestroyClippingRegion();
        dc->SetClippingRegion(xoff, yoff, w, h);
    } // if
} // TurtleSpace::clipDC


void TurtleSpace::drawLine(lua_Number x1, lua_Number y1,
                           lua_Number x2, lua_Number y2,
                           int r, int g, int b)
{
    wxMemoryDC *dc = this->getBackingDC();
    if (dc != NULL)
    {
        this->scaleXY(x1, y1);
        this->scaleXY(x2, y2);
        dc->SetPen(wxPen(wxColour(r, g, b)));
        dc->DrawLine((wxCoord) x1, (wxCoord) y1, (wxCoord) x2, (wxCoord) y2);
    } // if
} // TurtleSpace::drawLine


bool TurtleSpace::drawString(lua_Number x, lua_Number y, const wxString &str,
                             lua_Number angle, int r, int g, int b)
{
    if (!this->getFont()->IsOk())
        return false;

    wxMemoryDC *dc = this->getBackingDC();
    if (dc != NULL)
    {
        this->scaleXY(x, y);
        const wxColour color(r, g, b);
        dc->SetTextForeground(color);
        dc->DrawRotatedText(str, (wxCoord) x, (wxCoord) y, angle);
    } // if

    return true;
} // TurtleSpace::drawString


void TurtleSpace::drawTurtle(const Turtle *turtle, void *data)
{
    wxDC *dc = ( (data != NULL) ? ((wxDC *) data) : this->getBackingDC() );
    if (dc != NULL)
    {
        int xoff = 0;
        int yoff = 0;

        if (data != NULL)  // not the backing store? Clip it.
        {
            this->calcOffset(xoff, yoff);
            this->clipDC(dc, xoff, yoff);
        } // if

        TurtlePoint tpts[4];
        const lua_Number tx = turtle->pos.x;
        const lua_Number ty = turtle->pos.y;
        tpts[0].x = turtle->points[0].x + tx;
        tpts[0].y = turtle->points[0].y + ty;
        tpts[1].x = turtle->points[1].x + tx;
        tpts[1].y = turtle->points[1].y + ty;
        tpts[2].x = turtle->points[2].x + tx;
        tpts[2].y = turtle->points[2].y + ty;
        tpts[3].x = turtle->points[3].x + tx;
        tpts[3].y = turtle->points[3].y + ty;

        this->scaleXY(tpts[0].x, tpts[0].y);
        this->scaleXY(tpts[1].x, tpts[1].y);
        this->scaleXY(tpts[2].x, tpts[2].y);
        this->scaleXY(tpts[3].x, tpts[3].y);

        wxPoint points[3] =
        {
            wxPoint((wxCoord) tpts[0].x, (wxCoord) tpts[0].y),
            wxPoint((wxCoord) tpts[1].x, (wxCoord) tpts[1].y),
            wxPoint((wxCoord) tpts[2].x, (wxCoord) tpts[2].y),
        };

        const wxColor color(0, 255, 0);  // full green.
        dc->SetPen(wxPen(color));
        dc->SetBrush(wxBrush(color));
        dc->DrawPolygon(3, points, xoff, yoff);
        dc->SetPen(wxPen(wxColor(0, 0, 255)));  // full blue.
        dc->DrawLine( ((wxCoord)tpts[0].x) + ((wxCoord)xoff),
                      ((wxCoord)tpts[0].y) + ((wxCoord)yoff),
                      ((wxCoord)tpts[3].x) + ((wxCoord)xoff),
                      ((wxCoord)tpts[3].y) + ((wxCoord)yoff) );
    } // if
} // TurtleSpace::drawTurtle


void TurtleSpace::cleanup(int r, int g, int b, bool force)
{
    wxMemoryDC *dc = this->backingDC;
    const bool haveDC = (dc != NULL);
    if ((!haveDC) && (force))
    {
        this->constructBackingDC();
        dc = this->backingDC;
    } // if

    if (dc != NULL)
    {
        dc->SetBackground(wxBrush(wxColour(r, g, b)));
        dc->Clear();
    } // if

    if (!haveDC)
        this->nukeDC(&this->backingDC);
} // TurtleSpace::cleanup


void TurtleSpace::startRun(bool runForPrinting)
{
    int w = this->clientW;
    int h = this->clientH;

    if (runForPrinting)
    {
        // !!! FIXME
        // Run with a target of 300dpi on 8x11 paper. This isn't really
        //  right, but it's easy for now. Ideally we throw up a printer
        //  dialog and get the exact dimensions from the DC, but that adds
        //  all sorts of event queue tapdancing...
        w = (8 * 300);
        h = (11 * 300);
    } // if

    if (w > h)
        w = h;
    else
        h = w;

    // (this may be NULL already, that's okay.)
    this->nukeDC(&this->backingDC);

    // resized since last run?
    if ((w != this->backingW) || (h != this->backingH))
    {
        delete this->backing;  // (this may be NULL already, that's okay.)
        this->backing = NULL;
    } // if

    // Build new backing store if necessary...
    if (this->backing == NULL)
    {
        this->backing = new wxBitmap(w, h);
        this->backingW = w;
        this->backingH = h;
    } // if

    this->constructBackingDC();
} // TurtleSpace::startRun


void TurtleSpace::stopRun()
{
    this->nukeDC(&this->backingDC);  // flush to bitmap.
} // TurtleSpace::stopRun


void TurtleSpace::onResize(wxSizeEvent &evt)
{
    // Just cache the dimensions, since we're spending an enormous amount of
    //  time looking them up over and over during program execution.
    this->GetClientSize(&this->clientW, &this->clientH);
} // TurtleSpace::onResize


void TurtleSpace::onErase(wxEraseEvent &evt)
{
    // We don't want to do anything here, since we redraw the whole window
    //  in the Paint event...catching the erase event and doing nothing
    //  prevents flicker on some platforms, though.
    (void) evt.GetDC();
} // TurtleSpace::onErase


void TurtleSpace::onPaint(wxPaintEvent &evt)
{
    wxPaintDC dc(this);

    int xoff, yoff;
    this->calcOffset(xoff, yoff);

    int r, g, b;
    TOBY_background(&r, &g, &b);
    dc.SetBackground(wxBrush(wxColour(r, g, b)));
    
    if (this->backing == NULL)
        dc.Clear();   // no backing store, just dump the whole thing.
    else
    {
        // !!! FIXME: clear to something other than the background color?
        // delete in-progress backingDC to force flush of rendered data...
        const bool hasBackingDC = (this->backingDC != NULL);
        this->nukeDC(&this->backingDC);
        dc.DrawBitmap(*this->backing, xoff, yoff, false);
        TOBY_renderAllTurtles(&dc);
        if (hasBackingDC)
            this->constructBackingDC();

        // If there's some space in the window that isn't covered by the
        //  bitmap, blank it out. We do a lot of tapdancing to try and clip
        //  out exactly what will need clearing.
        if (yoff > 0)
        {
            const wxRect top(0, 0, this->clientW, yoff);
            dc.DestroyClippingRegion();
            dc.SetClippingRegion(top);
            dc.Clear();

            const wxRect bottom(0, this->backingH+yoff, this->clientW, yoff+1);
            dc.DestroyClippingRegion();
            dc.SetClippingRegion(bottom);
            dc.Clear();
        } // if

        if (xoff > 0)
        {
            const wxRect left(0, 0, xoff, this->clientH);
            dc.DestroyClippingRegion();
            dc.SetClippingRegion(left);
            dc.Clear();

            const wxRect right(xoff+this->backingW, 0, xoff+1, this->clientH);
            dc.DestroyClippingRegion();
            dc.SetClippingRegion(right);
            dc.Clear();
        } // if
    } // else 
} // TurtleSpace::onPaint


bool TobyPrintout::OnPrintPage(int page)
{
    const TobyFrame *tframe = wxGetApp().getTobyFrame();
    wxASSERT(!TOBY_isRunning());

    wxBitmap *bmp = tframe->getTurtleSpace()->getBacking();
    if (bmp != NULL)
    {
        wxDC *dc = this->GetDC();
        if (dc != NULL)
        {
            wxCoord dcw, dch;
            dc->GetSize(&dcw, &dch);

            int w = dcw;
            int h = dch;
            if (w > h) w = h; else h = w;  // keep it square.
            wxImage img(bmp->ConvertToImage());
            img.Rescale(w, h, wxIMAGE_QUALITY_HIGH);
            dc->DrawBitmap(wxBitmap(img), (dcw-w)/2, (dch-h)/2, false);
            return true;
        } // if
    } // if

    return false;
} // TobyPrintout::OnPrintPage


void TobyPrintout::GetPageInfo(int *minPage, int *maxPage,
                               int *selPageFrom, int *selPageTo)
{
    *minPage = *maxPage = *selPageFrom = *selPageTo = 1;
} // TobyPrintout::GetPageInfo


bool TobyPrintout::HasPage(int pageNum)
{
    return (pageNum == 1);
} // TobyPrint/out::HasPage



TobyFrame::TobyFrame()
    : wxFrame(NULL, -1, wxT("Toby"), getPreviousPos(), getPreviousSize())
    , turtleSpace(new TurtleSpace(this))
    , modified(false)
    , nonMaximizedX(0)
    , nonMaximizedY(0)
    , nonMaximizedWidth(0)
    , nonMaximizedHeight(0)
    , quitting(false)
    , runForPrinting(false)
    , breakAtStart(false)
    , execProgram(NULL)
    , fileMenu(new wxMenu)
    , runMenu(new wxMenu)
    , helpMenu(new wxMenu)
    , menuBar(new wxMenuBar)
{
    this->GetPosition(&this->nonMaximizedX, &this->nonMaximizedY);
    this->GetSize(&this->nonMaximizedWidth, &this->nonMaximizedHeight);
    // ...don't forget to resize turtleSpace somewhere in your subclass!

    // !!! FIXME: overflowing 80 chars here...

    // Set up common menu items. Subclasses will add more menus and menu items.
    this->fileMenu->Append(MENUCMD_Open, wxT("&Open\tCtrl-O"));
    this->fileMenu->Append(MENUCMD_SaveAsImage, wxT("Save As &Image"))->Enable(false);
    this->fileMenu->Append(MENUCMD_PageSetup, wxT("Pa&ge Setup"));
    this->fileMenu->Append(MENUCMD_PrintPreview, wxT("P&rint Preview"))->Enable(false);
    this->fileMenu->Append(MENUCMD_Print, wxT("&Print\tCtrl-P"))->Enable(false);
    this->fileMenu->AppendSeparator();
    this->fileMenu->Append(MENUCMD_Quit, wxT("E&xit\tCtrl-X"));

    this->runMenu->Append(MENUCMD_Run, wxT("&Run Program\tF5"))->Enable(false);
    this->runMenu->Append(MENUCMD_Stop, wxT("&Stop Program\tF6"))->Enable(false);
    this->runMenu->Append(MENUCMD_RunForPrinting, wxT("R&un Program for Printing"))->Enable(false);
    this->runMenu->Append(MENUCMD_Cleanup, wxT("&Clean up TurtleSpace"))->Enable(false);

    this->helpMenu->Append(MENUCMD_About, wxT("&About\tF1"));
    this->helpMenu->Append(MENUCMD_License, wxT("&License"));
    this->helpMenu->Append(MENUCMD_Website, wxT("Toby on the &Web"));

    this->menuBar->Append(this->fileMenu, wxT("&File"));
    this->menuBar->Append(this->runMenu, wxT("&Run"));
    this->menuBar->Append(this->helpMenu, wxT("&Help"));
    this->SetMenuBar(this->menuBar);

    long mx = 0;
    wxConfigBase *cfg = wxConfig::Get();
    if ((cfg != NULL) && (cfg->Read(wxT("Maximized"), &mx)) && (mx))
        this->Maximize();

    this->updateTitleBar();
} // TobyFrame::TobyFrame


TobyFrame::~TobyFrame()
{
    delete[] this->execProgram;

    // this->turtleSpace should be deleted by the subclass (because it usually
    //  wants to be killed by a wxSizer ...)

    // this->fileMenu, etc is deleted by wxFrame when it cleans up the menubar.
} // TobyFrame::~TobyFrame


const wxPoint TobyFrame::getPreviousPos()
{
    int dpyw, dpyh;
    ::wxDisplaySize(&dpyw, &dpyh);

    long winx, winy, winw, winh;
    wxConfigBase *cfg = wxConfig::Get();
    if ( (cfg == NULL) ||
         (!cfg->Read(wxT("WindowX"), &winx)) ||
         (!cfg->Read(wxT("WindowY"), &winy)) ||
         (!cfg->Read(wxT("WindowW"), &winw)) ||
         (!cfg->Read(wxT("WindowH"), &winh)) )
        return wxPoint(dpyw / 8, dpyh / 8);

    if (winw > dpyw) { winw = dpyw; winx = 0; }
    else if (winw < 50) winw = 50;
    if (winh > dpyh) { winh = dpyh; winy = 0; }
    else if (winh < 50) winh = 50;
    if (winx+winw < 10) winx = 0;
    else if (winx > dpyw-10) winx = dpyw-10;
    if (winy < 0) winy = 0;
    else if (winy > dpyh-10) winy = dpyh-10;
    return wxPoint(winx, winy);
} // TobyFrame::getPreviousPos


const wxSize TobyFrame::getPreviousSize()
{
    int dpyw, dpyh;
    ::wxDisplaySize(&dpyw, &dpyh);

    long winw, winh;
    wxConfigBase *cfg = wxConfig::Get();
    if ( (cfg == NULL) ||
         (!cfg->Read(wxT("WindowW"), &winw)) ||
         (!cfg->Read(wxT("WindowH"), &winh)) )
        return wxSize(dpyw - (dpyw / 4), dpyh - (dpyh / 4));

    if (winw > dpyw) winw = dpyw;
    else if (winw < 50) winw = 50;
    if (winh > dpyh) winh = dpyh;
    else if (winh < 50) winh = 50;
    return wxSize(winw, winh);
} // TobyFrame::getPreviousSize


void TobyFrame::pumpEvents()
{
    while (wxGetApp().Pending())
        wxGetApp().Dispatch();
} // TobyFrame::pumpEvents


void TobyFrame::repaintTurtlespace()
{
    TurtleSpace *tspace = this->turtleSpace;
    tspace->Refresh(false);
    tspace->Update();  // force repaint.
} // TobyFrame::repaintTurtlespace


void TobyFrame::toggleWidgetsRunnable(bool readyToRun)
{
    wxMenuBar *mb = this->menuBar;
    mb->FindItem(MENUCMD_Run)->Enable(readyToRun);
    mb->FindItem(MENUCMD_Stop)->Enable(!readyToRun);
    mb->FindItem(MENUCMD_RunForPrinting)->Enable(readyToRun);
    mb->FindItem(MENUCMD_SaveAsImage)->Enable(readyToRun);
    mb->FindItem(MENUCMD_Print)->Enable(readyToRun);
    mb->FindItem(MENUCMD_PrintPreview)->Enable(readyToRun);
    mb->FindItem(MENUCMD_Cleanup)->Enable(readyToRun);
    this->toggleWidgetsRunnableImpl(readyToRun);
} // TobyFrame::toggleWidgetsRunnable


void TobyFrame::startRun()
{
    wxASSERT(!TOBY_isRunning());
    this->toggleWidgetsRunnable(false);
    this->turtleSpace->startRun(this->runForPrinting);
    if (this->breakAtStart)
        TOBY_stepProgram();
    this->SetStatusText(wxT("Now running..."));
    this->profileStopwatch.Start(0);
} // TobyFrame::startRun


void TobyFrame::stopRun()
{
    wxASSERT(TOBY_isRunning());
    const long ms = this->profileStopwatch.Time();
    this->SetStatusText(wxString::Format(wxT("...ran %ld milliseconds."), ms));
    this->toggleWidgetsRunnable(true);
    this->turtleSpace->stopRun();
} // TobyFrame::stopRun


void TobyFrame::requestQuit()
{
    TOBY_haltProgram();
    this->quitting = true;
} // TobyFrame::requestQuit


void TobyFrame::runProgram(bool _runForPrinting, bool _breakAtStart)
{
    TOBY_haltProgram();  // stop the current run as soon as possible.
    // This gets kicked off in the next idle event.
    this->runForPrinting = _runForPrinting;
    delete[] this->execProgram;
    this->execProgram = this->getProgramImpl();
    this->breakAtStart = _breakAtStart;
} // TobyFrame::runProgram


void TobyFrame::onIdle(wxIdleEvent &evt)
{
    if (this->isQuitting())
    {
        if (TOBY_isRunning())
            TOBY_haltProgram();
        else
        {
            this->quitting = false;
            this->Close(false);
        } // else
    } // if

    else if (this->execProgram != NULL)
    {
        if (TOBY_isRunning())
            TOBY_haltProgram();
        else
        {
            char *prog = this->execProgram;
            this->execProgram = NULL;
            TOBY_runProgram(prog, this->runForPrinting);
            delete[] prog;
        } // else
    } // else if
} // TobyFrame::onIdle


void TobyFrame::updateTitleBar()
{
    wxString fnstr;
    if (!this->filename.IsEmpty())
        fnstr = wxFileName(this->filename).GetFullName();
    else
        fnstr = wxT("New file");
    wxString modstr(this->modified ? wxT("*") : wxT(""));
    this->SetTitle(wxString::Format(wxT("Toby - %s%s"),
                    fnstr.c_str(), modstr.c_str()));
} // TobyFrame::updateTitleBar


void TobyFrame::openFile(const wxString &path)
{
    wxFileInputStream strm(path);
    if (strm.IsOk())
    {
        size_t len = strm.GetLength();
        char *buf = new char[len + 1];
        if (!strm.Read(buf, len).IsOk())
        {
            TOBY_messageBox("Could not read file");
            delete[] buf;
        } // if
        else
        {
            buf[len] = '\0';
            this->filename = path;
            this->modified = false;
            this->updateTitleBar();
            this->openFileImpl(buf);  // openFileImpl will delete[]!
        } // else
    } // else
} // TobyFrame::openFile


void TobyFrame::onMenuOpen(wxCommandEvent& evt)
{
    wxASSERT(!TOBY_isRunning());
    TOBY_haltProgram();  // just in case.

    // !!! FIXME: localization.
    wxFileDialog dlg(this, wxT("Choose a file to open"), wxT(""), wxT(""),
                     wxT("Toby Programs (*.toby)|*.toby"),
                     wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (dlg.ShowModal() == wxID_OK)
        openFile(dlg.GetPath());
} // TobyFrame::onMenuOpen


void TobyFrame::onMenuSaveAsImage(wxCommandEvent &evt)
{
    wxString exts;
    wxList &handlers = wxImage::GetHandlers();
    for (wxNode *node = handlers.GetFirst(); node; node = node->GetNext())
    {
        wxImageHandler *handler = (wxImageHandler *) node->GetData();
        exts.Append(handler->GetName());
        exts.Append(wxT(" (*."));
        exts.Append(handler->GetExtension());
        exts.Append(wxT(")|*."));
        exts.Append(handler->GetExtension());
        if (node->GetNext() != NULL)
            exts.Append(wxT("|"));
    } // for

    // !!! FIXME: localization.
    wxFileDialog dlg(this, wxT("Choose a file to save"), wxT(""), wxT(""),
                     exts, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (dlg.ShowModal() == wxID_OK)
    {
        wxImage img(this->turtleSpace->getBacking()->ConvertToImage());
        // Force alpha to opaque so background shows up.
        unsigned char *alpha = img.GetAlpha();
        if (alpha != NULL)
            memset(alpha, 0xFF, img.GetWidth() * img.GetHeight());
        wxString fname(dlg.GetPath());
        if (fname.Find('.') == -1)
        {
            const int idx = dlg.GetFilterIndex();
            const wxNode *node = handlers.Item(idx);
            const wxImageHandler *handler = (wxImageHandler *) node->GetData();
            const wxString ext(handler->GetExtension());
            fname.Append(wxT("."));
            fname.Append(ext);
        } // if
        img.SaveFile(fname);  // if problem, does error message on destruction.
    } // if
} // TobyFrame::onMenuSaveAsImage


void TobyFrame::onMenuPageSetup(wxCommandEvent &event)
{
    wxPrintData *printData = wxGetApp().getPrintData();
    wxPageSetupDialogData pageSetupData(*printData);
    wxPageSetupDialog pageSetupDialog(this, &pageSetupData);
    pageSetupDialog.ShowModal();
    *printData = pageSetupDialog.GetPageSetupDialogData().GetPrintData();
} // TobyFrame::onMenuPageSetup


void TobyFrame::onMenuPrintPreview(wxCommandEvent &event)
{
    wxPrintData *printData = wxGetApp().getPrintData();
    wxPrintDialogData printDialogData(*printData);
    wxPrintPreview *preview = new wxPrintPreview(new TobyPrintout,
                                                 new TobyPrintout,
                                                 &printDialogData);

    if (!preview->Ok())
    {
        delete preview;
        ::wxMessageBox(wxT("There was a problem previewing.\nPerhaps your current printer is not set correctly?"),
                       wxT("Previewing"), wxOK);
        return;
    } // if

    wxPreviewFrame *frame = new wxPreviewFrame(preview, this,
                                               wxT("Toby Preview"),
                                               wxPoint(100, 100),
                                               wxSize(600, 650));

    frame->Centre(wxBOTH);
    frame->Initialize();
    frame->Show();
} // TobyFrame::onMenuPrintPreview


void TobyFrame::onMenuPrint(wxCommandEvent &event)
{
    wxPrintData *printData = wxGetApp().getPrintData();
    wxPrintDialogData printDialogData(*printData);
    wxPrinter printer(&printDialogData);
    TobyPrintout printout(wxT("Toby"));
    if (!printer.Print(this, &printout))
    {
        if (wxPrinter::GetLastError() == wxPRINTER_ERROR)
            wxMessageBox(wxT("There was a problem printing.\nPerhaps your current printer is not set correctly?"), wxT("Printing"), wxOK);
    } // if
} // TobyFrame::onMenuPrint


void TobyFrame::onMenuRun(wxCommandEvent &evt)
{
    if (!TOBY_isRunning())
        this->runProgram(false);  // Run will kick off in next idle event.
    else if (TOBY_isPaused())
    {
        this->menuBar->FindItem(MENUCMD_Run)->Enable(false);
        this->onContinueImpl();
        TOBY_continueProgram();
    } // else if
} // TobyFrame::onMenuRun


void TobyFrame::onMenuStop(wxCommandEvent &evt)
{
    TOBY_haltProgram();
} // TobyFrame::onMenuStop


void TobyFrame::onMenuStep(wxCommandEvent &evt)
{
    if (TOBY_isRunning())
        TOBY_stepProgram();
    else
    {
        // if it's not running, start it and break at startup.
        this->runProgram(false, true);  // Run will kick off in next idle event.
    } // else
} // TobyFrame::onMenuStep


void TobyFrame::onMenuRunForPrinting(wxCommandEvent &evt)
{
    // Run will kick off in next idle event.
    wxASSERT(!TOBY_isRunning());
    this->runProgram(true);
} // TobyFrame::onMenuRunForPrinting


void TobyFrame::onMenuCleanup(wxCommandEvent &evt)
{
    this->turtleSpace->cleanup(0, 0, 0, true);
    this->repaintTurtlespace();
} // TobyFrame::onMenuCleanup


void TobyFrame::onMenuAbout(wxCommandEvent &evt)
{
    #define TOBY_VERSION_STRING2(x) wxT(#x)
    #define TOBY_VERSION_STRING(x) TOBY_VERSION_STRING2(x)
    wxAboutDialogInfo info;
    info.SetName(wxT("Toby"));
    info.SetVersion(TOBY_VERSION_STRING(APPREV));
    info.SetDescription(wxT("A programming language for learning."));
    info.SetCopyright(wxT("(C) 2007 Ryan C. Gordon <icculus@icculus.org>"));
    ::wxAboutBox(info);
    #undef TOBY_VERSION_STRING2
    #undef TOBY_VERSION_STRING
} // TobyFrame::onMenuAbout


TobyLicenseDialog::TobyLicenseDialog(wxWindow *parent)
    : wxDialog(parent, -1, wxT("License"), wxDefaultPosition, wxDefaultSize, dlgFlags)
{
    ::wxBeginBusyCursor();

    wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

    wxSizerFlags flagsBorder2;
    flagsBorder2.DoubleBorder();

    int dpyw, dpyh;
    ::wxDisplaySize(&dpyw, &dpyh);

    wxTextCtrl *textCtrl = new wxTextCtrl(this, wxID_ANY,
                                    wxString(GLicense, wxConvUTF8),
                                    wxDefaultPosition, wxSize(dpyw/2, dpyh/2),
                                    wxTE_READONLY | wxTE_MULTILINE);

    topsizer->Add(textCtrl,
                  wxSizerFlags(1).Expand().TripleBorder(wxLEFT | wxRIGHT));

    wxSizer *buttonSizer = CreateSeparatedButtonSizer(wxOK);
    if (buttonSizer != NULL)
        topsizer->Add(buttonSizer, wxSizerFlags(flagsBorder2).Expand());

    this->SetAutoLayout(true);
    this->SetSizer(topsizer);

    topsizer->SetSizeHints(this);
    topsizer->Fit(this);

    this->Centre(wxBOTH);
    textCtrl->SetFocus();

    ::wxEndBusyCursor();
} // TobyLicenseDialog::TobyLicenseDialog


void TobyLicenseDialog::onOK(wxCommandEvent &evt)
{
    this->EndModal(wxID_OK);
} // TobyLicenseDialog::onOK


void TobyFrame::onMenuLicense(wxCommandEvent &evt)
{
    TobyLicenseDialog dlg(this);
    dlg.ShowModal();
} // TobyFrame::onMenuLicense


void TobyFrame::onMenuWebsite(wxCommandEvent &evt)
{
    wxLaunchDefaultBrowser(wxT("http://icculus.org/toby/"));
} // TobyFrame::onMenuWebsite


void TobyFrame::onMenuQuit(wxCommandEvent& evt)
{
    Close(true);
} // TobyFrame::onMenuQuit


void TobyFrame::onResize(wxSizeEvent &evt)
{
    this->Layout();  // Have sizer resize child windows...
    if (!this->IsMaximized())
        this->GetSize(&this->nonMaximizedWidth, &this->nonMaximizedHeight);
} // TobyFrame::onResize


void TobyFrame::onMove(wxMoveEvent &evt)
{
    if (!this->IsMaximized())
        this->GetPosition(&this->nonMaximizedX, &this->nonMaximizedY);
} // TobyFrame::onMove


void TobyFrame::onClose(wxCloseEvent &evt)
{
    if (TOBY_isRunning())
    {
        this->requestQuit();  // try it again later so program can halt...
        evt.Veto();  // ...this time, though, no deal.
    } // else if

    else if (this->shouldVetoClose())
    {
        evt.Veto();  // subclass says no.
    } // else if

    else  // really closing this time.
    {
        this->closeImpl();

        wxConfigBase *cfg = wxConfig::Get();
        if (cfg != NULL)
        {
            cfg->Write(wxT("WindowW"), (long) this->nonMaximizedWidth);
            cfg->Write(wxT("WindowH"), (long) this->nonMaximizedHeight);
            cfg->Write(wxT("WindowX"), (long) this->nonMaximizedX);
            cfg->Write(wxT("WindowY"), (long) this->nonMaximizedY);
            cfg->Write(wxT("Maximized"), (long) this->IsMaximized() ? 1 : 0);
        } // if
        this->Destroy();
    } // else
} // TobyFrame::onClose


static inline char *newstr(const char *str)
{
    char *retval = NULL;
    if (str != NULL)
    {
        retval = new char[strlen(str) + 1];
        strcpy(retval, str);
    } // if
    return retval;
} // newstr


#if TOBY_WX_BUILD_STANDALONE

TobyStandaloneFrame::TobyStandaloneFrame()
    : sourceCode(NULL)
{
    this->turtleSpace->SetSize(this->GetClientSize());
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(this->turtleSpace, 1, wxALL | wxEXPAND | wxALIGN_CENTRE);
    // let TurtleSpace shrink as far as window will.
    sizer->SetItemMinSize(this->turtleSpace, 1, 1);
    this->SetSizer(sizer);
} // TobyStandaloneFrame::TobyStandaloneFrame


char *TobyStandaloneFrame::getProgramImpl()
{
    return newstr(this->sourceCode);
} // TobyStandaloneFrame::getProgramImpl


void TobyStandaloneFrame::openFileImpl(char *prog)
{
    // Run will kick off in next idle event.
    delete[] this->sourceCode;
    this->sourceCode = prog;  // we have to delete[] this later!
    this->runProgram(false);  // just run right away.
} // TobyStandaloneFrame::openFileImpl

#endif  // TOBY_WX_BUILD_STANDALONE


#if TOBY_WX_BUILD_IDE

TobyIDEFrame::TobyIDEFrame()
    : topSplit(NULL)
    , ideSplit(NULL)
    , dbgSplit(NULL)
    , textCtrl(NULL)
    , callstackCtrl(NULL)
    , variablesCtrl(NULL)
{
    this->CreateStatusBar();
    this->SetStatusText(wxString(GBuildVer, wxConvUTF8));

    // !!! FIXME: localization.
    this->fileMenu->Append(MENUCMD_Save, wxT("&Save\tCtrl-S"));
    this->fileMenu->Append(MENUCMD_SaveAs, wxT("Save &As"));
    this->runMenu->Append(MENUCMD_Step, wxT("&Step Program\tF8"));

    wxSize clientSize(this->GetClientSize());
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    this->topSplit = new wxSplitterWindow(this, -1,
                                             wxDefaultPosition, clientSize,
                                             wxSP_3D | wxSP_LIVE_UPDATE);
    this->topSplit->SetSashGravity(0.5);
    this->topSplit->SetMinimumPaneSize(1);

    this->ideSplit = new wxSplitterWindow(this->topSplit, -1,
                                             wxDefaultPosition, wxDefaultSize,
                                             wxSP_3D | wxSP_LIVE_UPDATE);
    this->ideSplit->SetSashGravity(0.5);
    this->ideSplit->SetMinimumPaneSize(1);

    this->textCtrl = new wxTextCtrl(this->ideSplit, wxID_ANY, wxT(""),
                                    wxDefaultPosition, wxDefaultSize,
                                        wxTE_MULTILINE |
                                        wxTE_PROCESS_TAB |
                                        wxTE_DONTWRAP);

    this->dbgSplit = new wxSplitterWindow(this->ideSplit, -1,
                                        wxDefaultPosition, wxDefaultSize,
                                        wxSP_3D | wxSP_LIVE_UPDATE);
    this->dbgSplit->SetSashGravity(0.5);
    this->dbgSplit->SetMinimumPaneSize(1);

    this->callstackCtrl = new wxListBox(this->dbgSplit, -1);
    this->variablesCtrl = new wxListBox(this->dbgSplit, -1);

    this->turtleSpace->Reparent(this->topSplit);

    long pos = 0;
    wxConfigBase *cfg = wxConfig::Get();

    this->dbgSplit->SplitVertically(this->callstackCtrl, this->variablesCtrl);
    if ( (cfg != NULL) && (cfg->Read(wxT("IDEDbgSplitPos"), &pos)) )
        this->dbgSplit->SetSashPosition((int) pos);

    this->ideSplit->SplitHorizontally(this->textCtrl, this->dbgSplit);
    if ( (cfg != NULL) && (cfg->Read(wxT("IDEIdeSplitPos"), &pos)) )
        this->ideSplit->SetSashPosition((int) pos);

    this->topSplit->SplitVertically(this->ideSplit, this->turtleSpace);
    if ( (cfg != NULL) && (cfg->Read(wxT("IDETopSplitPos"), &pos)) )
        this->topSplit->SetSashPosition((int) pos);

    // The sizer just makes sure that topSplit owns whole client area.
    sizer->Add(this->topSplit, 1, wxALL | wxEXPAND | wxALIGN_CENTRE);
    sizer->SetItemMinSize(this->topSplit, 1, 1);
    this->SetSizer(sizer);

    this->textCtrl->SetFocus();
} // TobyIDEFrame::TobyIDEFrame


void TobyIDEFrame::toggleWidgetsRunnableImpl(bool readyToRun)
{
    this->callstackCtrl->Clear();
    this->variablesCtrl->Clear();
    // !!! FIXME: toggle other IDE-specific bits.
} // TobyIDEFrame::toggleWidgetsRunnableImpl


void TobyIDEFrame::closeImpl()
{
    wxConfigBase *cfg = wxConfig::Get();
    if (cfg != NULL)
    {
        cfg->Write(wxT("IDETopSplitPos"), (long) topSplit->GetSashPosition());
        cfg->Write(wxT("IDEIdeSplitPos"), (long) ideSplit->GetSashPosition());
        cfg->Write(wxT("IDEDbgSplitPos"), (long) dbgSplit->GetSashPosition()+84); // !!! FIXME: wx bug?
    } // if
} // TobyIDEFrame::closeImpl


bool TobyIDEFrame::shouldVetoClose()
{
    if (this->modified)
    {
        wxMessageDialog dlg(this, wxT("Program has been modified! Save?"),
                wxT("Toby"), wxYES_NO|wxCANCEL|wxICON_EXCLAMATION);

        int rc = dlg.ShowModal();
        if (rc == wxID_NO)
            return false;
        else if (rc == wxID_YES)
        {
            wxCommandEvent evt;
            this->onMenuSave(evt);
        } // else if
    } // if

    return this->modified;
} // TobyIDEFrame::shouldVetoClose


void TobyIDEFrame::onContinueImpl()
{
    this->callstackCtrl->Clear();
    this->variablesCtrl->Clear();
} // TobyIDEFrame::onContinueImpl


void TobyIDEFrame::pauseReached(int line, int fullstop,
                                int breakpoint, int pauseTicks)
{
    if ((fullstop) || (pauseTicks >= 300))
    {
        this->menuBar->FindItem(MENUCMD_Run)->Enable(true);

        wxString status;
        if (breakpoint == -1)
            status = wxString::Format(wxT("Now on line #%d"), line);
        else
        {
            status = wxString::Format(wxT("Hit breakpoint #%d on line #%d.\n"),
                                        breakpoint, line);
        } // else
        this->SetStatusText(status);

        int frames = 0;
        const TobyDebugInfo *cs = TOBY_getCallstack(&frames);

        // if you're on the (global variables) selection, stick there,
        //  otherwise always view the top stackframe by default.
        int viewing = this->callstackCtrl->GetSelection();
        if (viewing == wxNOT_FOUND)
            viewing = 0;
        else if (((unsigned int) viewing) == this->callstackCtrl->GetCount()-1)
            viewing = frames;
        else
            viewing = 0;

        if ((frames <= 0) || (cs == NULL))
            this->callstackCtrl->Clear();
        else
        {
            wxString *items = new wxString[frames];
            for (int i = 0; i < frames; i++, cs++)
            {
                items[i] = wxString(cs->name, wxConvUTF8);
                items[i] << ((wxChar) ':') << cs->linenum;
            } // for

            this->callstackCtrl->Set(frames, items);
            delete[] items;
        } // else

        this->callstackCtrl->Append(wxT("(global variables)"));
        this->callstackCtrl->Select(viewing);
        this->updateVariablesCtrl(viewing);
    } // if
} // TobyIDEFrame::pauseReached


char *TobyIDEFrame::getProgramImpl()
{
    const wxString wxstr(textCtrl->GetValue());
    const wxChar *wxtext = wxstr.c_str();
    #if wxUSE_UNICODE
    size_t len = wxstr.Len() + 1;
    char *utf8text = new char[len * 6];
    wxConvUTF8.WC2MB(utf8text, wxtext, len);
    return utf8text;
    #else
    return newstr(wxtext);
    #endif
} // TobyIDEFrame::getProgramImpl


void TobyIDEFrame::openFileImpl(char *prog)
{
    this->textCtrl->ChangeValue(wxString(prog, wxConvUTF8));
    delete[] prog;  // don't need this anymore.
    this->toggleWidgetsRunnable(true);
    this->textCtrl->SetFocus();
    TOBY_clearAllBreakpoints();
} // TobyIDEFrame::openFileImpl


void TobyIDEFrame::updateVariablesCtrl(int frame)
{
    if (frame >= 0)
    {
        if ( ((unsigned int) frame) == (this->callstackCtrl->GetCount()-1) )
            frame = -1;  // Requesting global variables, not a stack frame.
    } // if

    int varCount = 0;
    const TobyDebugInfo *vars = TOBY_getVariables(frame, &varCount);
    if ((varCount <= 0) || (vars == NULL))
        this->variablesCtrl->Clear();
    else
    {
        wxString *items = new wxString[varCount];
        for (int i = 0; i < varCount; i++, vars++)
        {
            items[i] = wxString(vars->name, wxConvUTF8);
            items[i] << ((wxChar) '=') << wxString(vars->value, wxConvUTF8);
        } // for
        this->variablesCtrl->Set(varCount, items);
        delete[] items;
    } // else
} // TobyIDEFrame::updateVariablesCtrl


void TobyIDEFrame::onCallstackSelected(wxCommandEvent &evt)
{
    int frame = this->callstackCtrl->GetSelection();
    if (frame == wxNOT_FOUND)
        return;
    this->updateVariablesCtrl(frame);
} // TobyIDEFrame::onCallstackSelected


void TobyIDEFrame::onMenuOpen(wxCommandEvent &evt)
{
    // if source is modified, prompt to throwaway/save...
    if (!this->shouldVetoClose())
        TobyFrame::onMenuOpen(evt);  // go through parent's open handler.
} // TobyIDEFrame::onMenuOpen


void TobyIDEFrame::onMenuSave(wxCommandEvent &evt)
{
    if (this->filename.IsEmpty())
        this->onMenuSaveAs(evt);  // get a filename and recall this function.
    else
    {
        char *buf = this->getProgramImpl();
        wxFileOutputStream strm(this->filename);
        if (strm.IsOk())
        {
            if (!strm.Write(buf, strlen(buf)).IsOk())
                TOBY_messageBox("Could not write file");
            else
            {
                this->modified = false;
                this->updateTitleBar();
            } // else
        } // if

        delete[] buf;
    } // else
} // TobyIDEFrame::onMenuSave


void TobyIDEFrame::onMenuSaveAs(wxCommandEvent &evt)
{
    wxFileDialog dlg(this, wxT("Choose a file to save"), wxT(""), wxT(""),
                     wxT("Toby Programs (*.toby)|*.toby"),
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (dlg.ShowModal() == wxID_OK)
    {
        wxString fname(dlg.GetPath());
        if (fname.Find('.') == -1)
            fname.Append(wxT(".toby"));
        this->filename = fname;
        this->onMenuSave(evt);
    } // if
} // TobyIDEFrame::onMenuSaveAs


void TobyIDEFrame::onTextModified(wxCommandEvent &evt)
{
    if (!this->modified)
    {
        this->modified = true;
        this->updateTitleBar();
    } // if
} // TobyIDEFrame::onTextModified

#endif  // TOBY_WX_BUILD_IDE


IMPLEMENT_APP(TobyWxApp)

wxPrintData *TobyWxApp::getPrintData()
{
    if (this->printData == NULL)
        this->printData = new wxPrintData;
    return this->printData;
} // TobyWxApp::getPrintData


// Like ::wxInitAllImageHandlers(), but we don't want ALL of them...
static void tobyInitAllImageHandlers()
{
    wxImage::AddHandler( new wxBMPHandler );

    #if wxUSE_LIBPNG
    wxImage::AddHandler( new wxPNGHandler );
    #endif
    #if wxUSE_LIBJPEG
    wxImage::AddHandler( new wxJPEGHandler );
    #endif
    #if wxUSE_LIBTIFF
    wxImage::AddHandler( new wxTIFFHandler );
    #endif
    #if wxUSE_TGA
    wxImage::AddHandler( new wxTGAHandler );
    #endif
} // tobyInitAllImageHandlers


bool TobyWxApp::OnInit()
{
    bool standalone = TOBY_WX_STANDALONE_DEFAULT;
    const wxChar *filenameArgument = NULL;

    // See if there are any interesting command line options to get started.
    for (int i = 1; i < this->argc; i++)
    {
        const wxChar *arg = this->argv[i];
        if ((*arg != '-') && (filenameArgument == NULL))
            filenameArgument = arg;
        else
        {
            while (*(++arg) == '-') { /* no-op. */ }
            if (wxString(arg) == wxT("standalone"))
                standalone = true;
            else if (wxString(arg) == wxT("ide"))
                standalone = false;
            else if (wxString(arg) == wxT("buildver"))
            {
                printf("%s\n", GBuildVer);
                return false;
            } // else if
            else if (wxString(arg) == wxT("license"))
            {
                printf("%s\n", GLicense);
                return false;
            } // else if
        } // else
    } // for

    #if PLATFORM_MACOSX
    // This lets a stdio app become a GUI app. Otherwise, you won't get
    //  GUI events from the system and other things will fail to work.
    // Putting the app in an application bundle does the same thing.
    //  TransformProcessType() is a 10.3+ API. SetFrontProcess() is 10.0+.
    if (TransformProcessType != NULL)  // check it as a weak symbol.
    {
        ProcessSerialNumber psn = { 0, kCurrentProcess };
        TransformProcessType(&psn, kProcessTransformToForegroundApplication);
        SetFrontProcess(&psn);
    } // if
    #endif

    wxConfigBase *cfg = new wxConfig(wxT("Toby"), wxT("icculus.org"));
    if (cfg != NULL) wxConfig::Set(cfg);

    this->processStopwatch.Start(0);

    tobyInitAllImageHandlers();

    if (standalone)
    {
        #if TOBY_WX_BUILD_STANDALONE
        if (cfg != NULL) cfg->SetPath(wxT("/Standalone"));
        this->mainWindow = new TobyStandaloneFrame;
        #else
        fprintf(stderr, "ERROR: No standalone app support in this build.\n");
        return false;
        #endif
    } // if
    else
    {
        #if TOBY_WX_BUILD_IDE
        if (cfg != NULL) cfg->SetPath(wxT("/IDE"));
        this->mainWindow = new TobyIDEFrame;
        #else
        fprintf(stderr, "ERROR: No IDE app support in this build.\n");
        return false;
        #endif
    } // else

    this->mainWindow->Show(true);
    SetTopWindow(this->mainWindow);

    if (filenameArgument != NULL)
        this->mainWindow->openFile(wxString(filenameArgument));

    return true;
} // TobyWxApp::OnInit


int TobyWxApp::OnExit()
{
    mainWindow = NULL;  // this is probably deleted already.
    delete this->printData;
    this->printData = NULL;
    return wxApp::OnExit();
} // TobyWxApp::OnExit

// end of toby_wxwidgets.cpp ...

