#ifdef __APPLE__
#include <Carbon/Carbon.h>
#endif

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/wfstream.h>
#include <wx/aboutdlg.h>

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
    inline void startRun();  // hook to TOBY_startRun() from toby_app
    inline void stopRun();   // hook to TOBY_stopRun() from toby_app
    inline int pumpEvents();  // hook to TOBY_pumpEvents() from toby_app
    inline void calcOffset(int &xoff, int &yoff) const;
    inline void clipDC(wxDC &dc, int xoff, int yoff) const;
    inline wxBitmap *getBacking() const { return this->backing; }
    inline wxDC *getBackingDC() const { return this->backingDC; }
    inline void scaleXY(int &x, int &y) const;
    inline void runProgram(char *program);  // hook to GUI.
    inline void halt();  // hook to GUI.
    bool isRunning() const { return this->running; }
    bool stopRequested() const { return (this->stopping) || (!this->running); }

    void onPaint(wxPaintEvent &evt);
    void onIdle(wxIdleEvent &evt);

    enum { windowFlags=wxFULL_REPAINT_ON_RESIZE };

private:
    char *program;
    bool stopping;
    bool running;
    int currentW;  // width for current run.
    int currentH;  // height for current run.
    wxBitmap *backing;
    wxMemoryDC *backingDC;
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TurtleSpace, wxWindow)
    EVT_PAINT(TurtleSpace::onPaint)
    EVT_IDLE(TurtleSpace::onIdle)
END_EVENT_TABLE()


enum TobyMenuCommands
{
    MENUCMD_About = wxID_ABOUT,
    MENUCMD_Quit = wxID_EXIT,
    MENUCMD_Open = wxID_HIGHEST,
    MENUCMD_New,
    MENUCMD_Run,
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
    void onClose(wxCloseEvent &evt);
    void onAbout(wxCommandEvent &evt);
    void onWebsite(wxCommandEvent &evt);
    void onLicense(wxCommandEvent &evt);

protected:
    TurtleSpace turtleSpace;

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TobyWindow, wxFrame)
    EVT_CLOSE(TobyWindow::onClose)
    EVT_MENU(MENUCMD_About, TobyWindow::onAbout)
    EVT_MENU(MENUCMD_Website, TobyWindow::onWebsite)
    EVT_MENU(MENUCMD_License, TobyWindow::onLicense)
END_EVENT_TABLE()


// This is a TobyWindow that only provides a TurtleSpace and no other UI.
class TobyStandaloneFrame : public TobyWindow
{
public:
    TobyStandaloneFrame();
    void onResize(wxSizeEvent &evt);
    void onMenuQuit(wxCommandEvent &evt);
    void onMenuOpen(wxCommandEvent &evt);

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TobyStandaloneFrame, TobyWindow)
    EVT_SIZE(TobyStandaloneFrame::onResize)
    EVT_MENU(MENUCMD_Quit, TobyStandaloneFrame::onMenuQuit)
    EVT_MENU(MENUCMD_Open, TobyStandaloneFrame::onMenuOpen)
END_EVENT_TABLE()



// This is the the Application itself.
class TobyWxApp : public wxApp
{
public:
    TobyWxApp() : mainWindow(NULL) { /* no-op */ }
    virtual bool OnInit();
    virtual int OnExit();
    TobyWindow *getTobyWindow() const { return this->mainWindow; }

private:
    TobyWindow *mainWindow;
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


void TOBY_startRun()
{
    wxGetApp().getTobyWindow()->getTurtleSpace()->startRun();
} // TOBY_startRun


void TOBY_stopRun()
{
    wxGetApp().getTobyWindow()->getTurtleSpace()->stopRun();
} // TOBY_stopRun


int TOBY_pumpEvents()
{
    const TurtleSpace *tspace = wxGetApp().getTobyWindow()->getTurtleSpace();
    while ((!tspace->stopRequested()) && (wxGetApp().Pending()))
        wxGetApp().Dispatch();
    return !tspace->stopRequested();
} // TOBY_pumpEvents


void TOBY_drawLine(int x1, int y1, int x2, int y2, int r, int g, int b)
{
    TurtleSpace *tspace = wxGetApp().getTobyWindow()->getTurtleSpace();
    int xoff, yoff;
    tspace->calcOffset(xoff, yoff);
    tspace->scaleXY(x1, y1);
    tspace->scaleXY(x2, y2);
    wxPen pen(wxColour(r, g, b));

    wxDC *backDC = tspace->getBackingDC();
    if (backDC != NULL)
    {
        backDC->SetPen(pen);
        backDC->DrawLine(x1, y1, x2, y2);
    } // if

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
    , currentW(1)
    , currentH(1)
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
    x = (int) (((float) this->currentW) * (((float) x) / 1000.0f));
    y = (int) (((float) this->currentH) * (((float) y) / 1000.0f));
} // TurtleSpace::scaleXY


void TurtleSpace::calcOffset(int &xoff, int &yoff) const
{
    int w, h;
    this->GetClientSize(&w, &h);
    xoff = (w - this->currentW) / 2;
    yoff = (h - this->currentH) / 2;
} // TurtleSpace::calcOffset


void TurtleSpace::clipDC(wxDC &dc, int xoff, int yoff) const
{
    if ((xoff != 0) || (yoff != 0))
    {
        dc.SetClippingRegion(xoff, yoff, this->currentW, this->currentH);
    } // if
} // TurtleSpace::clipDC


void TurtleSpace::startRun()
{
    wxASSERT(!this->running);

    int w, h;
    this->GetClientSize(&w, &h);

    if (w > h)
        w = h;
    else
        h = w;

    // resized since last run?
    if ((w != this->currentW) || (h != this->currentH))
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
        this->currentW = w;
        this->currentH = h;
        TOBY_cleanup(0, 0, 0);
    } // if

