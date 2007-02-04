#ifdef __APPLE__
#include <Carbon/Carbon.h>
#endif

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/wfstream.h>
#include <wx/aboutdlg.h>
#include <wx/print.h>
#include <wx/printdlg.h>

#include "toby_app.h"


// !!! FIXME: Just building standalone for now...
#define TOBY_STANDALONE 1

static bool GQuitProgramRequested = false;


// Interfaces ...

// TurtleSpace is the canvas we draw on...it manages a backing store for
//  repaints, if necessary for a given wxWidgets target, and handles
//  the canvas if the window resizes.
class TurtleSpace : public wxWindow
{
public:
    TurtleSpace(wxWindow *parent);
    virtual ~TurtleSpace();
    inline void startRun();
    inline void stopRun();
    inline int pumpEvents();
    inline void calcOffset(int &xoff, int &yoff) const;
    inline void clipDC(wxDC &dc, int xoff, int yoff) const;
    inline wxBitmap *getBacking() const { return this->backing; }
    inline wxDC *getBackingDC() const { return this->backingDC; }
    inline void scaleXY(int &x, int &y) const;
    inline void runProgram(char *program, bool printing);  // hook to GUI.
    inline void halt();  // hook to GUI.
    bool isRunning() const { return this->running; }
    bool stopRequested() const { return (this->stopping) || (!this->running); }

    void onResize(wxSizeEvent &evt);
    void onPaint(wxPaintEvent &evt);
    void onIdle(wxIdleEvent &evt);

    enum { windowFlags=wxFULL_REPAINT_ON_RESIZE };

private:
    char *program;
    bool stopping;
    bool running;
    bool runForPrinting;
    int clientW;  // width of physical window (changes on resize event).
    int clientH;  // height of physical window (changes on resize event).
    int backingW;  // width of backing store (changes on startRun()).
    int backingH;  // height of backing store (changes on startRun()).
    wxBitmap *backing;
    wxMemoryDC *backingDC;
    wxStopWatch stopwatch;
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TurtleSpace, wxWindow)
    EVT_SIZE(TurtleSpace::onResize)
    EVT_PAINT(TurtleSpace::onPaint)
    EVT_IDLE(TurtleSpace::onIdle)
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
    MENUCMD_Run = wxID_HIGHEST,

    // non-standard menu items go here.
    MENUCMD_RunForPrinting,
    MENUCMD_SaveAsImage,
    MENUCMD_Stop,
    MENUCMD_Cleanup,
    MENUCMD_Website,
    MENUCMD_License,
};

// TobyWindow is a standard wxFrame, but adds an interface for getting
//  the actual TurtleSpace canvas and other highlevel wankery...there are two
//  subclasses of this: one is a standalone window that only contains a
//  turtlespace, and one is a full layout of controls for editing, debugging,
//  etc.
// So to get the TurtleSpace to draw on from any arbitrary place, you'd do
//  this:  TurtleSpace *ts = wxGetApp().getTobyWindow()->getTurtleSpace();
class TobyWindow : public wxFrame
{
public:
    TobyWindow();
    TurtleSpace *getTurtleSpace() { return &this->turtleSpace; }
    static const wxPoint getPreviousPos();
    static const wxSize getPreviousSize();
    void openFile(const wxString &path);
    void startRun() { this->startRunImpl(); this->turtleSpace.startRun(); }
    void stopRun() { this->stopRunImpl(); this->turtleSpace.stopRun(); }
    virtual void openFileImpl(const wxString &fname, char *buf) = 0;
    virtual void resizeImpl(wxSizeEvent &evt) = 0;
    virtual void startRunImpl() = 0;
    virtual void stopRunImpl() = 0;
    void onClose(wxCloseEvent &evt);
    void onResize(wxSizeEvent &evt);
    void onMove(wxMoveEvent &evt);
    void onMenuPageSetup(wxCommandEvent &evt);
    void onMenuPrintPreview(wxCommandEvent &evt);
    void onMenuPrint(wxCommandEvent &evt);
    void onMenuAbout(wxCommandEvent &evt);
    void onMenuWebsite(wxCommandEvent &evt);
    void onMenuLicense(wxCommandEvent &evt);
    void onMenuOpen(wxCommandEvent &evt);
    void onMenuSaveAsImage(wxCommandEvent &evt);
    void onMenuCleanup(wxCommandEvent &evt);

protected:
    TurtleSpace turtleSpace;
    int nonMaximizedX;
    int nonMaximizedY;
    int nonMaximizedWidth;
    int nonMaximizedHeight;

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TobyWindow, wxFrame)
    EVT_CLOSE(TobyWindow::onClose)
    EVT_SIZE(TobyWindow::onResize)
    EVT_MOVE(TobyWindow::onMove)
    EVT_MENU(MENUCMD_Open, TobyWindow::onMenuOpen)
    EVT_MENU(MENUCMD_SaveAsImage, TobyWindow::onMenuSaveAsImage)
    EVT_MENU(MENUCMD_PageSetup, TobyWindow::onMenuPageSetup)
    EVT_MENU(MENUCMD_PrintPreview, TobyWindow::onMenuPrintPreview)
    EVT_MENU(MENUCMD_Print, TobyWindow::onMenuPrint)
    EVT_MENU(MENUCMD_About, TobyWindow::onMenuAbout)
    EVT_MENU(MENUCMD_Website, TobyWindow::onMenuWebsite)
    EVT_MENU(MENUCMD_License, TobyWindow::onMenuLicense)
    EVT_MENU(MENUCMD_Cleanup, TobyWindow::onMenuCleanup)
