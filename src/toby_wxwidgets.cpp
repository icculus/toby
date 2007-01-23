#ifdef __APPLE__
#include <Carbon/Carbon.h>
#endif

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/wfstream.h>
#include "toby_app.h"

// !!! FIXME: Just building standalone for now...
#define TOBY_STANDALONE 1


// Interfaces ...

// TurtleSpace is the canvas we draw on...it manages a backing store for
//  repaints, if necessary for a given wxWidgets target, and handles
//  the canvas if the window resizes.
class TurtleSpace : public wxWindow
{
public:
    TurtleSpace(wxWindow *parent);
    virtual ~TurtleSpace();
    void startRun();
    void calcOffset(int &xoff, int &yoff) const;
    void clipDC(wxDC &dc) const;
    wxBitmap *getBacking() const { return this->backing; }
    wxDC *getBackingDC() const { return this->backingDC; }
    inline void scaleXY(int &x, int &y) const;
    void onPaint(wxPaintEvent &evt);

    enum { windowFlags=wxFULL_REPAINT_ON_RESIZE };

private:
    int currentW;  // width for current run.
    int currentH;  // height for current run.
    wxBitmap *backing;
    wxMemoryDC *backingDC;
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TurtleSpace, wxWindow)
    EVT_PAINT(TurtleSpace::onPaint)
END_EVENT_TABLE()


enum TobyMenuCommands
{
    MENUCMD_Quit = 1,
    MENUCMD_Open,
    MENUCMD_New,
    MENUCMD_About,
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

protected:
    TurtleSpace turtleSpace;

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TobyWindow, wxFrame)
    EVT_CLOSE(TobyWindow::onClose)
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
    wxLongLong now = wxGetLocalTimeMillis();
    const wxLongLong end = now + ((wxLongLong) ms);
    while (now < end)
    {
        TOBY_pumpEvents();
        now = wxGetLocalTimeMillis();
        if (now < end)
        {
            const unsigned long ticks = (unsigned long) ((end-now).ToLong());
            wxMilliSleep((ticks > 50) ? 50 : ticks);
            now = wxGetLocalTimeMillis();
        } // if
    } // while

    return 1;  // !!! FIXME: allow user to kill program.
} // TOBY_delay


void TOBY_startRun()
{
    wxGetApp().getTobyWindow()->getTurtleSpace()->startRun();
} // TOBY_startRun


int TOBY_pumpEvents()
{
    // !!! FIXME: which of these do I really need?
    wxGetApp().Yield();
    while (wxGetApp().Pending())
        wxGetApp().Dispatch();

    return 1;   // !!! FIXME: this needs to check if stop run was requested.
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
    tspace->clipDC(clientdc);
    clientdc.SetPen(pen);
    clientdc.DrawLine(x1, y1, x2, y2);
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


void TurtleSpace::clipDC(wxDC &dc) const
{
    int xoff, yoff;
    this->calcOffset(xoff, yoff);
    if ((xoff != 0) || (yoff != 0))
    {
        dc.SetClippingRegion(xoff, yoff, this->currentW, this->currentH);
    } // if
} // TurtleSpace::clipDC


void TurtleSpace::startRun()
{
    int w, h;
    this->GetClientSize(&w, &h);

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
} // TurtleSpace::startRun


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


void TobyWindow::onClose(wxCloseEvent &evt)
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
} // TobyWindow::onClose


TobyStandaloneFrame::TobyStandaloneFrame()
{
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(MENUCMD_Open, wxT("&Open"), wxT("Open"));
    file_menu->AppendSeparator();
    file_menu->Append(MENUCMD_Quit, wxT("E&xit"), wxT("Exit"));
    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, wxT("&File"));
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
                TOBY_messageBox("Could not read file");
            else
            {
                buf[len] = '\0';
                TOBY_runProgram(buf);
            } // else
            delete[] buf;
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