    this->running = true;
    this->stopping = false;
} // TurtleSpace::startRun


void TurtleSpace::stopRun()
{
    wxASSERT(this->running);
    this->running = this->stopping = false;
} // TurtleSpace::stopRun


void TurtleSpace::halt()
{
    if (this->running)
        this->stopping = true;
} // TurtleSpace::halt


void TurtleSpace::runProgram(char *_program)
{
    // This gets kicked off in the next idle event.
    //  TurtleSpace will delete[] _program sometime later!
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
        char *prog = this->program;
        this->program = NULL;
        TOBY_runProgram(prog);
        delete[] prog;
    } // else if
} // TurtleSpace::onIdle


void TurtleSpace::onPaint(wxPaintEvent &evt)
{
    wxPaintDC dc(this);
    dc.SetBackground(wxBrush(wxColour(0, 0, 0)));
    dc.Clear();
    if (this->backing != NULL)
    {
        int xoff, yoff;
        this->calcOffset(xoff, yoff);
        dc.DrawBitmap(*this->backing, xoff, yoff, false);
    } // if
} // TurtleSpace::onPaint



TobyWindow::TobyWindow()
    : wxFrame(NULL, -1, wxT("Toby"), getPreviousPos(), getPreviousSize())
    , turtleSpace(this)
{
    // no-op ... but don't forget to resize turtleSpace!
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


void TobyWindow::onAbout(wxCommandEvent &evt)
{
    wxAboutDialogInfo info;
    info.SetName(wxT("Toby"));
    info.SetVersion(wxT("0.1"));
    info.SetDescription(wxT("A programming language for learning."));
    info.SetCopyright(wxT("(C) 2007 Ryan C. Gordon <icculus@icculus.org>"));
    ::wxAboutBox(info);
} // TobyWindow::onAbout


void TobyWindow::onLicense(wxCommandEvent &evt)
{
    // !!! FIXME: this isn't right.
    wxMessageDialog dlg(NULL, wxString(GLuaLicense, wxConvUTF8), wxT("License"), wxOK);
    dlg.ShowModal();
} // TobyWindow::onLicense


void TobyWindow::onWebsite(wxCommandEvent &evt)
{
    wxLaunchDefaultBrowser(wxT("http://icculus.org/toby/"));
} // TobyWindow::onWebsite


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
        int winw, winh, winx, winy;
        this->GetSize(&winw, &winh);
        this->GetPosition(&winx, &winy);
        cfg->Write(wxT("LastWindowW"), (long) winw);
        cfg->Write(wxT("LastWindowH"), (long) winh);
        cfg->Write(wxT("LastWindowX"), (long) winx);
        cfg->Write(wxT("LastWindowY"), (long) winy);
        this->Destroy();
    } // else
} // TobyWindow::onClose


TobyStandaloneFrame::TobyStandaloneFrame()
{
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(MENUCMD_Open, wxT("&Open"), wxT("Open"));
    file_menu->AppendSeparator();
    file_menu->Append(MENUCMD_Quit, wxT("E&xit"), wxT("Exit"));

    wxMenu *run_menu = new wxMenu;
    run_menu->Append(MENUCMD_Run, wxT("&Run Program"));
    run_menu->Append(MENUCMD_Stop, wxT("&Stop Program"));
    run_menu->Append(MENUCMD_Cleanup, wxT("&Clean up TurtleSpace"));

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(MENUCMD_About, wxT("&About"));
    help_menu->Append(MENUCMD_License, wxT("&License"));
    help_menu->Append(MENUCMD_Website, wxT("Toby on the &Web"));

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, wxT("&File"));
    menu_bar->Append(run_menu, wxT("&Run"));
    menu_bar->Append(help_menu, wxT("&Help"));
    this->SetMenuBar(menu_bar);
} // TobyStandaloneFrame::TobyStandaloneFrame


void TobyStandaloneFrame::onMenuQuit(wxCommandEvent& evt)
{
    Close(true);
} // TobyStandaloneFrame::onMenuQuit


void TobyStandaloneFrame::onMenuOpen(wxCommandEvent& evt)
{
    // !!! FIXME: localization.
    wxFileDialog dlg(this, wxT("Choose a file"), wxT(""), wxT(""),
                     wxT("Toby Programs (*.toby)|*.toby"),
                     wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (dlg.ShowModal() == wxID_OK)
    {
        wxString path(dlg.GetPath());
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
                // Will kick off in next idle event.
                this->turtleSpace.runProgram(buf);
            } // else
        } // else
    } // if
} // TobyStandaloneFrame::onMenuOpen


void TobyStandaloneFrame::onResize(wxSizeEvent &evt)
{
    this->turtleSpace.SetSize(this->GetClientSize());
} // TobyStandaloneFrame::onResize




IMPLEMENT_APP(TobyWxApp)

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

    this->mainWindow->Show(true);
    SetTopWindow(this->mainWindow);
    return true;
} // TobyWxApp::OnInit


int TobyWxApp::OnExit()
{
    mainWindow = NULL;  // this is probably deleted already.
    return wxApp::OnExit();
} // TobyWxApp::OnExit

// end of toby_wxwidgets.cpp ...

