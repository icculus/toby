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
    inline void putToScreen();

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
    MENUCMD_PageSetup = wxID_PAGE_SETUP,
    MENUCMD_PrintPreview = wxID_PRINT_SETUP,
    MENUCMD_Print = wxID_PRINT,
    MENUCMD_RunOrStop = wxID_HIGHEST,

    // non-standard menu items go here.
    MENUCMD_RunForPrinting,
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
private:
    enum ExecState
    {
        EXEC_STOPPED=0,
        EXEC_STOPPING,
        EXEC_RUNNING,
        EXEC_STEPPING,
    };

public:
    TobyFrame();
    virtual ~TobyFrame();
    TurtleSpace *getTurtleSpace() const { return this->turtleSpace; }
    static const wxPoint getPreviousPos();
    static const wxSize getPreviousSize();
    void openFile(const wxString &path);
    void startRun();
    void stopRun();
    bool pumpEvents(int hook=TOBY_HOOKDELAY, int currentline=-1);
    inline void requestQuit();
    inline void runProgram(bool printing);
    inline void haltProgram();
    inline bool isStepping() const { return this->execState == EXEC_STEPPING; }
    inline bool isQuitting() const { return this->quitting; }

    inline bool isRunning() const
    {
        return ( (this->execState == EXEC_RUNNING) ||
                 (this->execState == EXEC_STEPPING) ||
                 (this->execState == EXEC_STOPPING) );
    } // isRunning

    inline bool isStopping() const
    {
        return ( (this->execState == EXEC_STOPPING) ||
                 (this->execState == EXEC_STOPPED) ||
                 (this->quitting) );
    } // isStopping

    // subclasses fill in these.
    virtual void openFileImpl(const wxString &fname, char *buf) = 0;
    virtual void startRunImpl() = 0;
    virtual void stopRunImpl() = 0;
    virtual char *getProgramImpl() = 0;
    virtual bool shouldVetoClose() = 0;

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
    void onMenuRunOrStop(wxCommandEvent &evt);
    void onMenuStep(wxCommandEvent &evt);
    void onMenuRunForPrinting(wxCommandEvent &evt);

protected:
    TurtleSpace *turtleSpace;
    int nonMaximizedX;
    int nonMaximizedY;
    int nonMaximizedWidth;
    int nonMaximizedHeight;
    ExecState execState;
    bool quitting;
    bool runForPrinting;
    char *execProgram;
    wxMenu *fileMenu;
    wxMenu *runMenu;
    wxMenu *helpMenu;
    wxMenuBar *menuBar;

private:
    wxStopWatch pumpStopwatch;
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
    EVT_MENU(MENUCMD_RunOrStop, TobyFrame::onMenuRunOrStop)
    EVT_MENU(MENUCMD_Step, TobyFrame::onMenuStep)
    EVT_MENU(MENUCMD_RunForPrinting, TobyFrame::onMenuRunForPrinting)
END_EVENT_TABLE()


#if TOBY_WX_BUILD_STANDALONE
// This is a TobyFrame that only provides a TurtleSpace, basic menu items,
//  and no other UI.
class TobyStandaloneFrame : public TobyFrame
{
public:
    TobyStandaloneFrame();
    virtual ~TobyStandaloneFrame() { delete[] this->sourceCode; }
    virtual void openFileImpl(const wxString &fname, char *prog);
    virtual void startRunImpl() { /* no-op in this implementation. */ }
    virtual void stopRunImpl() { /* no-op in this implementation. */ }
    virtual char *getProgramImpl();
    virtual bool shouldVetoClose() { return false; }  // nothing to save, etc.

private:
    char *sourceCode;
};
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

private:
    TobyFrame *mainWindow;
    wxPrintData *printData;
};

DECLARE_APP(TobyWxApp)




// implementations of callbacks during execution of Toby programs...