END_EVENT_TABLE()


// This is a TobyWindow that only provides a TurtleSpace and no other UI.
class TobyStandaloneFrame : public TobyWindow
{
public:
    TobyStandaloneFrame();
    virtual ~TobyStandaloneFrame();
    void onMenuQuit(wxCommandEvent &evt);
    void onMenuRun(wxCommandEvent &evt);
    void onMenuStop(wxCommandEvent &evt);
    void onMenuRunForPrinting(wxCommandEvent &evt);
    virtual void resizeImpl(wxSizeEvent &evt);
    virtual void openFileImpl(const wxString &fname, char *prog);
    virtual void startRunImpl();
    virtual void stopRunImpl();

private:
    char *program;
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TobyStandaloneFrame, TobyWindow)
    EVT_MENU(MENUCMD_Quit, TobyStandaloneFrame::onMenuQuit)
    EVT_MENU(MENUCMD_Run, TobyStandaloneFrame::onMenuRun)
    EVT_MENU(MENUCMD_RunForPrinting, TobyStandaloneFrame::onMenuRunForPrinting)
    EVT_MENU(MENUCMD_Stop, TobyStandaloneFrame::onMenuStop)
END_EVENT_TABLE()



// This is the the Application itself.
class TobyWxApp : public wxApp
{
public:
    TobyWxApp() : mainWindow(NULL), printData(NULL) { /* no-op. */ }
    virtual bool OnInit();
    virtual int OnExit();
    TobyWindow *getTobyWindow() const { return this->mainWindow; }
    inline wxPrintData *getPrintData();

private:
    TobyWindow *mainWindow;
    wxPrintData *printData;
};

DECLARE_APP(TobyWxApp)




// implementations of callbacks during execution of Toby programs...

int TOBY_delay(int ms)
{
    wxLongLong now = ::wxGetLocalTimeMillis();
    const wxLongLong end = now + ((wxLongLong) ms);
    while (now < end)
    {
        if (!TOBY_pumpEvents())
            return 0;
        now = ::wxGetLocalTimeMillis();
        if (now < end)
        {
            const unsigned long ticks = (unsigned long) ((end-now).ToLong());
            ::wxMilliSleep((ticks > 50) ? 50 : ticks);
            now = ::wxGetLocalTimeMillis();
        } // if
    } // while

    return TOBY_pumpEvents();
} // TOBY_delay


#define TOBY_PROFILE 1
#if TOBY_PROFILE
static wxStopWatch *stopWatch = NULL;
#endif

void TOBY_startRun()
{
    wxGetApp().getTobyWindow()->startRun();

    #if TOBY_PROFILE
    stopWatch = new wxStopWatch;
    #endif
} // TOBY_startRun


