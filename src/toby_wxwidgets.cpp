#ifdef __APPLE__
#include <Carbon/Carbon.h>
#endif

#include <wx/wx.h>
#include "toby_app.h"

// !!! FIXME: Just building standalone for now...
#define TOBY_STANDALONE 1


// Interfaces ...

// TurtleSpace is the canvas we draw on...it manages a backing store for
//  repaints, if necessary for a given wxWidgets target, and handles rescaling
//  the canvas if the window resizes.
class TurtleSpace : public wxWindow
{
public:
    TurtleSpace(wxWindow *parent);
    virtual ~TurtleSpace();
    wxBitmap *getBacking() const { return this->backing; }
    wxDC *getBackingDC() const { return this->backingDC; }
    inline void scaleXY(int &x, int &y) const;
    void onResize(wxSizeEvent &evt);
    void onPaint(wxPaintEvent &evt);

private:
    // !!! FIXME: check wxWindow::IsDoubleBuffered() before building a
    // !!! FIXME:  backing store...
    wxBitmap *backing;
    wxMemoryDC *backingDC;
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TurtleSpace, wxWindow)
    EVT_SIZE(TurtleSpace::onResize)
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


static inline void doLine(wxDC *dc, wxPen &pen, int x1, int y1, int x2, int y2)
{
    if (dc != NULL)
    {
        dc->SetPen(pen);
        dc->DrawLine(x1, y1, x2, y2);
    } // if
} // doLine


void TOBY_drawLine(int x1, int y1, int x2, int y2, int r, int g, int b)
{
    TurtleSpace *tspace = wxGetApp().getTobyWindow()->getTurtleSpace();
    tspace->scaleXY(x1, y1);
    tspace->scaleXY(x2, y2);
    wxPen pen(wxColour(r, g, b));
    doLine(tspace->getBackingDC(), pen, x1, y1, x2, y2);
    wxClientDC clientdc(tspace);
    doLine(&clientdc, pen, x1, y1, x2, y2);
} // TOBY_drawLine


void TOBY_drawTurtle(int x, int y, int angle, int w, int h)
{
    // !!! FIXME: write me.
} // TOBY_drawTurtle


static inline void doCleanup(wxDC *dc, wxBrush &brush)
{
    if (dc != NULL)
    {
        wxColor origColor(dc->GetBackground().GetColour());
        dc->SetBackground(brush);
        dc->Clear();
        dc->SetBackground(wxBrush(origColor));
    } // if
} // doCleanup


void TOBY_cleanup(int r, int g, int b)
{
    TurtleSpace *tspace = wxGetApp().getTobyWindow()->getTurtleSpace();
    wxBrush brush(wxColour(r, g, b));
    doCleanup(tspace->getBackingDC(), brush);
    wxClientDC clientdc(tspace);
    doCleanup(&clientdc, brush);
} // TOBY_cleanup


void TOBY_messageBox(const char *msg)
{
    ::wxMessageBox(wxString(msg, wxConvUTF8), wxString(wxT("Toby")));
} // TOBY_messageBox




// The rest of the application...

TurtleSpace::TurtleSpace(wxWindow *parent)
    : wxWindow(parent, wxID_ANY)
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
    const wxSize &size = this->GetClientSize();
    x = ((float) size.GetWidth()) * (((float) x) / 1000.0f);
    y = ((float) size.GetHeight()) * (((float) y) / 1000.0f);
} // TurtleSpace::scaleXY


void TurtleSpace::onResize(wxSizeEvent &evt)
{
    printf("TurtleSpace::onResize\n");
} // TobyStandaloneFrame::onResize


void TurtleSpace::onPaint(wxPaintEvent &evt)
{
    printf("TurtleSpace::onPaint\n");
    wxPaintDC(this);

    // !!! FIXME: not right at all...
    TOBY_cleanup(0, 0, 0);
    TOBY_drawLine(10, 10, 990, 990, 255, 0, 0);
    TOBY_drawLine(990, 10, 10, 990, 0, 255, 0);
    TOBY_drawLine(500, 10, 500, 990, 0, 0, 255);
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