int TOBY_delay(int ms)
{
    TobyFrame *tframe = wxGetApp().getTobyFrame();
    wxLongLong now(::wxGetLocalTimeMillis());
    const wxLongLong end = now + wxLongLong(ms);
    while (now < end)
    {
        if (!tframe->pumpEvents())
            return 0;
        now = ::wxGetLocalTimeMillis();
        if (now < end)
        {
            const unsigned long ticks = (unsigned long) ((end-now).ToLong());
            ::wxMilliSleep((ticks > 50) ? 50 : ticks);
            now = ::wxGetLocalTimeMillis();
        } // if
    } // while

    return tframe->pumpEvents();
} // TOBY_delay


#define TOBY_PROFILE 1
#if TOBY_PROFILE
static wxStopWatch *profileStopwatch = NULL;
#endif

void TOBY_startRun()
{
    wxGetApp().getTobyFrame()->startRun();

    #if TOBY_PROFILE
    profileStopwatch = new wxStopWatch;
    #endif
} // TOBY_startRun


void TOBY_stopRun()
{
    #if TOBY_PROFILE
    printf("time to execute: %ld\n", profileStopwatch->Time());
    delete profileStopwatch;
    profileStopwatch = NULL;
    #endif

    wxGetApp().getTobyFrame()->stopRun();
} // TOBY_stopRun


int TOBY_pumpEvents(int hook, int currentline)
{
    return wxGetApp().getTobyFrame()->pumpEvents(hook, currentline) ? 1 : 0;
} // TOBY_pumpEvents


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
    : wxWindow(parent, wxID_ANY)
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


void TurtleSpace::putToScreen()
{
    this->Refresh(false);
    this->Update();  // force repaint.
} // TurtleSpace::putToScreen


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
    this->putToScreen();
} // TurtleSpace::stopRun


void TurtleSpace::onResize(wxSizeEvent &evt)
{
    // Just cache the dimensions, since we're spending an enormous amount of
    //  time looking them up over and over during program execution.
// !!! FIXME: do this without the wxSize?
    wxSize size(this->GetClientSize());
    this->clientW = size.GetWidth();
    this->clientH = size.GetHeight();
    this->putToScreen();  // force a full redraw.
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
    wxASSERT(!tframe->isRunning());

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
    , nonMaximizedX(0)
    , nonMaximizedY(0)
    , nonMaximizedWidth(0)
    , nonMaximizedHeight(0)
    , execState(EXEC_STOPPED)
    , quitting(false)
    , runForPrinting(false)
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
    this->fileMenu->Append(MENUCMD_SaveAsImage, wxT("&Save As Image\tCtrl-S"))->Enable(false);
    this->fileMenu->Append(MENUCMD_PageSetup, wxT("Pa&ge Setup"));
    this->fileMenu->Append(MENUCMD_PrintPreview, wxT("P&rint Preview"))->Enable(false);
    this->fileMenu->Append(MENUCMD_Print, wxT("&Print\tCtrl-P"))->Enable(false);
    this->fileMenu->AppendSeparator();
    this->fileMenu->Append(MENUCMD_Quit, wxT("E&xit\tCtrl-X"));

    this->runMenu->Append(MENUCMD_RunOrStop, wxT("&Run Program\tF5"))->Enable(false);
    this->runMenu->Append(MENUCMD_Step, wxT("&Step\tF8"))->Enable(false);
    this->runMenu->Append(MENUCMD_RunForPrinting, wxT("R&un Program for Printing"))->Enable(false);
    this->runMenu->Append(MENUCMD_Cleanup, wxT("&Clean up TurtleSpace"))->Enable(false);

    this->helpMenu->Append(MENUCMD_About, wxT("&About\tF1"));
    this->helpMenu->Append(MENUCMD_License, wxT("&License"));
    this->helpMenu->Append(MENUCMD_Website, wxT("Toby on the &Web"));

    this->menuBar->Append(this->fileMenu, wxT("&File"));
    this->menuBar->Append(this->runMenu, wxT("&Run"));
    this->menuBar->Append(this->helpMenu, wxT("&Help"));
    this->SetMenuBar(this->menuBar);
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
    if ( (!cfg->Read(wxT("LastWindowX"), &winx, -1)) ||
         (!cfg->Read(wxT("LastWindowY"), &winy, -1)) ||
         (!cfg->Read(wxT("LastWindowW"), &winw, -1)) ||
         (!cfg->Read(wxT("LastWindowH"), &winh, -1)) )
        return wxDefaultPosition;

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
    if ( (!cfg->Read(wxT("LastWindowW"), &winw, -1)) ||
         (!cfg->Read(wxT("LastWindowH"), &winh, -1)) )
        return wxDefaultSize;

    if (winw > dpyw) winw = dpyw;
    else if (winw < 50) winw = 50;
    if (winh > dpyh) winh = dpyh;
    else if (winh < 50) winh = 50;
    return wxSize(winw, winh);
} // TobyFrame::getPreviousSize