void TOBY_stopRun()
{
    #if TOBY_PROFILE
    printf("time to execute: %ld\n", stopWatch->Time());
    delete stopWatch;
    stopWatch = NULL;
    #endif

    wxGetApp().getTobyWindow()->stopRun();
} // TOBY_stopRun


int TOBY_pumpEvents()
{
    return wxGetApp().getTobyWindow()->getTurtleSpace()->pumpEvents();
} // TOBY_pumpEvents


void TOBY_drawLine(int x1, int y1, int x2, int y2, int r, int g, int b)
{
    TurtleSpace *tspace = wxGetApp().getTobyWindow()->getTurtleSpace();
    wxPen pen(wxColour(r, g, b));

    tspace->scaleXY(x1, y1);
    tspace->scaleXY(x2, y2);

    wxDC *backDC = tspace->getBackingDC();
    if (backDC != NULL)
    {
        backDC->SetPen(pen);
        backDC->DrawLine(x1, y1, x2, y2);
    } // if

    int xoff, yoff;
    tspace->calcOffset(xoff, yoff);
    wxClientDC clientdc(tspace);
    tspace->clipDC(clientdc, xoff, yoff);
    clientdc.SetPen(pen);
    clientdc.DrawLine(x1+xoff, y1+yoff, x2+xoff, y2+yoff);
} // TOBY_drawLine


void TOBY_drawTurtle(int x, int y, int angle, int w, int h)
{
    // !!! FIXME: write me.
} // TOBY_drawTurtle


void TOBY_cleanup(int r, int g, int b)
{
    TurtleSpace *tspace = wxGetApp().getTobyWindow()->getTurtleSpace();
    wxBrush brush(wxColour(r, g, b));

    wxDC *backDC = tspace->getBackingDC();
    if (backDC != NULL)
    {
        backDC->SetBackground(brush);
        backDC->Clear();
    } // if

    wxClientDC clientdc(tspace);
    clientdc.SetBackground(brush);
    clientdc.Clear();
} // TOBY_cleanup


void TOBY_messageBox(const char *msg)
{
    ::wxMessageBox(wxString(msg, wxConvUTF8), wxString(wxT("Toby")));
} // TOBY_messageBox




// The rest of the application...

TurtleSpace::TurtleSpace(wxWindow *parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, windowFlags)
    , program(NULL)
    , stopping(false)
    , running(false)
    , runForPrinting(false)
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
    delete this->program;
} // TurtleSpace::~TurtleSpace


void TurtleSpace::scaleXY(int &x, int &y) const
{
    x = (int) (((float) this->backingW) * (((float) x) / 1000.0f));
    y = (int) (((float) this->backingH) * (((float) y) / 1000.0f));
} // TurtleSpace::scaleXY


void TurtleSpace::calcOffset(int &xoff, int &yoff) const
{
    xoff = (this->clientW - this->backingW) / 2;
    yoff = (this->clientH - this->backingH) / 2;
} // TurtleSpace::calcOffset


void TurtleSpace::clipDC(wxDC &dc, int xoff, int yoff) const
{
    const int w = this->backingW;
    const int h = this->backingH;
    if ((w < this->clientW) || (h < this->clientH))
        dc.SetClippingRegion(xoff, yoff, w, h);
} // TurtleSpace::clipDC


void TurtleSpace::startRun()
{
    wxASSERT(!this->running);

    int w = this->clientW;
    int h = this->clientH;

    if (this->runForPrinting)
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

    // resized since last run?
    if ((w != this->backingW) || (h != this->backingH))
    {
        // (these may be NULL already, that's okay.)
        delete this->backingDC;
        delete this->backing;
        this->backingDC = NULL;
        this->backing = NULL;
    } // if

    // Build new backing store if necessary...
    if (this->backing == NULL)
    {
        wxASSERT(this->backingDC == NULL);
        this->backing = new wxBitmap(w, h);
        this->backingDC = new wxMemoryDC(*this->backing);
        this->backingW = w;
        this->backingH = h;
    } // if

    this->running = true;
    this->stopping = false;
} // TurtleSpace::startRun


