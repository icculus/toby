#ifdef __APPLE__
#include <Carbon/Carbon.h>
#endif

#include <wx/wx.h>
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
    void startingNewRun();
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
//  the actual TurtleSpace canvas...there are two subclasses of this:
//  one is a standalone window that only contains a turtlespace, and one
//  is a full layout of controls for editing, debugging, etc.
// So to get the TurtleSpace to draw on from any arbitrary place, you'd do
//  this:  TurtleSpace *ts = wxGetApp().getTobyWindow()->getTurtleSpace();
class TobyWindow : public wxFrame
{
public:
    TobyWindow(const wxPoint &pos, const wxSize &size);
    TurtleSpace *getTurtleSpace() { return &this->turtleSpace; }

protected:
    TurtleSpace turtleSpace;
};


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

int TOBY_pumpEvents()
{
    wxGetApp().Yield();
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
    printf("TurtleSpace::TurtleSpace\n");
} // TurtleSpace::TurtleSpace


TurtleSpace::~TurtleSpace()
{
    printf("TurtleSpace::~TurtleSpace\n");
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


void TurtleSpace::startingNewRun()
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

        // !!! FIXME: test code, ditch this...
        TOBY_drawLine(10, 10, 990, 990, 255, 0, 0);
        TOBY_drawLine(990, 10, 10, 990, 0, 255, 0);
        TOBY_drawLine(500, 10, 500, 990, 0, 0, 255);
    } // if
} // TurtleSpace::startingNewRun


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



TobyWindow::TobyWindow(const wxPoint &pos, const wxSize &size)
    // !!! FIXME: lose default wxFrame params when we have resize support...
    : wxFrame(NULL, -1, wxT("Toby"), pos, size)
    , turtleSpace(this)
{
    // no-op ... but don't forget to resize turtleSpace!
    printf("TobyWindow::TobyWindow\n");
} // TobyWindow::TobyWindow


TobyStandaloneFrame::TobyStandaloneFrame()
    : TobyWindow(wxPoint(200, 200), wxSize(400, 400))  // !!! FIXME
{
    printf("TobyStandaloneFrame::TobyStandaloneFrame\n");
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(MENUCMD_Open, wxT("O&pen"), wxT("Open"));
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
    printf("TobyStandaloneFrame::onMenuOpen\n");
    getTurtleSpace()->startingNewRun();  // !!! FIXME: shouldn't be here...
} // TobyStandaloneFrame::onMenuOpen


void TobyStandaloneFrame::onResize(wxSizeEvent &evt)
{
    printf("TobyStandaloneFrame::onResize\n");
    this->turtleSpace.SetSize(this->GetClientSize());
} // TobyStandaloneFrame::onResize




IMPLEMENT_APP(TobyWxApp)

bool TobyWxApp::OnInit()
{
    printf("TobyWxApp::OnInit\n");

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

    #if TOBY_STANDALONE
        this->mainWindow = new TobyStandaloneFrame;
    #else
        #error Write me.
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
    printf("TobyWxApp::OnExit\n");
    mainWindow = NULL;  // this is probably deleted already.
    return wxApp::OnExit();
} // TobyWxApp::OnExit

// end of toby_wxwidgets.cpp ...