bool TobyFrame::pumpEvents(int hook, int currentline)
{
#if 0
    if (this->executingLine != currentline)
    {
        this->executingLine = currentline;
        if (this->stepping)
        {
        } // if
    } // if
#endif

    if (this->pumpStopwatch.Time() > 50)
    {
        // force repaint here...this means we will be clamped to 20fps, but
        //  the overall execution of the program will be much faster, as
        //  rendering primitives will batch.
        this->turtleSpace->putToScreen();

        // Pump the system event queue if we aren't requesting a program halt.
        while ((!this->isStopping()) && (wxGetApp().Pending()))
            wxGetApp().Dispatch();

        this->pumpStopwatch.Start(0);  // reset this for next call.
    } /* if */

    return !this->isStopping();
} // TobyFrame::pumpEvents


void TobyFrame::startRun()
{
    wxASSERT(!this->isRunning());

    wxMenuBar *mb = this->menuBar;
    mb->FindItem(MENUCMD_RunOrStop)->SetText(wxT("&Stop Program\tF5"));
    mb->FindItem(MENUCMD_RunOrStop)->Enable(true);
    mb->FindItem(MENUCMD_RunForPrinting)->Enable(false);
    mb->FindItem(MENUCMD_SaveAsImage)->Enable(false);
    mb->FindItem(MENUCMD_Print)->Enable(false);
    mb->FindItem(MENUCMD_PrintPreview)->Enable(false);
    mb->FindItem(MENUCMD_Cleanup)->Enable(false);

    this->startRunImpl();
    this->turtleSpace->startRun(this->runForPrinting);

    this->execState = EXEC_RUNNING;
} // TobyFrame::startRun


void TobyFrame::stopRun()
{
    wxASSERT(this->isRunning());

    wxMenuBar *mb = this->menuBar;
    mb->FindItem(MENUCMD_RunOrStop)->SetText(wxT("&Run Program\tF5"));
    mb->FindItem(MENUCMD_RunOrStop)->Enable(true);
    mb->FindItem(MENUCMD_RunForPrinting)->Enable(true);
    mb->FindItem(MENUCMD_SaveAsImage)->Enable(true);
    mb->FindItem(MENUCMD_Print)->Enable(true);
    mb->FindItem(MENUCMD_PrintPreview)->Enable(true);
    mb->FindItem(MENUCMD_Cleanup)->Enable(true);

    this->stopRunImpl();
    this->turtleSpace->stopRun();
    this->execState = EXEC_STOPPED;
} // TobyFrame::stopRun


void TobyFrame::requestQuit()
{
    this->haltProgram();
    this->quitting = true;
} // TobyFrame::requestQuit


void TobyFrame::haltProgram()
{
    if (this->isRunning())
        this->execState = EXEC_STOPPING;
} // TobyFrame::haltProgram


void TobyFrame::runProgram(bool _runForPrinting)
{
    this->haltProgram();  // stop the current run as soon as possible.
    // This gets kicked off in the next idle event.
    this->runForPrinting = _runForPrinting;
    delete[] this->execProgram;
    this->execProgram = this->getProgramImpl();
} // TobyFrame::runProgram


void TobyFrame::onIdle(wxIdleEvent &evt)
{
    if (this->isQuitting())
    {
        if (this->isRunning())
            this->haltProgram();
        else
        {
            this->quitting = false;
            this->Close(false);
        } // else
    } // if

    else if (this->execProgram != NULL)
    {
        if (this->isRunning())
            this->haltProgram();
        else
        {
            char *prog = this->execProgram;
            this->execProgram = NULL;
            TOBY_runProgram(prog, this->runForPrinting);
            delete[] prog;
        } // else
    } // else if
} // TobyFrame::onIdle


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
            this->openFileImpl(path, buf);  // openFileImpl will delete[]!
        } // else
    } // else
} // TobyFrame::openFile