void TurtleSpace::stopRun()
{
    wxASSERT(this->running);
    this->running = this->stopping = false;
} // TurtleSpace::stopRun


int TurtleSpace::pumpEvents()
{
    if (this->stopwatch.Time() > 50)
    {
        while ((!this->stopRequested()) && (wxGetApp().Pending()))
            wxGetApp().Dispatch();
        this->stopwatch.Start(0);  // reset this for next call.
    } /* if */

    return !this->stopRequested();
} // TurtleSpace::pumpEvents


void TurtleSpace::halt()
{
    if (this->running)
        this->stopping = true;
} // TurtleSpace::halt


void TurtleSpace::runProgram(char *_program, bool _runForPrinting)
{
    this->halt();  // stop the current run as soon as possible.
    // This gets kicked off in the next idle event.
    //  Don't delete[] _program until it's finished running!
    this->runForPrinting = _runForPrinting;
    delete[] this->program;
    this->program = _program;
} // TurtleSpace::runProgram


void TurtleSpace::onIdle(wxIdleEvent &evt)
{
    if (GQuitProgramRequested)
    {
        if (this->isRunning())
            this->halt();
        else
        {
            GQuitProgramRequested = false;
            GetParent()->Close(false);
        } // else
    } // if
    else if (this->program != NULL)
    {
        if (this->isRunning())
            this->halt();
        else
        {
            char *prog = this->program;
            this->program = NULL;
            TOBY_runProgram(prog, this->runForPrinting);
        } // else
    } // else if
} // TurtleSpace::onIdle


void TurtleSpace::onResize(wxSizeEvent &evt)
{
    // Just cache the dimensions, since we're spending an enormous amount of
    //  time looking them up over and over during program execution.
    wxSize size(this->GetClientSize());
    this->clientW = size.GetWidth();
    this->clientH = size.GetHeight();
} // TurtleSpace::onResize


void TurtleSpace::onPaint(wxPaintEvent &evt)
{
    wxPaintDC dc(this);
    int r, g, b;
    TOBY_background(&r, &g, &b);
    dc.SetBackground(wxBrush(wxColour(r, g, b)));
    dc.Clear();
    if (this->backing != NULL)
    {
        int xoff, yoff;
        this->calcOffset(xoff, yoff);
        dc.DrawBitmap(*this->backing, xoff, yoff, false);
    } // if
} // TurtleSpace::onPaint


bool TobyPrintout::OnPrintPage(int page)
{
    wxBitmap *bmp = wxGetApp().getTobyWindow()->getTurtleSpace()->getBacking();
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
} // TobyPrintout::HasPage



TobyWindow::TobyWindow()
    : wxFrame(NULL, -1, wxT("Toby"), getPreviousPos(), getPreviousSize())
    , turtleSpace(this)
{
    this->GetPosition(&this->nonMaximizedX, &this->nonMaximizedY);
    this->GetSize(&this->nonMaximizedWidth, &this->nonMaximizedHeight);
    // ...don't forget to resize turtleSpace somewhere in your subclass!
} // TobyWindow::TobyWindow


const wxPoint TobyWindow::getPreviousPos()
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
} // TobyWindow::getPreviousPos


const wxSize TobyWindow::getPreviousSize()
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
} // TobyWindow::getPreviousSize


void TobyWindow::openFile(const wxString &path)
{
    wxFileInputStream strm(path);
    if (!strm.IsOk())
        TOBY_messageBox("Could not open file");
    else
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
} // TobyWindow::openFile


void TobyWindow::onMenuOpen(wxCommandEvent& evt)
{
    // !!! FIXME: localization.
    wxFileDialog dlg(this, wxT("Choose a file"), wxT(""), wxT(""),
                     wxT("Toby Programs (*.toby)|*.toby"),
                     wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (dlg.ShowModal() == wxID_OK)
        openFile(dlg.GetPath());
} // TobyWindow::onMenuOpen


void TobyWindow::onMenuSaveAsImage(wxCommandEvent &evt)
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
        wxImage img(getTurtleSpace()->getBacking()->ConvertToImage());
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
} // TobyWindow::onMenuSaveAsImage