void TobyFrame::onMenuOpen(wxCommandEvent& evt)
{
    // !!! FIXME: localization.
    wxFileDialog dlg(this, wxT("Choose a file"), wxT(""), wxT(""),
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
    wxFileDialog dlg(this, wxT("Choose a file"), wxT(""), wxT(""),
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


void TobyFrame::onMenuRunOrStop(wxCommandEvent &evt)
{
    if (this->isRunning())
        this->haltProgram();
    else
        this->runProgram(false);  // Run will kick off in next idle event.
} // TobyFrame::onMenuRunOrStop


void TobyFrame::onMenuStep(wxCommandEvent &evt)
{
    if (!this->isStopping())
    {
        // if it's not running, start it.
        if (!this->isRunning())
            this->runProgram(false);  // Run will kick off in next idle event.
        this->execState = EXEC_STEPPING;
    } // if
} // TobyFrame::onMenuStep


void TobyFrame::onMenuRunForPrinting(wxCommandEvent &evt)
{
    // Run will kick off in next idle event.
    wxASSERT(!this->isRunning());
    this->runProgram(true);
} // TobyFrame::onMenuRunForPrinting


void TobyFrame::onMenuCleanup(wxCommandEvent &evt)
{
    this->turtleSpace->cleanup(0, 0, 0, true);
    this->turtleSpace->putToScreen();
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
    if (this->shouldVetoClose())
        evt.Veto();  // subclass says no.

    else if (this->isRunning())
    {
        this->requestQuit();  // try it again later so program can halt...
        evt.Veto();  // ...this time, though, no deal.
    } // else if

    else  // really closing this time.
    {
        // !!! FIXME: this may not be the best place for this...
        wxConfigBase *cfg = wxConfig::Get();
        cfg->Write(wxT("LastWindowW"), (long) this->nonMaximizedWidth);
        cfg->Write(wxT("LastWindowH"), (long) this->nonMaximizedHeight);
        cfg->Write(wxT("LastWindowX"), (long) this->nonMaximizedX);
        cfg->Write(wxT("LastWindowY"), (long) this->nonMaximizedY);
        cfg->Write(wxT("Maximized"), (long) this->IsMaximized() ? 1 : 0);
        this->Destroy();
    } // else
} // TobyFrame::onClose


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
    char *retval = NULL;
    if (this->sourceCode != NULL)
    {
        retval = new char[strlen(this->sourceCode) + 1];
        strcpy(retval, this->sourceCode);
    } // if

    return retval;
} // TobyStandaloneFrame::getProgramImpl


void TobyStandaloneFrame::openFileImpl(const wxString &fname, char *prog)
{
    // Run will kick off in next idle event.
    delete[] this->sourceCode;
    this->sourceCode = prog;  // we have to delete[] this later!
    this->runProgram(false);  // just run right away.
} // TobyStandaloneFrame::openFileImpl

#endif  // TOBY_WX_BUILD_STANDALONE



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
    wxConfig::Set(cfg);

    tobyInitAllImageHandlers();

    if (standalone)
    {
        #if TOBY_WX_BUILD_STANDALONE
        cfg->SetPath(wxT("/Standalone"));
        this->mainWindow = new TobyStandaloneFrame;
        #else
        fprintf(stderr, "ERROR: No standalone app support in this build.\n");
        return false;
        #endif
    } // if
    else
    {
        #if TOBY_WX_BUILD_IDE
        cfg->SetPath(wxT("/IDE"));
        this->mainWindow = new TobyIDEFrame;
        #else
        fprintf(stderr, "ERROR: No IDE app support in this build.\n");
        return false;
        #endif
    } // else

    long mx = 0;
    if ((wxConfig::Get()->Read(wxT("Maximized"), &mx, -1)) && (mx))
        this->mainWindow->Maximize();

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