void TobyWindow::onMenuPageSetup(wxCommandEvent &event)
{
    wxPrintData *printData = wxGetApp().getPrintData();
    wxPageSetupDialogData pageSetupData(*printData);
    wxPageSetupDialog pageSetupDialog(this, &pageSetupData);
    pageSetupDialog.ShowModal();
    *printData = pageSetupDialog.GetPageSetupDialogData().GetPrintData();
} // TobyWindow::onMenuPageSetup


void TobyWindow::onMenuPrintPreview(wxCommandEvent &event)
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
} // TobyWindow::onMenuPrintPreview


void TobyWindow::onMenuPrint(wxCommandEvent &event)
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
} // TobyWindow::onMenuPrint


void TobyWindow::onMenuCleanup(wxCommandEvent &evt)
{
    TOBY_cleanup(0, 0, 0);
} // TobyWindow::onMenuCleanup


void TobyWindow::onMenuAbout(wxCommandEvent &evt)
{
    wxAboutDialogInfo info;
    info.SetName(wxT("Toby"));
    info.SetVersion(wxT("0.1"));
    info.SetDescription(wxT("A programming language for learning."));
    info.SetCopyright(wxT("(C) 2007 Ryan C. Gordon <icculus@icculus.org>"));
    ::wxAboutBox(info);
} // TobyWindow::onMenuAbout


void TobyWindow::onMenuLicense(wxCommandEvent &evt)
{
    // !!! FIXME: this isn't right.
    wxMessageDialog dlg(NULL, wxString(GLuaLicense, wxConvUTF8), wxT("License"), wxOK);
    dlg.ShowModal();
} // TobyWindow::onMenuLicense


void TobyWindow::onMenuWebsite(wxCommandEvent &evt)
{
    wxLaunchDefaultBrowser(wxT("http://icculus.org/toby/"));
} // TobyWindow::onMenuWebsite


void TobyWindow::onResize(wxSizeEvent &evt)
{
    if (!this->IsMaximized())
        this->GetSize(&this->nonMaximizedWidth, &this->nonMaximizedHeight);
    this->resizeImpl(evt);
} // TobyWindow::onResize


void TobyWindow::onMove(wxMoveEvent &evt)
{
    if (!this->IsMaximized())
        this->GetPosition(&this->nonMaximizedX, &this->nonMaximizedY);
} // TobyWindow::onMove


void TobyWindow::onClose(wxCloseEvent &evt)
{
    TurtleSpace *tspace = wxGetApp().getTobyWindow()->getTurtleSpace();
    if (tspace->isRunning())
    {
        tspace->halt();
        GQuitProgramRequested = true; // try it again later so tspace can halt.
        evt.Veto();  // this time, though, no deal.
    } // if

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
} // TobyWindow::onClose


// !!! FIXME: overflowing 80 chars here...
TobyStandaloneFrame::TobyStandaloneFrame()
{
    this->program = NULL;

    wxMenu *file_menu = new wxMenu;
    file_menu->Append(MENUCMD_Open, wxT("&Open\tCtrl-O"));
    file_menu->Append(MENUCMD_SaveAsImage, wxT("&Save As Image\tCtrl-S"))->Enable(false);
    file_menu->Append(MENUCMD_PageSetup, wxT("Pa&ge Setup"));
    file_menu->Append(MENUCMD_PrintPreview, wxT("P&rint Preview"))->Enable(false);
    file_menu->Append(MENUCMD_Print, wxT("&Print\tCtrl-P"))->Enable(false);
    file_menu->AppendSeparator();
    file_menu->Append(MENUCMD_Quit, wxT("E&xit\tCtrl-X"));

    wxMenu *run_menu = new wxMenu;
    run_menu->Append(MENUCMD_Run, wxT("&Run Program\tF5"))->Enable(false);
    run_menu->Append(MENUCMD_RunForPrinting, wxT("R&un Program for Printing"))->Enable(false);
    run_menu->Append(MENUCMD_Stop, wxT("&Stop Program\tESC"))->Enable(false);
    run_menu->Append(MENUCMD_Cleanup, wxT("&Clean up TurtleSpace"))->Enable(false);

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(MENUCMD_About, wxT("&About\tF1"));
    help_menu->Append(MENUCMD_License, wxT("&License"));
    help_menu->Append(MENUCMD_Website, wxT("Toby on the &Web"));

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, wxT("&File"));
    menu_bar->Append(run_menu, wxT("&Run"));
    menu_bar->Append(help_menu, wxT("&Help"));
    this->SetMenuBar(menu_bar);
} // TobyStandaloneFrame::TobyStandaloneFrame


TobyStandaloneFrame::~TobyStandaloneFrame()
{
    delete[] this->program;
} // TobyStandaloneFrame::~TobyStandaloneFrame


void TobyStandaloneFrame::onMenuQuit(wxCommandEvent& evt)
{
    Close(true);
} // TobyStandaloneFrame::onMenuQuit


void TobyStandaloneFrame::openFileImpl(const wxString &fname, char *prog)
{
    // Run will kick off in next idle event.
    this->program = prog;  // we have to delete[] this later!
    turtleSpace.runProgram(this->program, false);
} // TobyStandaloneFrame::openFileImpl


void TobyStandaloneFrame::onMenuRun(wxCommandEvent &evt)
{
    // Run will kick off in next idle event.
    wxASSERT(this->program != NULL);
    turtleSpace.runProgram(this->program, false);
} // TobyStandaloneFrame::onMenuRun


void TobyStandaloneFrame::onMenuRunForPrinting(wxCommandEvent &evt)
{
    // Run will kick off in next idle event.
    wxASSERT(this->program != NULL);
    turtleSpace.runProgram(this->program, true);
} // TobyStandaloneFrame::onMenuRunForPrinting


void TobyStandaloneFrame::onMenuStop(wxCommandEvent &evt)
{
    turtleSpace.halt();
} // TobyStandaloneFrame::onMenuStop


void TobyStandaloneFrame::startRunImpl()
{
    wxMenuBar *mb = this->GetMenuBar();
    mb->FindItem(MENUCMD_Run)->Enable(false);
    mb->FindItem(MENUCMD_RunForPrinting)->Enable(false);
    mb->FindItem(MENUCMD_Stop)->Enable(true);
    mb->FindItem(MENUCMD_SaveAsImage)->Enable(false);
    mb->FindItem(MENUCMD_Print)->Enable(false);
    mb->FindItem(MENUCMD_PrintPreview)->Enable(false);
    mb->FindItem(MENUCMD_Cleanup)->Enable(false);
} // TobyStandaloneFrame::startRunImpl


void TobyStandaloneFrame::stopRunImpl()
{
    wxMenuBar *mb = this->GetMenuBar();
    mb->FindItem(MENUCMD_Run)->Enable(true);
    mb->FindItem(MENUCMD_RunForPrinting)->Enable(true);
    mb->FindItem(MENUCMD_Stop)->Enable(false);
    mb->FindItem(MENUCMD_SaveAsImage)->Enable(true);
    mb->FindItem(MENUCMD_Print)->Enable(true);
    mb->FindItem(MENUCMD_PrintPreview)->Enable(true);
    mb->FindItem(MENUCMD_Cleanup)->Enable(true);
} // TobyStandaloneFrame::stopRunImpl


void TobyStandaloneFrame::resizeImpl(wxSizeEvent &evt)
{
    this->turtleSpace.SetSize(this->GetClientSize());
} // TobyStandaloneFrame::resizeImpl



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
    #ifdef __APPLE__
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

    #if TOBY_STANDALONE
        cfg->SetPath(wxT("/Standalone"));
        this->mainWindow = new TobyStandaloneFrame;
    #else
        #error Write me.
        cfg->SetPath(wxT("/IDE"));
        this->mainWindow = new TobyWxGuiFrame;
    #endif

    if (this->mainWindow == NULL)
        return false;

    long mx = 0;
    if ((wxConfig::Get()->Read(wxT("Maximized"), &mx, -1)) && (mx))
        this->mainWindow->Maximize();

    this->mainWindow->Show(true);
    SetTopWindow(this->mainWindow);
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

