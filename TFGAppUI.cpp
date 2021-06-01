///////////////////////////////////////////////////////////////////////////////
// Name:        TFGAppUI.cpp
// Purpose:     Final degree work.
// Author:      David Lozano Pascual
// Created:     2021-06-01
// Copyright:   (C) Copyright 2021, David Lozano Pascual, All Rights Reserved.
// Licence:     wxWindows Library Licence, Version 3.1
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/app.h"
#include "wx/grid.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/vector.h"
#include "wx/propgrid/propgrid.h"
#include "wx/propgrid/advprops.h"
#include "wx/propgrid/manager.h"
#include "wx/propgrid/props.h"
#include "wx/treectrl.h"
#include "wx/listctrl.h"
#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/image.h"
#include "wx/bitmap.h"
#include "wx/graphics.h"
#include "wx/wfstream.h"
#include "wx/stdpaths.h"
#include "wx/config.h"
#include "wx/confbase.h"
#include "wx/fileconf.h"
#include "wx/display.h"
#include "wx/wfstream.h"
#include "wx/txtstrm.h"
#include "wx/textfile.h"
#include "wx/stattext.h"
#include "wx/font.h"
#include "wx/gdicmn.h" 
#include "wx/numdlg.h"



#include "wx/aui/aui.h"
//#include "../sample.xpm"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif
using namespace std;



// ----------------------------------------------------------------------------
///CUSTOM EVENTS///////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------

wxDECLARE_EVENT(BORRA_PANEL, wxCommandEvent);
wxDECLARE_EVENT(CLICK_PANEL, wxCommandEvent);
wxDECLARE_EVENT(BORRA_TEXTO, wxCommandEvent);
wxDECLARE_EVENT(CLICK_TEXTO, wxCommandEvent);
wxDECLARE_EVENT(PANEL_UP, wxCommandEvent);
wxDECLARE_EVENT(TEXT_UP, wxCommandEvent);


wxDEFINE_EVENT(BORRA_PANEL, wxCommandEvent);
wxDEFINE_EVENT(CLICK_PANEL, wxCommandEvent);
wxDEFINE_EVENT(BORRA_TEXTO, wxCommandEvent);
wxDEFINE_EVENT(CLICK_TEXTO, wxCommandEvent);
wxDEFINE_EVENT(PANEL_UP, wxCommandEvent);
wxDEFINE_EVENT(TEXT_UP, wxCommandEvent);

// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------


wxDisplay display;
wxRect screen = display.GetClientArea();
wxSize tamDisplay = wxSize(screen.width, screen.height);

float escala = 1.3;

wxSize tamInspector = wxSize(screen.width/5, screen.height/2);
wxSize tamJerarquia = wxSize(screen.width/5, screen.height/4);

wxSize tamVista = wxSize(screen.width - screen.width/5, tamDisplay.y);
wxSize tamVistaImagen = wxSize(tamDisplay.x/escala, tamDisplay.y/escala);

int tamVentanaX;
int tamVentanaY;



void CreateConsole()
{
    // allocate a console for this app
    if (!AllocConsole())
        MessageBox(NULL, L"The console window was not created",
            NULL, MB_ICONEXCLAMATION);

#if (_MSC_VER <= 1800)

    FILE* fp;
    // set the screen buffer to be big enough to let us scroll text
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 300;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    // redirect unbuffered STDOUT to the console
    int hConHandle;
    HANDLE stdHandle;
    stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);

    // redirect unbuffered STDIN to the console
    stdHandle = GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "r");
    *stdin = *fp;
    setvbuf(stdin, NULL, _IONBF, 0);

    // redirect unbuffered STDERR to the console
    stdHandle = GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    *stderr = *fp;
    setvbuf(stderr, NULL, _IONBF, 0);

#else

    FILE* fp;
    errno_t error;
    bool ok;

    error = freopen_s(&fp, "CONIN$", "r", stdin);
    error = freopen_s(&fp, "CONOUT$", "w", stderr);
    error = freopen_s(&fp, "CONOUT$", "w", stdout);

    // Note that there is no CONERR$ file
    HANDLE hStdout = CreateFile(L"CONOUT$", GENERIC_READ |
        GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hStdin = CreateFile(L"CONIN$", GENERIC_READ |
        GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    ok = SetStdHandle(STD_OUTPUT_HANDLE, hStdout);
    ok = SetStdHandle(STD_ERROR_HANDLE, hStdout);
    ok = SetStdHandle(STD_INPUT_HANDLE, hStdin);

    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    ok = GetConsoleScreenBufferInfo(hStdout, &coninfo);
    coninfo.dwSize.Y = 300;  // set the screen buffer to be big enough to let us scroll text
        ok = SetConsoleScreenBufferSize(hStdout, coninfo.dwSize);

#endif

    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
    // point to console as well
    std::ios::sync_with_stdio();

    //Clear the error state for each of the C++ standard stream objects.
    std::wclog.clear();
    std::clog.clear();
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class wxImagePanel : public wxPanel
{

    private:
        
        int tamX;
        int tamY;
        int w;
        int h;

        wxBitmap resized;
        wxImage image;
        wxPanel* padre;


    public:

        /*wxImagePanel(wxPanel*, wxString, wxBitmapType, int, int);
        wxBitmap getImage();
        void setTamanyo(int, int);*/

        void paintEvent(wxPaintEvent& event);
        void paintNow();
        void OnSize(wxSizeEvent& event);
        void render(wxDC& dc);


    wxImagePanel(wxPanel* parent, wxString file, wxBitmapType format, int xTam, int yTam) : wxPanel(parent)
    {
        image.LoadFile(file, format);
        w = -1;
        h = -1;
        tamX = xTam;
        tamY = yTam;
        this->padre = parent;
        this->SetSize((wxSize(tamX, tamY)));
    }


    wxBitmap getImage()
    {
        return image;
    }


    void setTamanyo(int x, int y)
    {
        tamX = x;
        tamY = y;
        this->SetSize((wxSize(tamX, tamY)));
    }


    wxDECLARE_EVENT_TABLE();
};


    BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
    EVT_PAINT(wxImagePanel::paintEvent)
    EVT_SIZE(wxImagePanel::OnSize)
    END_EVENT_TABLE()


    void wxImagePanel::paintEvent(wxPaintEvent& evt)
    {
        // depending on your system you may need to look at double-buffered dcs
        wxPaintDC dc(this);
        render(dc);
    }


    /*
        * Alternatively, you can use a clientDC to paint on the panel
        * at any time. Using this generally does not free you from
        * catching paint events, since it is possible that e.g. the window
        * manager throws away your drawing when the window comes to the
        * background, and expects you will redraw it when the window comes
        * back (by sending a paint event).
        */
    void wxImagePanel::paintNow()
    {
        // depending on your system you may need to look at double-buffered dcs
        wxClientDC dc(this);
        render(dc);
    }


    /*
        * Here we do the actual rendering. I put it in a separate
        * method so that it can work no matter what type of DC
        * (e.g. wxPaintDC or wxClientDC) is used.
        */
    void wxImagePanel::render(wxDC& dc)
    {
        int neww, newh;
        dc.GetSize(&neww, &newh);

        if (neww != w || newh != h)
        {
            resized = wxBitmap(image.Scale(neww, newh));
            w = neww;
            h = newh;
            dc.DrawBitmap(resized, 0, 0, false);
        }
        else
        {
            dc.DrawBitmap(resized, 0, 0, false);
        }
    }


    /*
        * Here we call refresh to tell the panel to draw itself again.
        * So when the user resizes the image panel the image should be resized too.
        */
    void wxImagePanel::OnSize(wxSizeEvent& event)
    {
        Refresh();
        //skip the event.
        event.Skip();
    };


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class wxMovableImagePanel : public wxPanel
{

    private:

        int nPanel;
        int nPadre;
        int posX;
        int posY;
        int posXProvisional;
        int posYProvisional;
        int tamX;
        int tamY;
        bool dragging;
        bool global;
        int w;
        int h;
        
        wxBitmap bitMap;
        wxBitmap resized;
        wxImage image;
        wxPanel* padre;
        wxPoint posicionPadre;
        wxPropertyGrid* gridPropiedades;


    public:

        /*wxMovableImagePanel(wxPanel*, wxString, wxBitmapType, int, int, bool, int, int);
        wxBitmap getImage();
        void onMouseDown(wxMouseEvent& evt);
        void onLeftDDown(wxMouseEvent& evt);
        void onRightDown(wxMouseEvent& evt);
        void onMouseUp(wxMouseEvent& evt);
        void onMove(wxMouseEvent& evt);
        void setTamanyo(int, int);
        wxPropertyGrid* getProperties();
        void setProperties(wxPropertyGrid*);*/

        void paintEvent(wxPaintEvent& evt);
        void paintNow();
        void OnSize(wxSizeEvent& event);
        void render(wxDC& dc);


        wxMovableImagePanel(wxPanel* parent, wxString file, wxBitmapType format, int xTam, int yTam, bool global, int numPanel, int numPadre) : wxPanel(parent)
        {
            this->padre = parent;

            nPanel = numPanel;
            nPadre = numPadre;

            w = -1;
            h = -1;
            tamX = xTam;
            tamY = yTam;

            posicionPadre = padre->GetPosition();
            posX = posicionPadre.x;
            posX = posicionPadre.y;

            posXProvisional = posX;
            posYProvisional = posY;

            this->global = global;

            this->SetSize((wxSize(tamX/escala, tamY/escala)));

            bitMap.LoadFile(file, format);
            image = bitMap.ConvertToImage();
        }


        wxBitmap getImage()
        {
            return image;
        }


        void onMouseDown(wxMouseEvent& evt)
        {
            CaptureMouse();
            posX = evt.GetX();
            posY = evt.GetY();
            dragging = true;

            //evt.Veto();
        }


        void onLeftDDown(wxMouseEvent& evt)
        {
            wxCommandEvent eventoClick(CLICK_PANEL, nPanel);
            eventoClick.SetClientData(new int(nPanel));
            ProcessWindowEvent(eventoClick);
        }


        void onRightDown(wxMouseEvent& evt)
        {
            wxCommandEvent eventoBorra(BORRA_PANEL, nPanel);
            eventoBorra.SetClientData(new int(nPanel));
            ProcessWindowEvent(eventoBorra);
        }


        void onMouseUp(wxMouseEvent& evt)
        {

            if (global)
            {
                wxCommandEvent eventoGlobalUp(PANEL_UP, nPadre);
                eventoGlobalUp.SetInt(nPanel);
                eventoGlobalUp.SetClientData(new int(nPadre));
                ProcessWindowEvent(eventoGlobalUp);
            }

            ReleaseMouse();
            dragging = false;
            posX = posXProvisional;
            posY = posYProvisional;
        }


        void onMove(wxMouseEvent& evt)
        {
            if (dragging)
            {
                wxPoint mouseOnScreen = wxGetMousePosition();
                int newposX = mouseOnScreen.x - posX;
                int newposY = mouseOnScreen.y - posY;

                wxSize sizeVista = padre->GetParent()->GetSize();
                int vistaX = sizeVista.GetWidth();
                int vistaY = sizeVista.GetHeight();


                if (global == false)
                {
                    if (newposX > vistaX - 34)
                    {
                        newposX = vistaX - 34;
                        padre->Move(padre->GetParent()->ScreenToClient(wxPoint(newposX, newposY)));

                    }
                    else if (newposX < 34)
                    {
                        newposX = 34;
                        padre->Move(padre->GetParent()->ScreenToClient(wxPoint(newposX, newposY)));
                    }
                    else if (newposY > vistaY)
                    {
                        newposY = vistaY;
                        padre->Move(padre->GetParent()->ScreenToClient(wxPoint(newposX, newposY)));
                    }
                    else if (newposY < 86)
                    {
                        newposY = 86;
                        padre->Move(padre->GetParent()->ScreenToClient(wxPoint(newposX, newposY)));
                    }
                    else
                    {
                        padre->Move(padre->GetParent()->ScreenToClient(wxPoint(newposX, newposY)));
                    }

                    this->gridPropiedades->GetProperty("Posición X")->SetValue(newposX - 34);
                    this->gridPropiedades->GetProperty("Posición Y")->SetValue(newposY - 86);
                }
                else
                {
                    padre->Move(padre->GetParent()->ScreenToClient(wxPoint(newposX, newposY)));;
                }

                posXProvisional = newposX - 34;
                posYProvisional = newposY - 86;

            }
        }


        void setTamanyo(int x, int y)
        {
            tamX = x;
            tamY = y;

            this->SetSize((wxSize(tamX/escala, tamY/escala)));

            this->Update();
            this->Refresh();

        }


        wxPropertyGrid* getProperties()
        {
            return this->gridPropiedades;
        }


        void setProperties(wxPropertyGrid* propiedades)
        {
            this->gridPropiedades = propiedades;
            Refresh();
        }


        wxDECLARE_EVENT_TABLE();
};


    BEGIN_EVENT_TABLE(wxMovableImagePanel, wxPanel)
    EVT_LEFT_DOWN(wxMovableImagePanel::onMouseDown)
    EVT_RIGHT_DOWN(wxMovableImagePanel::onRightDown)
    EVT_LEFT_DCLICK(wxMovableImagePanel::onLeftDDown)
    EVT_LEFT_UP(wxMovableImagePanel::onMouseUp)
    EVT_MOTION(wxMovableImagePanel::onMove)
    EVT_PAINT(wxMovableImagePanel::paintEvent)
    EVT_SIZE(wxMovableImagePanel::OnSize)
    END_EVENT_TABLE()


    void wxMovableImagePanel::paintEvent(wxPaintEvent& evt)
    {
        // depending on your system you may need to look at double-buffered dcs
        wxPaintDC dc(this);
        render(dc);
    }


    /* Alternatively, you can use a clientDC to paint on the panel
     * at any time. Using this generally does not free you from
     * catching paint events, since it is possible that e.g. the window
     * manager throws away your drawing when the window comes to the
     * background, and expects you will redraw it when the window comes
     * back (by sending a paint event).
     */
    void wxMovableImagePanel::paintNow()
    {
        // depending on your system you may need to look at double-buffered dcs
        wxClientDC dc(this);
        render(dc);
    }


    /* Here we do the actual rendering. I put it in a separate
     * method so that it can work no matter what type of DC
     * (e.g. wxPaintDC or wxClientDC) is used.
     */
    void wxMovableImagePanel::render(wxDC& dc)
    {
        int neww, newh;
        dc.GetSize(&neww, &newh);

        if (neww != w || newh != h)
        {
            resized = wxBitmap(image.Scale(neww, newh));
            w = neww;
            h = newh;
            dc.DrawBitmap(resized, 0, 0, false);
        }
        else 
        {
            dc.DrawBitmap(resized, 0, 0, false);
        }
    }


    /* Here we call refresh to tell the panel to draw itself again.
     * So when the user resizes the image panel the image should be resized too.
     */
    void wxMovableImagePanel::OnSize(wxSizeEvent& event)
    {
        Refresh();
        //skip the event.
        event.Skip();
    };



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class movablePanel : public wxPanel
{

    private:

        int nPanel;
        int nPadre;
        int posX;
        int posY;
        int posXProvisional;
        int posYProvisional;
        int tamX;
        int tamY;
        int r;
        int g;
        int b;
        bool dragging;
        bool global;
        bool selfActivo;
        bool hayImagen = false;

        wxMovableImagePanel* imagePanel;
        wxPanel* padre;
        wxPanel* self;
        wxPropertyGrid* gridPropiedades;
        wxSize tamVista;
        wxString urlImagen;
        wxString nombrePanel;
        

    public:

        /*movablePanel(wxPanel*, int, int, wxString, bool);
        movablePanel(wxImagePanel*, int, int, wxString);
        void ponImagen(wxString, wxBitmapType);
        void quitaImagen();
        void Borrame();
        void onMouseDown(wxMouseEvent& evt);
        void onLeftDDown(wxMouseEvent& evt);
        void onRightDown(wxMouseEvent& evt);
        void onMouseUp(wxMouseEvent& evt);
        void onMove(wxMouseEvent& evt);
        int getNumeroPanel();
        void setNumeroPanel(int);
        int getTamX();
        void setTamX(int);
        int getTamY();
        void setTamY(int);
        int getPosX();
        void setPosX(int);
        int getPosY();
        void setPosY(int);
        int getR();
        void setR(int);
        int getG();
        void setG(int);
        int getB();
        void setB(int);
        wxString getNombre();
        void setNombre(wxString);
        bool getHayImagen();
        void setHayImagen(bool);
        wxString getURLImagen();
        void setURLImagen(wxString);
        void setTamVista(wxSize);
        wxPropertyGrid* getProperties();
        void setProperties(wxPropertyGrid*);
        wxPanel getParent();
        void setParent(wxPanel*);*/


        movablePanel(wxPanel* parent, int numeroPanel, int numeroPadre, wxString name, bool global) : wxPanel(parent, wxID_ANY)
        {
            tamX = 100;
            tamY = 100;

            posX = 0;
            posY = 0;

            posXProvisional = 0;
            posYProvisional = 0;


            r = 0;
            g = 250;
            b = 0;

            nPanel = numeroPanel;
            nPadre = numeroPadre;

            nombrePanel = name;

            padre = parent;

            this->global = global;

            movablePanel::padre = parent;
            self = new wxPanel(parent, wxID_ANY);

            this->SetSize((wxSize(tamX/escala, tamY/escala)));
            self->SetSize((wxSize(tamX/escala, tamY/escala)));

            this->SetPosition(wxPoint(posX, posY));
            self->SetPosition(wxPoint(posX, posY));

            self->SetBackgroundColour(wxColour(r, g, b));

            tamVista = padre->GetSize();

            urlImagen = "NO HAY";

            dragging = false;
            selfActivo = true;
        }


        movablePanel(wxImagePanel* parent, int numeroPanel, int numeroPadre, wxString name) : wxPanel(parent, wxID_ANY)
        {
            tamX = 100;
            tamY = 100;

            posX = 0;
            posY = 0;

            posXProvisional = 0;
            posYProvisional = 0;

            r = 0;
            g = 250;
            b = 0;

            nPanel = numeroPanel;
            nPadre = numeroPadre;

            nombrePanel = name;

            padre = parent;

            movablePanel::padre = parent;
            self = new wxPanel(parent, wxID_ANY);

            this->SetSize((wxSize(tamX / escala, tamY / escala)));
            self->SetSize((wxSize(tamX / escala, tamY / escala)));

            this->SetPosition(wxPoint(posX, posY));
            self->SetPosition(wxPoint(posX, posY));

            self->SetBackgroundColour(wxColour(r, g, b));

            tamVista = padre->GetSize();

            urlImagen = "NO HAY";

            dragging = false;
            selfActivo = true;
        }


        void ponImagen(wxString file, wxBitmapType format)
        {
            if (selfActivo)
            {
                self->Destroy();
                selfActivo = false;

                Update();
                Refresh();
            }

            urlImagen = file;
            imagePanel = new wxMovableImagePanel(this, file, format, tamX, tamY, global, nPanel, nPadre);
            imagePanel->setProperties(gridPropiedades);
            hayImagen = true;

            imagePanel->Update();
            imagePanel->Refresh();
            Update();
            Refresh();
        }


        void quitaImagen()
        {
            self = new wxPanel(padre, wxID_ANY);
            selfActivo = true;

            self->SetSize((wxSize(tamX / escala, tamY / escala)));
            self->SetPosition(wxPoint(posX, posY));
            self->SetBackgroundColour(wxColour(r, g, b));
            self->Show();

            Update();
            Refresh();

            this->imagePanel->Hide();

            hayImagen = false;

            Update();
            Refresh();
        }


        void Borrame()
        {
            if (selfActivo)
            {
                self->Destroy();
            }

            Update();
            Refresh();
            this->Destroy();
        }


        void onMouseDown(wxMouseEvent& evt)
        {
            CaptureMouse();
            posX = evt.GetX();
            posY = evt.GetY();

            dragging = true;
        }


        void onLeftDDown(wxMouseEvent& evt)
        {
            wxCommandEvent eventoClick(CLICK_PANEL, nPanel);
            eventoClick.SetClientData(new int(nPanel));
            ProcessWindowEvent(eventoClick);
        }


        void onRightDown(wxMouseEvent& evt)
        {
            wxCommandEvent eventoBorra(BORRA_PANEL, nPanel);
            eventoBorra.SetClientData(new int(nPanel));
            ProcessWindowEvent(eventoBorra);
        }


        void onMouseUp(wxMouseEvent& evt)
        {
            if (global)
            {
                wxCommandEvent eventoGlobalUp(PANEL_UP, nPadre);
                eventoGlobalUp.SetInt(nPanel);
                eventoGlobalUp.SetClientData(new int(nPadre));
                ProcessWindowEvent(eventoGlobalUp);
            }

            ReleaseMouse();
            dragging = false;
            posX = posXProvisional;
            posY = posYProvisional;
        }


        void onMove(wxMouseEvent& evt)
        {
            if (dragging)
            {
                wxPoint mouseOnScreen = wxGetMousePosition();

                int newposX = mouseOnScreen.x - posX;
                int newposY = mouseOnScreen.y - posY;

                int vistaX = tamVistaImagen.x;
                int vistaY = tamVistaImagen.y;

                if (global == false)
                {
                    if (newposX > vistaX - 34)
                    {
                        newposX = vistaX - 34;
                        this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                        if (selfActivo)
                        {
                            self->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                        }

                    }
                    else if (newposX < 34)
                    {
                        newposX = 34;
                        this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                        if (selfActivo)
                        {
                            self->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                        }
                    }
                    else if (newposY > vistaY)
                    {
                        newposY = vistaY;
                        this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                        if (selfActivo)
                        {
                            self->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                        }
                    }
                    else if (newposY < 86)
                    {
                        newposY = 86;
                        this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                        if (selfActivo)
                        {
                            self->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                        }
                    }
                    else
                    {
                        this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                        if (selfActivo)
                        {
                            self->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                        }
                    }

                    this->gridPropiedades->GetProperty("Posición X")->SetValue(newposX - 34);
                    this->gridPropiedades->GetProperty("Posición Y")->SetValue(newposY - 86);
                }
                else
                {
                    this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                    if (selfActivo)
                    {
                        self->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                    }

                    //cout << "papaTamX: " << padre->GetSize().x << endl;
                    //cout << "papaTamY: " << padre->GetSize().y << endl;

                    //cout << "screenPosX: " << this->GetScreenPosition().x << endl;
                    //cout << "screenPosY: " << this->GetScreenPosition().y << endl;
                }

                posXProvisional = newposX - 34;
                posYProvisional = newposY - 86;

            }
        }


        int getNumeroPanel()
        {
            return nPanel;
        }


        void setNumeroPanel(int numeroProvisional)
        {
            nPanel = numeroProvisional;
        }


        int getTamX()
        {
            return tamX;
        }


        void setTamX(int tamanyoX)
        {
            tamX = tamanyoX;
            this->SetSize((wxSize(tamX / escala, tamY / escala)));

            if (hayImagen)
            {
                imagePanel->setTamanyo(tamX, tamY);
                imagePanel->Update();
                imagePanel->Refresh();
            }
            else
            {
                self->SetSize((wxSize(tamX / escala, tamY / escala)));
            }

            Update();
            Refresh();
        }


        int getTamY()
        {
            return tamY;
        }


        void setTamY(int tamanyoY)
        {
            tamY = tamanyoY;
            this->SetSize((wxSize(tamX / escala, tamY / escala)));

            if (hayImagen)
            {
                imagePanel->setTamanyo(tamX, tamY);
                imagePanel->Update();
                imagePanel->Refresh();
            }
            else
            {
                self->SetSize((wxSize(tamX / escala, tamY / escala)));
            }

            Update();
            Refresh();
        }


        int getPosX()
        {
            return posX;
        }


        void setPosX(int posicionX)
        {
            posX = posicionX;
            this->SetPosition(wxPoint(posX, posY));

            if (selfActivo)
            {
                self->SetPosition(wxPoint(posX, posY));
            }

            Update();
            Refresh();
        }


        int getPosY()
        {
            return posY;
        }


        void setPosY(int posicionY)
        {
            posY = posicionY;
            this->SetPosition(wxPoint(posX, posY));

            if (selfActivo)
            {
                self->SetPosition(wxPoint(posX, posY));
            }

            Update();
            Refresh();
        }


        int getR()
        {
            return r;
        }


        void setR(int colorR)
        {
            r = colorR;

            if (hayImagen == false)
            {
                self->SetBackgroundColour(wxColour(r, g, b));
            }

            Update();
            Refresh();
        }


        int getG()
        {
            return g;
        }


        void setG(int colorG)
        {
            g = colorG;

            if (hayImagen == false)
            {
                self->SetBackgroundColour(wxColour(r, g, b));
            }

            Update();
            Refresh();
        }


        int getB()
        {
            return b;
        }


        void setB(int colorB)
        {
            b = colorB;

            if (hayImagen == false)
            {
                self->SetBackgroundColour(wxColour(r, g, b));
            }

            Update();
            Refresh();
        }


        wxString getNombre()
        {
            return nombrePanel;
        }


        void setNombre(wxString nombreSet)
        {
            nombrePanel = nombreSet;

            Update();
            Refresh();
        }


        bool getHayImagen()
        {
            return hayImagen;
        }


        void setHayImagen(bool isImg)
        {
            hayImagen = isImg;

            Update();
            Refresh();
        }


        wxString getURLImagen()
        {
            return urlImagen;
        }


        void setURLImagen(wxString nombreImg)
        {
            urlImagen = nombreImg;

            Update();
            Refresh();
        }


        void setTamVista(wxSize tam)
        {
            tamVista = tam;
        }


        wxPropertyGrid* getProperties()
        {
            return this->gridPropiedades;
        }


        void setProperties(wxPropertyGrid* propiedades)
        {
            this->gridPropiedades = propiedades;
        }


        wxPanel getParent()
        {
            return padre;
        }


        void setParent(wxPanel* papa2)
        {
            this->padre = papa2;

            Update();
            Refresh();
        }


        DECLARE_EVENT_TABLE()
};

    BEGIN_EVENT_TABLE(movablePanel, wxPanel)
    EVT_LEFT_DOWN(movablePanel::onMouseDown)
    EVT_RIGHT_DOWN(movablePanel::onRightDown)
    EVT_LEFT_DCLICK(movablePanel::onLeftDDown)
    EVT_LEFT_UP(movablePanel::onMouseUp)
    EVT_MOTION(movablePanel::onMove)
    END_EVENT_TABLE()

        
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class movableText : public wxPanel
{

    private:

        int nPanel;
        int nPadre;
        int posX;
        int posY;
        int posXProvisional;
        int posYProvisional;
        int tamFuente;
        bool dragging;
        bool global;

        wxFont fuenteTexto;
        wxPanel* padre;
        wxPropertyGrid* gridPropiedades;
        wxSize tamVista;
        wxStaticText* texto;
        wxString nombrePanel;
        wxString valorTexto;


    public:

        /*movableText(wxPanel*, int, int, wxString, bool);
        movableText(wxImagePanel*, int, int, wxString, bool);
        void Borrame();
        void onMouseDown(wxMouseEvent& evt);
        void onLeftDDown(wxMouseEvent& evt);
        void onRightDown(wxMouseEvent& evt);
        void onMouseUp(wxMouseEvent& evt);
        void onMove(wxMouseEvent& evt);
        int getNumeroPanel();
        void setNumeroPanel(int);
        int getPosX();
        void setPosX(int);
        int getPosY();
        void setPosY(int);
        wxString getNombre();
        void setNombre(wxString);
        wxFont getFuente();
        void setFuente(wxFont);
        wxStaticText* getTexto();
        void setTexto(wxStaticText*);
        wxString getValorTexto();
        void setValorTexto(wxString);
        void Refresca();
        void setTamVista(wxSize);
        wxPropertyGrid* getProperties();
        void setProperties(wxPropertyGrid*);
        wxPanel getParent();
        void setParent(wxPanel*);*/




        movableText(wxPanel* parent, int numeroPanel, int numeroPadre, wxString name, bool global) : wxPanel(parent, wxID_ANY)
        {
            posX = 0;
            posY = 0;

            posXProvisional = 0;
            posYProvisional = 0;

            nPadre = numeroPadre;
            nPanel = numeroPanel;

            nombrePanel = name;

            padre = parent;

            this->global = global;

            movableText::padre = parent;

            this->SetPosition(wxPoint(posX, posY));

            tamVista = padre->GetSize();

            tamFuente = 10;
            valorTexto = "Lorem Ipsum";
            fuenteTexto = wxFont(tamFuente, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);


            texto = new wxStaticText(parent, wxID_ANY, valorTexto);

            texto->SetFont(wxFont(tamFuente, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

            this->SetSize(texto->GetSize());

            texto->Update();
            texto->Refresh();
            Refresh();

            dragging = false;
        }


        movableText(wxImagePanel* parent, int numeroPanel, int numeroPadre, wxString name, bool global) : wxPanel(parent, wxID_ANY)
        {

            posX = 0;
            posY = 0;

            posXProvisional = 0;
            posYProvisional = 0;

            nPadre = numeroPadre;
            nPanel = numeroPanel;

            nombrePanel = name;

            padre = parent;

            this->global = global;

            movableText::padre = parent;

            this->SetPosition(wxPoint(posX, posY));

            tamVista = padre->GetSize();

            tamFuente = 10;
            valorTexto = "Lorem Ipsum";
            fuenteTexto = wxFont(tamFuente, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

            texto = new wxStaticText(parent, wxID_ANY, valorTexto);

            texto->SetFont(wxFont(tamFuente, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

            this->SetSize(texto->GetSize());

            texto->Update();
            texto->Refresh();
            Refresh();

            dragging = false;
        }


        void Borrame()
        {
            texto->Destroy();

            Update();
            Refresh();
            this->Destroy();
        }


        void onMouseDown(wxMouseEvent& evt)
        {
            CaptureMouse();
            posX = evt.GetX();
            posY = evt.GetY();

            dragging = true;

            texto->Refresh();

            //evt.Veto();
        }


        void onLeftDDown(wxMouseEvent& evt)
        {
            wxCommandEvent eventoClick(CLICK_TEXTO, nPanel);
            eventoClick.SetClientData(new int(nPanel));
            ProcessWindowEvent(eventoClick);
        }


        void onRightDown(wxMouseEvent& evt)
        {
            wxCommandEvent eventoBorra(BORRA_TEXTO, nPanel);
            eventoBorra.SetClientData(new int(nPanel));
            ProcessWindowEvent(eventoBorra);
        }


        void onMouseUp(wxMouseEvent& evt)
        {
            if (global)
            {
                wxCommandEvent eventoGlobalUp(TEXT_UP, nPadre);
                eventoGlobalUp.SetInt(nPanel);
                eventoGlobalUp.SetClientData(new int(nPadre));
                ProcessWindowEvent(eventoGlobalUp);
            }

            ReleaseMouse();
            dragging = false;
            posX = posXProvisional;
            posY = posYProvisional;

            texto->Update();
            texto->Refresh();
        }


        void onMove(wxMouseEvent& evt)
        {
            if (dragging)
            {
                wxPoint mouseOnScreen = wxGetMousePosition();
                int newposX = mouseOnScreen.x - posX;
                int newposY = mouseOnScreen.y - posY;

                wxSize sizeVista = padre->GetSize();
                int vistaX = sizeVista.GetWidth();
                int vistaY = sizeVista.GetHeight();

                int tamX = texto->GetSize().x;
                int tamY = texto->GetSize().y;


                if (global == false)
                {
                    if (newposX > (vistaX - 20))
                    {
                        newposX = vistaX - 20;
                        this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                        texto->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                        texto->Update();
                        texto->Refresh();
                    }
                    else if (newposX < 75)
                    {
                        newposX = 0 + 75;
                        this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                        texto->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                        texto->Update();
                        texto->Refresh();
                    }
                    else if (newposY > vistaY + 40)
                    {
                        newposY = vistaY + 40;
                        this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                        texto->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                        texto->Update();
                        texto->Refresh();
                    }
                    else if (newposY < tamY + 34)
                    {
                        newposY = tamY + 34;
                        this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                        texto->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                        texto->Update();
                        texto->Refresh();
                    }
                    else
                    {
                        this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                        texto->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                        texto->Update();
                        texto->Refresh();
                    }

                    this->gridPropiedades->GetProperty("Posición X")->SetValue(newposX - 34);
                    this->gridPropiedades->GetProperty("Posición Y")->SetValue(newposY - 86);
                }
                else
                {
                    this->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));
                    texto->Move(padre->ScreenToClient(wxPoint(newposX, newposY)));

                    texto->Update();
                    texto->Refresh();
                }

                posXProvisional = newposX - 34;
                posYProvisional = newposY - 86;

                texto->Update();
                texto->Refresh();
            }
        }


        int getNumeroPanel()
        {
            return nPanel;
        }


        void setNumeroPanel(int numeroProvisional)
        {
            nPanel = numeroProvisional;

            texto->Update();
            texto->Refresh();
            Refresh();
        }


        int getPosX()
        {
            return posX;
        }


        void setPosX(int posicionX)
        {
            posX = posicionX;
            this->SetPosition(wxPoint(posX, posY));
            texto->SetPosition(wxPoint(posX, posY));

            texto->Update();
            texto->Refresh();
            Refresh();
        }


        int getPosY()
        {
            return posY;
        }


        void setPosY(int posicionY)
        {
            posY = posicionY;
            this->SetPosition(wxPoint(posX, posY));
            texto->SetPosition(wxPoint(posX, posY));

            texto->Update();
            texto->Refresh();
            Refresh();
        }


        wxString getNombre()
        {
            return nombrePanel;
        }


        void setNombre(wxString nombreSet)
        {
            nombrePanel = nombreSet;

            texto->Update();
            texto->Refresh();
            Refresh();
        }


        wxFont getFuente()
        {
            return fuenteTexto;
        }


        void setFuente(wxFont font)
        {
            fuenteTexto = font;
            texto->SetFont(font);

            this->SetSize(texto->GetSize());

            texto->Update();
            texto->Refresh();
            Refresh();
        }


        wxStaticText* getTexto()
        {
            return texto;
        }


        void setTexto(wxStaticText* text)
        {
            texto = text;

            this->SetSize(texto->GetSize());

            texto->Update();
            texto->Refresh();
            Refresh();
        }


        wxString getValorTexto()
        {
            return valorTexto;
        }


        void setValorTexto(wxString valor)
        {
            valorTexto = valor;
            texto->SetLabel(valorTexto);

            this->SetSize(texto->GetSize());

            texto->Update();
            texto->Refresh();
            Refresh();
        }


        void Refresca()
        {
            this->Update();
            this->Refresh();

            texto->Update();
            texto->Refresh();
            Refresh();
        }


        void setTamVista(wxSize tam)
        {
            tamVista = tam;
        }


        wxPropertyGrid* getProperties()
        {
            return this->gridPropiedades;
        }


        void setProperties(wxPropertyGrid* propiedades)
        {
            this->gridPropiedades = propiedades;

            texto->Update();
            texto->Refresh();
            Refresh();
        }


        wxPanel getParent()
        {
            return padre;
        }


        void setParent(wxPanel* papa2)
        {
            this->padre = papa2;

            Update();
            Refresh();
        }


        wxDECLARE_EVENT_TABLE();
};


    BEGIN_EVENT_TABLE(movableText, wxPanel)
        EVT_LEFT_DOWN(movableText::onMouseDown)
        EVT_RIGHT_DOWN(movableText::onRightDown)
        EVT_LEFT_DCLICK(movableText::onLeftDDown)
        EVT_LEFT_UP(movableText::onMouseUp)
        EVT_MOTION(movableText::onMove)
    END_EVENT_TABLE()


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ventanaHUD : public wxAuiNotebook
{

    public:

        ventanaHUD(wxFrame*, wxAuiManager*, wxAuiNotebook*, wxString, int, wxSize, wxSize);
        ventanaHUD(wxFrame*, wxAuiManager*, wxAuiNotebook*, wxString, int, bool, wxString, wxSize, wxSize);

        wxString getNombreVentana();
        wxSize getTamVentana();
        int getNumVentana();
        wxString getNombrePropiedades();
        wxString getNombreArbol();
        wxString getURLImagenVista();
        bool getImagenVista();
        int getContadorPaneles();
        int getContadorTextos();
        int getContadorElementosJerarquia();
        wxListCtrl* getArbolJerarquia();

        void clickLista(wxListEvent& evt);
        void borraListaItem(wxListEvent& evt);
        void borraItem(wxString, int);

        void cambiaFondoPantalla(wxString);
        void quitaFondoPantalla();

        void OnPropertyGridChangePanel(wxPropertyGridEvent& evt);
        void OnPropertyGridChangeTexto(wxPropertyGridEvent& evt);
        void OnPropertyGridChanging(wxPropertyGridEvent& evt);

        void creaPanel(bool, wxString, int, int, int, int, int, int, int, int, wxString, bool);
        void creaTexto(bool, wxString, int, int, int, wxString);

        void ocultaPanelesInformacion(bool oculta);

        void guardaArchivoTemporal();
        void guardaVentana(wxString);
        void guardaTodoVentana(wxString);
        
        void cargaArchivoTemporal();
        void cargaVentana(wxString);

        void borraPanel(int);
        void borraTexto(int);

        void eventoBorraPanel(wxCommandEvent& evt);
        void eventoBorraTexto(wxCommandEvent& evt);

        void seleccionaElementoLista(int);

        void eventoClickPanel(wxCommandEvent& evt);
        void eventoClickTexto(wxCommandEvent& evt);

        //void eventoMuevePanel(wxCommandEvent& evt);
        //void eventoMueveTexto(wxCommandEvent& evt);


        movablePanel* getPanel(int);
        movableText* getText(int);

        wxVector<movablePanel*> panelesCreados;
        wxVector<wxPropertyGrid*> propiedadesPanelesCreados;
        wxVector<movableText*> textosCreados;
        wxVector<wxPropertyGrid*> propiedadesTextosCreados;


    private:

        int contadorNumerosPaneles = 1;
        int contadorPaneles = 0;
        int contadorNumerosTextos = 1;
        int contadorTextos = 0;
        int contadorPropiedadesPaneles = 0;
        int contadorPropiedadesTextos = 0;
        int contadorElementosJerarquia = 0;
        int nVentana;
        bool imagenVista;

        wxAuiManager* m_mgr;
        wxFrame* padre;
        wxListCtrl* arbolJerarquia;
        movablePanel* panelProvisional;
        movablePanel* panelImagenProvisional;
        movableText* textoProvisional;
        wxPanel* panelVista;
        wxPanel* panelVistaImagen;
        wxPanel* panelInspector;
        wxPanel* panelJerarquia;
        wxPropertyGrid* gridPropiedadesPanel;
        wxPropertyGrid* gridPropiedadesTexto;
        wxImagePanel* imagenGeneral;
        wxSize sizeVentana;
        wxSize tamNotebook;
        wxSize tamVentana;
        wxString CurrentDocPath;
        wxString nombreVentana;
        wxString nombreArbol;
        wxString nombrePropiedades;
        wxString urlImagenVista = "NO HAY";
        wxTextFile file;
        

        wxDECLARE_EVENT_TABLE();
};


    wxBEGIN_EVENT_TABLE(ventanaHUD, wxAuiNotebook)
    wxEND_EVENT_TABLE()


    ventanaHUD::ventanaHUD(wxFrame* pare, wxAuiManager* manager, wxAuiNotebook* ctrl, wxString name, int nWindow, wxSize sizeNotebook, wxSize sizeVentana)
    {

        m_mgr = manager;
        padre = pare;
        nVentana = nWindow;

        nombreVentana = name;
    
        tamNotebook = sizeNotebook;
        tamVentana = sizeVentana;

        imagenVista = false;

        panelVista = new wxPanel(padre, wxID_ANY);
        panelVista->SetBackgroundColour(wxColour(217, 37, 52));
        panelVista->SetSize(tamNotebook);

        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, FromDIP(wxSize(16, 16)));

        wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);

        panelVistaImagen = new wxPanel(panelVista, wxID_ANY, wxDefaultPosition, sizeVentana*0.75, 0);
        panelVistaImagen->SetBackgroundColour(wxColor(0, 255, 255));

        verticalSizer->AddStretchSpacer(1);
        verticalSizer->Add(panelVistaImagen, 0);
        verticalSizer->AddStretchSpacer(1);

        horizontalSizer->AddStretchSpacer(1);
        horizontalSizer->Add(verticalSizer, 0, wxALIGN_CENTER_VERTICAL);
        horizontalSizer->AddStretchSpacer(1);

        panelVista->SetSizer(horizontalSizer);
        panelVista->Layout();

        Refresh();

        ctrl->AddPage(panelVista, nombreVentana, false, page_bmp);


        // Añade el panel con el Property Grid.
        panelInspector = new wxPanel(padre, wxID_ANY);
        panelInspector->SetSize(tamInspector);
        panelInspector->SetBackgroundColour(wxColour(45, 75, 166));
    
        nombrePropiedades = "Propiedades ";
        nombrePropiedades << nVentana + 1;
        //std::cout << nombrePropiedades << endl;
        m_mgr->AddPane(panelInspector, wxAuiPaneInfo().Name(nombrePropiedades).Caption(nombrePropiedades).Right().Layer(1).Position(1).CloseButton(false).MaximizeButton(false).MinSize(panelInspector->GetSize()).MaxSize(panelInspector->GetSize()).BestSize(panelInspector->GetSize()));


        // Añade el panel con el Árbol de Elementos.
        panelJerarquia = new wxPanel(padre, wxID_ANY);
        panelJerarquia->SetSize(tamJerarquia);
        panelJerarquia->SetBackgroundColour(wxColour(242, 88, 53));
        arbolJerarquia = new wxListCtrl(panelJerarquia, wxID_ANY, wxDefaultPosition, wxSize(300, 660));
        arbolJerarquia->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(ventanaHUD::clickLista), NULL, this);
        arbolJerarquia->Connect(wxEVT_LIST_ITEM_RIGHT_CLICK, wxListEventHandler(ventanaHUD::borraListaItem), NULL, this);

        nombreArbol = "Árbol Elementos ";
        nombreArbol << nVentana + 1;
        //std::cout << nombreArbol << endl;
        m_mgr->AddPane(panelJerarquia, wxAuiPaneInfo().Name(nombreArbol).Caption(nombreArbol).Right().Layer(1).Position(2).CloseButton(false).MaximizeButton(false).MinSize(panelJerarquia->GetSize()).MaxSize(panelJerarquia->GetSize()).BestSize(panelJerarquia->GetSize()));

    }


    ventanaHUD::ventanaHUD(wxFrame* pare, wxAuiManager* manager, wxAuiNotebook* ctrl, wxString name, int nWindow, bool hayImagenVista, wxString urlImagen, wxSize sizeNotebook, wxSize sizeVentana)
    {

        imagenVista = hayImagenVista;

        m_mgr = manager;
        padre = pare;
        nVentana = nWindow;

        nombreVentana = name;

        tamNotebook = sizeNotebook;
        tamVentana = sizeVentana;

        imagenVista = false;

        panelVista = new wxPanel(padre, wxID_ANY);
        panelVista->SetBackgroundColour(wxColour(217, 37, 52));
        panelVista->SetSize(tamNotebook);

        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, FromDIP(wxSize(16, 16)));

        wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);

        panelVistaImagen = new wxPanel(panelVista, wxID_ANY, wxDefaultPosition, sizeVentana * 0.75, 0);
        panelVistaImagen->SetBackgroundColour(wxColor(0, 255, 255));

        verticalSizer->AddStretchSpacer(1);
        verticalSizer->Add(panelVistaImagen, 0);
        verticalSizer->AddStretchSpacer(1);

        horizontalSizer->AddStretchSpacer(1);
        horizontalSizer->Add(verticalSizer, 0, wxALIGN_CENTER_VERTICAL);
        horizontalSizer->AddStretchSpacer(1);

        panelVista->SetSizer(horizontalSizer);
        panelVista->Layout();

        Refresh();


        ctrl->AddPage(panelVista, nombreVentana, false, page_bmp);


        // Añade el panel con el Property Grid.
        panelInspector = new wxPanel(padre, wxID_ANY);
        panelInspector->SetSize(tamInspector);
        panelInspector->SetBackgroundColour(wxColour(45, 75, 166));

        nombrePropiedades = "Propiedades ";
        nombrePropiedades << nVentana + 1;

        //std::cout << nombrePropiedades << endl;

        m_mgr->AddPane(panelInspector, wxAuiPaneInfo().Name(nombrePropiedades).Caption(nombrePropiedades).Right().Layer(1).Position(1).CloseButton(true).MaximizeButton(true).MinSize(panelInspector->GetSize()).MaxSize(panelInspector->GetSize()).BestSize(panelInspector->GetSize()));


        // Añade el panel con el Árbol de Elementos.
        panelJerarquia = new wxPanel(padre, wxID_ANY);
        panelJerarquia->SetSize(tamJerarquia);
        panelJerarquia->SetBackgroundColour(wxColour(242, 88, 53));
        arbolJerarquia = new wxListCtrl(panelJerarquia, wxID_ANY, wxDefaultPosition, wxSize(300, 660));
        arbolJerarquia->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(ventanaHUD::clickLista), NULL, this);
        arbolJerarquia->Connect(wxEVT_LIST_ITEM_RIGHT_CLICK, wxListEventHandler(ventanaHUD::borraListaItem), NULL, this);

        nombreArbol = "Árbol Elementos ";
        nombreArbol << nVentana + 1;

        //std::cout << nombreArbol << endl;

        m_mgr->AddPane(panelJerarquia, wxAuiPaneInfo().Name(nombreArbol).Caption(nombreArbol).Right().Layer(1).Position(2).CloseButton(true).MaximizeButton(true).MinSize(panelJerarquia->GetSize()).MaxSize(panelJerarquia->GetSize()).BestSize(panelJerarquia->GetSize()));

        if (hayImagenVista)
        {
            cambiaFondoPantalla(urlImagen);
            Update();
            Refresh();
        }
    }


    wxString ventanaHUD::getNombreVentana()
    {
        return nombreVentana;
    }


    wxSize ventanaHUD::getTamVentana()
    {
        return tamVentana;
    }


    int ventanaHUD::getNumVentana()
    {
        return nVentana;
    }


    wxString ventanaHUD::getNombrePropiedades()
    {
        return nombrePropiedades;
    }


    wxString ventanaHUD::getNombreArbol()
    {
        return nombreArbol;
    }


    wxString ventanaHUD::getURLImagenVista()
    {
        return urlImagenVista;
    }


    bool ventanaHUD::getImagenVista()
    {
        return imagenVista;
    }


    int ventanaHUD::getContadorPaneles()
    {
        return contadorPaneles;
    }


    int ventanaHUD::getContadorTextos()
    {
        return contadorTextos;
    }


    int ventanaHUD::getContadorElementosJerarquia()
    {
        return contadorElementosJerarquia;
    }


    wxListCtrl* ventanaHUD::getArbolJerarquia()
    {
        return arbolJerarquia;
    }


    void ventanaHUD::ocultaPanelesInformacion(bool oculta)
    {
        if (oculta)
        {
            m_mgr->GetPane(nombrePropiedades).Show(false);
            m_mgr->GetPane(nombreArbol).Show(false);
        }
        else
        {
            m_mgr->GetPane(nombrePropiedades).Show(true);
            m_mgr->GetPane(nombreArbol).Show(true);
        }

        m_mgr->Update();
    }


    void ventanaHUD::creaPanel(bool carga, wxString nombrePanel, int numPaneles, int posXPanel, int posYPanel, int tamXPanel, int tamYPanel, int colorRPanel, int colorGPanel, int colorBPanel, wxString urlImagenPanel, bool hayImagenPanel)
    {
        cout << contadorPaneles << endl;
        //cout << "PAnel " << contadorPaneles + 1 << endl;

        wxString nombre = "Panel ";
        nombre << contadorPaneles + 1;


        if (imagenVista)
        {
            panelProvisional = new movablePanel(this->imagenGeneral, contadorPaneles, nVentana, nombre, false);
            panelesCreados.insert(panelesCreados.begin() + contadorPaneles, panelProvisional);
        }
        else
        {
            panelProvisional = new movablePanel(this->panelVistaImagen, contadorPaneles, nVentana, nombre, false);
            panelesCreados.insert(panelesCreados.begin() + contadorPaneles, panelProvisional);
        }

        if (carga)
        {
            panelProvisional->setNombre(nombrePanel);
            panelProvisional->setPosX(posXPanel);
            panelProvisional->setPosY(posYPanel);
            panelProvisional->setTamX(tamXPanel);
            panelProvisional->setTamY(tamYPanel);
            panelProvisional->setR(colorRPanel);
            panelProvisional->setG(colorGPanel);
            panelProvisional->setB(colorBPanel);
            panelProvisional->setURLImagen(urlImagenPanel);
            panelProvisional->setHayImagen(hayImagenPanel);

            panelProvisional->Refresh();
        }

        panelProvisional->Bind(BORRA_PANEL, &ventanaHUD::eventoBorraPanel, this);
        panelProvisional->Bind(CLICK_PANEL, &ventanaHUD::eventoClickPanel, this);

        arbolJerarquia->InsertItem(contadorElementosJerarquia, panelProvisional->getNombre());

        padre->Update();
        padre->Refresh();

        gridPropiedadesPanel = new wxPropertyGrid(panelInspector, contadorPropiedadesPaneles, wxDefaultPosition, tamInspector, wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED);
        gridPropiedadesPanel->SetSplitterPosition(tamInspector.x/2);

        gridPropiedadesPanel->Connect(wxEVT_PG_CHANGED, wxPropertyGridEventHandler(ventanaHUD::OnPropertyGridChangePanel), NULL, this);
        gridPropiedadesPanel->Connect(wxEVT_PG_CHANGING, wxPropertyGridEventHandler(ventanaHUD::OnPropertyGridChanging), NULL, this);

        gridPropiedadesPanel->Append(new wxStringProperty("Nombre", wxPG_LABEL, panelProvisional->getNombre()));
        gridPropiedadesPanel->Append(new wxIntProperty("Posición X", wxPG_LABEL, panelProvisional->getPosX()));
        gridPropiedadesPanel->Append(new wxIntProperty("Posición Y", wxPG_LABEL, panelProvisional->getPosY()));
        gridPropiedadesPanel->Append(new wxIntProperty("Tamaño X", wxPG_LABEL, panelProvisional->getTamX()));
        gridPropiedadesPanel->Append(new wxIntProperty("Tamaño Y", wxPG_LABEL, panelProvisional->getTamY()));
        gridPropiedadesPanel->Append(new wxSystemColourProperty("Color", wxPG_LABEL, wxColour(panelProvisional->getR(), panelProvisional->getG(), panelProvisional->getB())));
        gridPropiedadesPanel->Append(new wxImageFileProperty("Imagen", wxPG_LABEL, panelProvisional->getURLImagen()));
        gridPropiedadesPanel->Append(new wxBoolProperty("Imagen sí/no", wxPG_LABEL, panelProvisional->getHayImagen()));

        propiedadesPanelesCreados.insert(propiedadesPanelesCreados.begin() + contadorPropiedadesPaneles, gridPropiedadesPanel);

        for (int i = 0; i < contadorPropiedadesPaneles; i++)
        {
            propiedadesPanelesCreados[i]->Hide();
        }

        propiedadesPanelesCreados[contadorPropiedadesPaneles]->Show();
        panelesCreados[contadorPaneles]->setProperties(gridPropiedadesPanel);

        if (hayImagenPanel)
        {
            panelProvisional->ponImagen(urlImagenPanel, wxBITMAP_TYPE_ANY);
        }

        padre->Update();
        padre->Refresh();

        Update();
        Refresh();

        contadorPaneles++;
        contadorPropiedadesPaneles++;

        contadorNumerosPaneles++;
        contadorElementosJerarquia++;

        gridPropiedadesPanel->Show();

    }


    void ventanaHUD::creaTexto(bool carga, wxString nombrePanel, int numPaneles, int posXPanel, int posYPanel, wxString valorTexto) // , wxFont fuente)
    {

        wxString nombreTexto = "Texto ";
        nombreTexto << contadorNumerosTextos;

        if (imagenVista)
        {
            textoProvisional = new movableText(this->imagenGeneral, contadorTextos, nVentana, nombreTexto, false);
            textosCreados.insert(textosCreados.begin() + contadorTextos, textoProvisional);
        }
        else
        {
            textoProvisional = new movableText(this->panelVistaImagen, contadorTextos, nVentana, nombreTexto, false);
            textosCreados.insert(textosCreados.begin() + contadorTextos, textoProvisional);
        }


        if (carga)
        {
            textoProvisional->setNombre(nombrePanel);
            textoProvisional->setValorTexto(valorTexto);
            textoProvisional->setPosX(posXPanel);
            textoProvisional->setPosY(posYPanel);        
        }

        textoProvisional->Bind(BORRA_TEXTO, &ventanaHUD::eventoBorraTexto, this);
        textoProvisional->Bind(CLICK_TEXTO, &ventanaHUD::eventoClickTexto, this);

        arbolJerarquia->InsertItem(contadorElementosJerarquia, textoProvisional->getNombre());

        padre->Update();
        padre->Refresh();

        gridPropiedadesTexto = new wxPropertyGrid(panelInspector, contadorPropiedadesTextos, wxDefaultPosition, tamInspector, wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED);
        gridPropiedadesTexto->SetSplitterPosition(tamInspector.x/2);

        gridPropiedadesTexto->Connect(wxEVT_PG_CHANGED, wxPropertyGridEventHandler(ventanaHUD::OnPropertyGridChangeTexto), NULL, this);
        gridPropiedadesTexto->Connect(wxEVT_PG_CHANGING, wxPropertyGridEventHandler(ventanaHUD::OnPropertyGridChanging), NULL, this);

        gridPropiedadesTexto->Append(new wxStringProperty("Nombre", wxPG_LABEL, textoProvisional->getNombre()));
        gridPropiedadesTexto->Append(new wxStringProperty("Texto", wxPG_LABEL, textoProvisional->getValorTexto()));
        gridPropiedadesTexto->Append(new wxIntProperty("Posición X", wxPG_LABEL, textoProvisional->getPosX()));
        gridPropiedadesTexto->Append(new wxIntProperty("Posición Y", wxPG_LABEL, textoProvisional->getPosY()));
        gridPropiedadesTexto->Append(new wxFontProperty("Fuente", wxPG_LABEL, textoProvisional->getFuente()));


        propiedadesTextosCreados.insert(propiedadesTextosCreados.begin() + contadorPropiedadesTextos, gridPropiedadesTexto);

        for (int i = 0; i < contadorPropiedadesTextos; i++)
        {

            propiedadesTextosCreados[i]->Hide();
        }

        propiedadesTextosCreados[contadorPropiedadesTextos]->Show();
        textosCreados[contadorTextos]->setProperties(gridPropiedadesTexto);
    
        contadorPropiedadesTextos++;
        contadorTextos++;

        textoProvisional->Refresca();

        Update();
        Refresh();

        contadorNumerosTextos++;
        contadorElementosJerarquia++;

        gridPropiedadesTexto->Show();

    }


    void ventanaHUD::guardaVentana(wxString path)
    {
        wxFile file(path, wxFile::write);

        if (file.IsOpened())
        {
            for (int i = 0; i < contadorElementosJerarquia; i++)
            {
                wxString nombre = arbolJerarquia->GetItemText(i);

                for (int j = 0; j < contadorPaneles; j++)
                {
                    if (panelesCreados[j]->getNombre() == nombre)
                    {
                        file.Write("Panel\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getNumeroPanel()) + "\n");
                        file.Write(panelesCreados[j]->getNombre() + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getPosX()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getPosY()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getTamX()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getTamY()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getR()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getG()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getB()) + "\n");
                        file.Write(panelesCreados[j]->getURLImagen() + "\n");
                        file.Write(wxString::Format(wxT("%d"), panelesCreados[j]->getHayImagen()) + "\n");
                        break;
                    }
                }

                for (int k = 0; k < contadorTextos; k++)
                {
                    if (textosCreados[k]->getNombre() == nombre)
                    {
                        file.Write("Texto\n");
                        file.Write(wxString::Format(wxT("%d"), (int)textosCreados[k]->getNumeroPanel()) + "\n");
                        file.Write(textosCreados[k]->getNombre() + "\n");
                        file.Write(textosCreados[k]->getValorTexto() + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)textosCreados[k]->getPosX()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)textosCreados[k]->getPosY()) + "\n");
                        break;
                    }
                }
            }
        }

        file.Close();
    }


    void ventanaHUD::guardaTodoVentana(wxString path)
    {
        wxFile file(path, wxFile::write_append);

        if (file.IsOpened())
        {
            for (int i = 0; i < contadorElementosJerarquia; i++)
            {
                wxString nombre = arbolJerarquia->GetItemText(i);

                for (int j = 0; j < contadorPaneles; j++)
                {
                    if (panelesCreados[j]->getNombre() == nombre)
                    {
                        file.Write("Panel\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getNumeroPanel()) + "\n");
                        file.Write(panelesCreados[j]->getNombre() + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getPosX()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getPosY()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getTamX()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getTamY()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getR()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getG()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)panelesCreados[j]->getB()) + "\n");
                        file.Write(panelesCreados[j]->getURLImagen() + "\n");
                        file.Write(wxString::Format(wxT("%d"), panelesCreados[j]->getHayImagen()) + "\n");
                        break;
                    }
                }

                for (int k = 0; k < contadorTextos; k++)
                {
                    if (textosCreados[k]->getNombre() == nombre)
                    {
                        file.Write("Texto\n");
                        file.Write(wxString::Format(wxT("%d"), (int)textosCreados[k]->getNumeroPanel()) + "\n");
                        file.Write(textosCreados[k]->getNombre() + "\n");
                        file.Write(textosCreados[k]->getValorTexto() + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)textosCreados[k]->getPosX()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)textosCreados[k]->getPosY()) + "\n");
                        break;
                    }
                }
            }
        }

        file.Close();
    }


    void ventanaHUD::cargaVentana(wxString path)
    {
        wxFileInputStream input(path);
        wxTextInputStream texto(input, wxT("\x09"), wxConvUTF8);
        bool archivoErroneo = false;

        for (int i = 0; i < contadorPaneles; i++)
        {
            panelesCreados[i]->Borrame();
            propiedadesPanelesCreados[i]->Destroy();

            Update();
            Refresh();
        }

        for (int i = 0; i < contadorTextos; i++)
        {
            textosCreados[i]->Borrame();
            propiedadesTextosCreados[i]->Destroy();
        
            Update();
            Refresh();
        }

        panelesCreados.clear();
        propiedadesPanelesCreados.clear();

        textosCreados.clear();
        propiedadesTextosCreados.clear();

        contadorPaneles = 0;
        contadorTextos = 0;

        contadorPropiedadesPaneles = 0;
        contadorPropiedadesTextos = 0;

        contadorElementosJerarquia = 0;

        arbolJerarquia->Destroy();

        arbolJerarquia = new wxListCtrl(panelJerarquia, wxID_ANY, wxDefaultPosition, wxSize(300, 460));
        arbolJerarquia->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(ventanaHUD::clickLista), NULL, this);
        arbolJerarquia->Connect(wxEVT_LIST_ITEM_RIGHT_CLICK, wxListEventHandler(ventanaHUD::borraListaItem), NULL, this);

        while (input.IsOk() && !input.Eof())
        {
            wxString linea = texto.ReadLine();

            if (linea != "Panel" && linea != "Texto" && !input.Eof())
            {
                archivoErroneo = true;
                break;
            }
            else if (linea == "Panel")
            {
                wxString lineaDatosNumero = texto.ReadLine();
                int nObjetoArchivo = wxAtoi(lineaDatosNumero);

                wxString nombreObjetoArchivo = texto.ReadLine();

                wxString lineaDatosPosX = texto.ReadLine();
                int posXArchivo = wxAtoi(lineaDatosPosX);

                wxString lineaDatosPosY = texto.ReadLine();
                int posYArchivo = wxAtoi(lineaDatosPosY);

                wxString lineaDatosTamX = texto.ReadLine();
                int tamXArchivo = wxAtoi(lineaDatosTamX);

                wxString lineaDatosTamY = texto.ReadLine();
                int tamYArchivo = wxAtoi(lineaDatosTamY);

                wxString lineaDatosColorR = texto.ReadLine();
                int colorRArchivo = wxAtoi(lineaDatosColorR);

                wxString lineaDatosColorG = texto.ReadLine();
                int colorGArchivo = wxAtoi(lineaDatosColorG);

                wxString lineaDatosColorB = texto.ReadLine();
                int colorBArchivo = wxAtoi(lineaDatosColorB);

                wxString urlImagenArchivo = texto.ReadLine();

                wxString lineaDatosImagen = texto.ReadLine();
                int imagen = wxAtoi(lineaDatosImagen);
                bool hayImagenArchivo;

                if (imagen == 0)
                {
                    hayImagenArchivo = false;
                }
                else
                {
                    hayImagenArchivo = true;
                }

                creaPanel(true, nombreObjetoArchivo, nObjetoArchivo, posXArchivo, posYArchivo, tamXArchivo, tamYArchivo, colorRArchivo, colorGArchivo, colorBArchivo, urlImagenArchivo, hayImagenArchivo);

                Update();
                Refresh();

            }
            else if (linea == "Texto")
            {
                wxString lineaDatosNombre = texto.ReadLine();
                int nObjetoArchivo = wxAtoi(lineaDatosNombre);

                wxString nombreObjetoArchivo = texto.ReadLine();

                wxString valorObjetoArchivo = texto.ReadLine();

                wxString lineaDatosPosX = texto.ReadLine();
                int posXArchivo = wxAtoi(lineaDatosPosX);

                wxString lineaDatosPosY = texto.ReadLine();
                int posYArchivo = wxAtoi(lineaDatosPosY);

                creaTexto(true, nombreObjetoArchivo, nObjetoArchivo, posXArchivo, posYArchivo, valorObjetoArchivo);

                Update();
                Refresh();

            }
        }

        if (archivoErroneo)
        {
            int res = wxMessageBox("No has Cargado un tipo de archivo apropiado. Inténtalo otra vez.", "Error de Archivo", wxOK, this);
        }
    }


    void ventanaHUD::cambiaFondoPantalla(wxString CurrentDocPath)
    {
        wxSize tamanyo = panelVistaImagen->GetSize();

        urlImagenVista = CurrentDocPath;

        imagenGeneral = new wxImagePanel(panelVistaImagen, CurrentDocPath, wxBITMAP_TYPE_ANY, tamanyo.x, tamanyo.y);

        imagenGeneral->Show();
        imagenVista = true;

        guardaVentana("temp.txt");
        cargaVentana("temp.txt");


        for (int i = 0; i < contadorPaneles; i++)
        {
            panelesCreados[i]->Refresh();
        }

        for (int i = 0; i < contadorTextos; i++)
        {
            textosCreados[i]->Refresca();
        }

        padre->Update();
        padre->Refresh();
    }


    void ventanaHUD::quitaFondoPantalla()
    {
        if (imagenVista)
        {
            imagenVista = false;

            guardaVentana("temp.txt");
            cargaVentana("temp.txt");

            for (int i = 0; i < contadorPaneles; i++)
            {
                panelesCreados[i]->Refresh();
            }

            for (int i = 0; i < contadorTextos; i++)
            {
                textosCreados[i]->Refresca();
            }

            imagenGeneral->Destroy();
            padre->Update();
            padre->Refresh();
        }
        else
        {
            int res = wxMessageBox("No hay ninguna imagen de fondo", "Cuidado", wxOK, this);
        }
    }


    /*void ventanaHUD::eventoMuevePanel(wxCommandEvent& event)
    {
       
    }*/


    void ventanaHUD::eventoClickPanel(wxCommandEvent& event)
    {
        wxAny valor = event.GetId();
        int id = valor.As<int>();
        //cout << id << endl;

        wxString nombre = panelesCreados[id]->getNombre();

        int idJerarquia = 0;

        for (int i = 0; i < contadorElementosJerarquia; i++)
        {
            if (nombre == arbolJerarquia->GetItemText(i))
            {
                idJerarquia = i;
            }
        }

        seleccionaElementoLista(idJerarquia);
    }


    /*void ventanaHUD::eventoMueveTexto(wxCommandEvent& event)
    {
       
    }*/


    void ventanaHUD::eventoClickTexto(wxCommandEvent& event)
    {
        wxAny valor = event.GetId();
        int id = valor.As<int>();
        //cout << id << endl;

        wxString nombre = textosCreados[id]->getNombre();

        int idJerarquia = 0;

        for (int i = 0; i < contadorElementosJerarquia; i++)
        {
            if (nombre == arbolJerarquia->GetItemText(i))
            {
                idJerarquia = i;
            }
        }

        seleccionaElementoLista(idJerarquia);
    }


    void ventanaHUD::clickLista(wxListEvent& event)
    {
        int idProvisional;
        idProvisional = event.GetIndex();

        seleccionaElementoLista(idProvisional);
    }


    void ventanaHUD::seleccionaElementoLista(int id)
    {
        wxString nombre = arbolJerarquia->GetItemText(id);

        for (int i = 0; i < contadorPaneles; i++)
        {
            propiedadesPanelesCreados[i]->Hide();

            if (propiedadesPanelesCreados[i]->GetProperty("Nombre")->GetValue() == nombre)
            {
                propiedadesPanelesCreados[i]->Show();
            }
        }

        for (int j = 0; j < contadorTextos; j++)
        {

            propiedadesTextosCreados[j]->Hide();

            if (propiedadesTextosCreados[j]->GetProperty("Nombre")->GetValue() == nombre)
            {
                propiedadesTextosCreados[j]->Show();
            }
        }

        padre->Update();
        padre->Refresh();
    }


    void ventanaHUD::borraListaItem(wxListEvent& event)
    {
        int idProvisional;
        idProvisional = event.GetIndex();

        wxString nombre = arbolJerarquia->GetItemText(idProvisional);

        int idJerarquia;

        for (int i = 0; i < contadorElementosJerarquia; i++)
        {
            if (nombre == arbolJerarquia->GetItemText(i))
            {
                idJerarquia = i;
            }
        }

        borraItem(nombre, idJerarquia);
    }


    void ventanaHUD::borraItem(wxString nombre, int idProvisional)
    {
        for (int i = 0; i < contadorPaneles; i++)
        {
            if (panelesCreados[i]->getNombre() == nombre)
            {

                propiedadesPanelesCreados[i]->Destroy();
                propiedadesPanelesCreados.erase(propiedadesPanelesCreados.begin() + i);

                panelesCreados[i]->Borrame();
                panelesCreados.erase(panelesCreados.begin() + i);

                panelVistaImagen->Update();
                panelVistaImagen->Refresh();

                arbolJerarquia->DeleteItem(idProvisional);

                arbolJerarquia->Arrange();

                contadorPaneles--;
                contadorPropiedadesPaneles--;
                contadorElementosJerarquia--;

                arbolJerarquia->Update();
                arbolJerarquia->Refresh();
                Update();
                Refresh();

                break;
            }
        }

        for (int i = 0; i < contadorTextos; i++)
        {
            if (textosCreados[i]->getNombre() == nombre)
            {

                propiedadesTextosCreados[i]->Destroy();
                propiedadesTextosCreados.erase(propiedadesTextosCreados.begin() + i);

                textosCreados[i]->Borrame();
                textosCreados.erase(textosCreados.begin() + i);

                panelVistaImagen->Update();
                panelVistaImagen->Refresh();

                arbolJerarquia->DeleteItem(idProvisional);

                arbolJerarquia->Arrange();

                contadorTextos--;
                contadorPropiedadesTextos--;
                contadorElementosJerarquia--;

                arbolJerarquia->Update();
                arbolJerarquia->Refresh();

                Update();
                Refresh();

                break;
            }
        }

        arbolJerarquia->Update();
        arbolJerarquia->Refresh();

        Update();
        Refresh();
    }


    void ventanaHUD::eventoBorraPanel(wxCommandEvent& event)
    {
        wxAny valor = event.GetId();
        int id = valor.As<int>();

        wxString nombrePanel = panelesCreados[id]->getNombre();

        int idJerarquia;

        for (int i = 0; i < contadorElementosJerarquia; i++)
        {
            if (nombrePanel == arbolJerarquia->GetItemText(i))
            {
                idJerarquia = i;
            }
        }

        borraItem(nombrePanel, idJerarquia);
    }


    void ventanaHUD::eventoBorraTexto(wxCommandEvent& event)
    {
        wxAny valor = event.GetId();
        int id = valor.As<int>();

        wxString nombrePanel = textosCreados[id]->getNombre();

        int idJerarquia;

        for (int i = 0; i < contadorElementosJerarquia; i++)
        {
            if (nombrePanel == arbolJerarquia->GetItemText(i))
            {
                idJerarquia = i;
            }
        }

        borraItem(nombrePanel, idJerarquia);
    }


    void ventanaHUD::borraTexto(int posicion)
    {
        int idProvisional;
        wxString nombreJerarquia = textosCreados[posicion]->getNombre();

        for (int i = 0; i < contadorElementosJerarquia; i++)
        {
            if (arbolJerarquia->GetItemText(i) == nombreJerarquia)
            {
                idProvisional = i;
            }
        }
    
        propiedadesTextosCreados[posicion]->Destroy();
        propiedadesTextosCreados.erase(propiedadesTextosCreados.begin() + posicion);

        textosCreados[posicion]->Borrame();
        textosCreados.erase(textosCreados.begin() + posicion);

        panelVistaImagen->Update();
        panelVistaImagen->Refresh();

        arbolJerarquia->DeleteItem(idProvisional);

        arbolJerarquia->Arrange();

        contadorTextos--;
        contadorPropiedadesTextos--;
        contadorElementosJerarquia--;

        arbolJerarquia->Update();
        arbolJerarquia->Refresh();

        Update();
        Refresh();
    }


    void ventanaHUD::OnPropertyGridChangePanel(wxPropertyGridEvent& event)
    {
        int idProvisional;
        idProvisional = event.GetId();

        wxPGProperty* property = event.GetProperty();
        const wxString& nombrePropiedad = property->GetName();
        wxAny valor = property->GetValue();

        wxPGProperty* colorProperty = propiedadesPanelesCreados[idProvisional]->GetProperty("Color");

        if (nombrePropiedad == "Nombre")
        {
            wxString nombreOld = panelesCreados[idProvisional]->getNombre();
            panelesCreados[idProvisional]->setNombre(valor.As<wxString>());
        
            for (int i = 0; i < contadorElementosJerarquia; i++)
            {
                if (arbolJerarquia->GetItemText(i) == nombreOld)
                {
                    arbolJerarquia->SetItemText(i, panelesCreados[idProvisional]->getNombre());
                }
            } 

            arbolJerarquia->Update();
            arbolJerarquia->Refresh();

            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Posición X")
        {
            panelesCreados[idProvisional]->setPosX(valor.As<int>());
            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Posición Y")
        {
            panelesCreados[idProvisional]->setPosY(valor.As<int>());
            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Tamaño X")
        {
            panelesCreados[idProvisional]->setTamX(valor.As<int>());
            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Tamaño Y")
        {
            panelesCreados[idProvisional]->setTamY(valor.As<int>());
            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Color")
        {
            wxColourPropertyValue colorValue = valor.As<wxColourPropertyValue>();
            wxColor color = colorValue.m_colour;

            panelesCreados[idProvisional]->setR(color.Red());
            panelesCreados[idProvisional]->setG(color.Green());
            panelesCreados[idProvisional]->setB(color.Blue());

            panelesCreados[idProvisional]->Refresh();
            panelesCreados[idProvisional]->Update();
            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Imagen")
        {
            panelesCreados[idProvisional]->ponImagen(valor.As<wxString>(), wxBITMAP_TYPE_ANY);
            panelesCreados[idProvisional]->setURLImagen(valor.As<wxString>());
            panelesCreados[idProvisional]->setHayImagen(true);
            propiedadesPanelesCreados[idProvisional]->GetProperty("Imagen sí/no")->SetValue(true);
            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Imagen sí/no")
        {
            if (panelesCreados[idProvisional]->getURLImagen() == "NO HAY")
            {
                int res = wxMessageBox("No hay ninguna imagen añadida. Añádela.", "Cuidado", wxOK, this);
            }
            else 
            {
                if (valor.As<bool>())
                {
                    panelesCreados[idProvisional]->setHayImagen(true);
                    panelesCreados[idProvisional]->ponImagen(panelesCreados[idProvisional]->getURLImagen(), wxBITMAP_TYPE_ANY);
                }
                else
                {
                    panelesCreados[idProvisional]->setHayImagen(false);
                    panelesCreados[idProvisional]->quitaImagen();

                    if (imagenVista)
                    {
                        imagenGeneral->Show();
                        imagenGeneral->Update();
                        imagenGeneral->Refresh();

                        panelVistaImagen->Update();
                        panelVistaImagen->Refresh();
                    }
                }
            }
        }
    }


    void ventanaHUD::OnPropertyGridChangeTexto(wxPropertyGridEvent& event)
    {
        int idProvisional;
        idProvisional = event.GetId();

        wxPGProperty* property = event.GetProperty();
        const wxString& nombrePropiedad = property->GetName();
        wxAny valor = property->GetValue();

        wxPGProperty* colorProperty = propiedadesTextosCreados[idProvisional]->GetProperty("Color");

        if (nombrePropiedad == "Nombre")
        {
            wxString nombreOld = textosCreados[idProvisional]->getNombre();
            textosCreados[idProvisional]->setNombre(valor.As<wxString>());

            for (int i = 0; i < contadorElementosJerarquia; i++)
            {
                if (arbolJerarquia->GetItemText(i) == nombreOld)
                {
                    arbolJerarquia->SetItemText(i, textosCreados[idProvisional]->getNombre());
                }
            }
            arbolJerarquia->Update();
            arbolJerarquia->Refresh();

            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Texto")
        {
            textosCreados[idProvisional]->setValorTexto(valor.As<wxString>());
            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Posición X")
        {
            textosCreados[idProvisional]->setPosX(valor.As<int>());
            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Posición Y")
        {
            textosCreados[idProvisional]->setPosY(valor.As<int>());
            padre->Update();
            padre->Refresh();
        }
        else if (nombrePropiedad == "Fuente")
        {
            textosCreados[idProvisional]->setFuente(valor.As<wxFont>());
            padre->Update();
            padre->Refresh();
        }
    
    }


    void ventanaHUD::OnPropertyGridChanging(wxPropertyGridEvent& event)
    {
        wxPGProperty* p = event.GetProperty();

        if (p->GetName() == "Font")
        {
            int res =
                wxMessageBox(wxString::Format("'%s' is about to change (to variant of type '%s')\n\nAllow or deny?",
                    p->GetName(), event.GetValue().GetType()),
                    "Testing wxEVT_PG_CHANGING", wxYES_NO, gridPropiedadesPanel);

            if (res == wxNO)
            {
                wxASSERT(event.CanVeto());

                event.Veto();

                // Since we ask a question, it is better if we omit any validation
                // failure behaviour.
                event.SetValidationFailureBehavior(0);
            }
        }
    }
    

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ventanaGlobal : public wxPanel
{

    public:

        ventanaGlobal(wxPanel*, int, wxSize);

        void creaPanel(bool, wxString, int, int, int, int, int, int, int, int, wxString, bool, bool, bool);
        void creaTexto(wxString, wxString, int, int);
        void ponImagenGlobal(wxString);
        bool getImagenPadre();

        wxPanel* panelVentana;
        wxPanel* panelGrid;
        wxVector<movablePanel*> paneles;
        wxVector<movableText*> textos;


    private:

        int contadorPaneles = 0;
        int contadorTextos = 0;
        int nVentana;
        bool imagenPadre = false;

        wxBoxSizer* horizontalSizer;
        wxBoxSizer* verticalSizer;
        wxImagePanel* imagenGlobal;
        movablePanel* panelProvisional;
        movableText* textoProvisional;
        wxPanel* padre;
        wxSize tamVentana;   
};


    ventanaGlobal::ventanaGlobal(wxPanel* parent, int numeroPanel, wxSize sizeVentana)
    {
        padre = parent;
        nVentana = numeroPanel;

        contadorPaneles = 0;
        contadorTextos = 0;

        tamVentana = sizeVentana;

        panelGrid = new wxPanel(parent, wxID_ANY);
        panelGrid->SetSize(wxSize(100, 100));
        panelGrid->Update();

        panelVentana = new wxPanel(panelGrid, nVentana, wxDefaultPosition, tamVentana, 0);
        panelVentana->SetBackgroundColour(wxColor(0, 255, 255));

        horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
        verticalSizer = new wxBoxSizer(wxVERTICAL);

        verticalSizer->AddStretchSpacer(1);
        verticalSizer->Add(panelVentana, 0);
        verticalSizer->AddStretchSpacer(1);

        horizontalSizer->AddStretchSpacer(1);
        horizontalSizer->Add(verticalSizer, 0, wxALIGN_CENTER_VERTICAL);
        horizontalSizer->AddStretchSpacer(1);

        panelGrid->SetSizer(horizontalSizer);
        panelGrid->Layout();
    }


    void ventanaGlobal::creaPanel(bool carga, wxString nombrePanel, int numPaneles, int posXPanel, int posYPanel, int tamXPanel, int tamYPanel, int colorRPanel, int colorGPanel, int colorBPanel, wxString urlImagenPanel, bool hayImagenPanel, bool movidoGlobal, bool imagenFondo)
    {
        if (imagenPadre)
        {
            panelProvisional = new movablePanel(this->imagenGlobal, numPaneles, nVentana, nombrePanel, true);
        }
        else
        {
            panelProvisional = new movablePanel(this->panelVentana, numPaneles, nVentana, nombrePanel, true);
        }

        panelProvisional->setNombre(nombrePanel);
        panelProvisional->setPosX(posXPanel);
        panelProvisional->setPosY(posYPanel);
        panelProvisional->setTamX(tamXPanel);
        panelProvisional->setTamY(tamYPanel);
        panelProvisional->setR(colorRPanel);
        panelProvisional->setG(colorGPanel);
        panelProvisional->setB(colorBPanel);
        panelProvisional->setURLImagen(urlImagenPanel);
        panelProvisional->setHayImagen(hayImagenPanel);

        if (hayImagenPanel)
        {
            panelProvisional->ponImagen(urlImagenPanel, wxBITMAP_TYPE_ANY);
        }

        paneles.insert(paneles.begin() + contadorPaneles, panelProvisional);
        contadorPaneles++;
               

        padre->Update();
        padre->Refresh();

        Update();
        Refresh();
    }


    void ventanaGlobal::creaTexto(wxString nombreTexto, wxString valorTexto, int posXPanel, int posYPanel)
    {
        if (imagenPadre)
        {
            textoProvisional = new movableText(this->imagenGlobal, contadorTextos, nVentana, nombreTexto, true);
        }
        else
        {
            textoProvisional = new movableText(this->panelVentana, contadorTextos, nVentana, nombreTexto, true);
        }

        wxFont fuenteTexto = wxFont(3, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        textoProvisional->setFuente(fuenteTexto);
           
        textoProvisional->setNombre(nombreTexto);
        textoProvisional->setValorTexto(valorTexto);
        textoProvisional->setPosX(posXPanel);
        textoProvisional->setPosY(posYPanel);

        textos.insert(textos.begin() + contadorTextos, textoProvisional);
        contadorTextos++;
    }


    void ventanaGlobal::ponImagenGlobal(wxString url)
    {
        imagenPadre = true;
        imagenGlobal = new wxImagePanel(this->panelVentana, url, wxBITMAP_TYPE_ANY, tamVentana.x, tamVentana.y);
        imagenGlobal->Show();
    }


    bool ventanaGlobal::getImagenPadre()
    {
        return imagenPadre;
    }
 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ventanaPantallas : public wxFrame
{

    public:

        ventanaPantallas(wxWindow*, wxWindowID, const wxString&, const wxPoint&, const wxSize&, int, wxVector<ventanaHUD*>);

        void creaVentana(int, int);
        void creaVentanaImagen(wxString, int, int);
        void cargaVentanas(wxString);
        void creaPanel(wxPanel*, int, bool, wxString, int, int, int, int, int, int, int, int, wxString, bool, bool, bool);
        void creaTexto(wxPanel*, int, wxString, int, int, wxString);
        void onPanelMouseUp(wxCommandEvent& evt);
        void onTextMouseUp(wxCommandEvent& evt);

        wxVector<ventanaHUD*> listaVentanas;


    private:

        int contadorVentanas = 0;
        int contadorPaneles = 0;
        int limiteVentanas;
        int FilasColumnas;
        int idPanel;
        int idTexto;
        int idVentana;
        
        wxBoxSizer* sizer;
        wxGridSizer* gs;
        wxPanel* panelFondo;
        wxSize tamVentana;
        ventanaGlobal* globalVentana;

        wxVector<ventanaGlobal*> ventanas;
        

    wxDECLARE_EVENT_TABLE();

};

    wxBEGIN_EVENT_TABLE(ventanaPantallas, wxFrame)
    wxEND_EVENT_TABLE()


    ventanaPantallas::ventanaPantallas(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, int windowLimit, wxVector<ventanaHUD*> windowsList) : wxFrame(parent, id, title, pos, size)
    {
        contadorPaneles = 0;

        tamVentana = size;
        limiteVentanas = windowLimit;
        listaVentanas = windowsList;
        FilasColumnas = ceil(sqrt(limiteVentanas));

        cout << "Tam Celda X: " << tamVentana.x/FilasColumnas << endl;
        cout << "Tam Celda Y: " << tamVentana.y/FilasColumnas << endl;

        panelFondo = new wxPanel(this, wxID_ANY, wxDefaultPosition, size);
        panelFondo->SetBackgroundColour(wxColour(217, 37, 52));

        sizer = new wxBoxSizer(wxVERTICAL);
        gs = new wxGridSizer(FilasColumnas, FilasColumnas, 10, 10);

        sizer->Add(gs, 1, wxEXPAND);
        SetSizer(sizer);
        SetMinSize(wxSize(270, 220));

        Centre();
    }


    void ventanaPantallas::creaVentana(int tamVentanaX, int tamVentanaY)
    {
        globalVentana = new ventanaGlobal(panelFondo, contadorVentanas, wxSize(tamVentanaX * 0.3, tamVentanaY * 0.3));
        globalVentana->panelVentana->Bind(PANEL_UP, &ventanaPantallas::onPanelMouseUp, this);
        globalVentana->panelVentana->Bind(TEXT_UP, &ventanaPantallas::onTextMouseUp, this);

        gs->Add(globalVentana->panelGrid, 0, wxEXPAND);

        ventanas.insert(ventanas.begin() + contadorVentanas, globalVentana);

        contadorVentanas++;
    }


    void ventanaPantallas::creaVentanaImagen(wxString url, int tamVentanaX, int tamVentanaY)
    {
        globalVentana = new ventanaGlobal(panelFondo, contadorVentanas, wxSize(tamVentanaX * 0.3, tamVentanaY * 0.3));
        globalVentana->panelVentana->Bind(PANEL_UP, &ventanaPantallas::onPanelMouseUp, this);
        globalVentana->panelVentana->Bind(TEXT_UP, &ventanaPantallas::onTextMouseUp, this);
        globalVentana->ponImagenGlobal(url);

        gs->Add(globalVentana->panelGrid, 0, wxEXPAND);

        ventanas.insert(ventanas.begin() + contadorVentanas, globalVentana);

        contadorVentanas++;
    }


    void ventanaPantallas::onPanelMouseUp(wxCommandEvent& evt)
    {
        //ReleaseMouse();
        bool borra = false;

        int tamDivisionX = tamVentana.x / FilasColumnas;
        int tamDivisionY = tamVentana.y / FilasColumnas;

        idVentana = evt.GetId();    // idVentana
        idPanel = evt.GetInt();     // idPanel

        cout << endl;
        cout << "Sucede Evento" << endl << endl;
        cout << "Panel X: " << ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x << endl;
        cout << "Panel Y: " << ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y << endl;
        cout << "Contador Ventanas: : " << contadorVentanas << endl;



        // ARRIBA - IZQUIERDA
        if (ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x > 0 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x < tamDivisionX
        && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y > 0 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y < tamDivisionY)
        {
            int ventana = 0;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->paneles.size() << endl;
            }
            else
            {

                //DISTINTA VENTANA. HACE.
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaPanel(false, ventanas[idVentana]->paneles[idPanel]->getNombre(), ventanas[idVentana]->paneles[idPanel]->getNumeroPanel(), ventanas[idVentana]->paneles[idPanel]->getPosX(),
                    ventanas[idVentana]->paneles[idPanel]->getPosY(), ventanas[idVentana]->paneles[idPanel]->getTamX(), ventanas[idVentana]->paneles[idPanel]->getTamY(),
                    ventanas[idVentana]->paneles[idPanel]->getR(), ventanas[idVentana]->paneles[idPanel]->getG(), ventanas[idVentana]->paneles[idPanel]->getB(),
                    ventanas[idVentana]->paneles[idPanel]->getURLImagen(), ventanas[idVentana]->paneles[idPanel]->getHayImagen(), true, ventanas[idVentana]->getImagenPadre());


                listaVentanas[ventana]->creaPanel(true, listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), listaVentanas[idVentana]->panelesCreados[idPanel]->getNumeroPanel(), listaVentanas[idVentana]->panelesCreados[idPanel]->getPosX(), 
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getPosY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamX(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getR(), 
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getG(), listaVentanas[idVentana]->panelesCreados[idPanel]->getB(), listaVentanas[idVentana]->panelesCreados[idPanel]->getURLImagen(), listaVentanas[idVentana]->panelesCreados[idPanel]->getHayImagen());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), idPanel);

            }
        }
        // ARRIBA - MEDIO
        else if (contadorVentanas > 1 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x > tamDivisionX && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x < tamDivisionX*2
        && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y > 0 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y < tamDivisionY)
        {
            int ventana = 1;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->paneles.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaPanel(false, ventanas[idVentana]->paneles[idPanel]->getNombre(), ventanas[idVentana]->paneles[idPanel]->getNumeroPanel(), ventanas[idVentana]->paneles[idPanel]->getPosX(),
                    ventanas[idVentana]->paneles[idPanel]->getPosY(), ventanas[idVentana]->paneles[idPanel]->getTamX(), ventanas[idVentana]->paneles[idPanel]->getTamY(),
                    ventanas[idVentana]->paneles[idPanel]->getR(), ventanas[idVentana]->paneles[idPanel]->getG(), ventanas[idVentana]->paneles[idPanel]->getB(),
                    ventanas[idVentana]->paneles[idPanel]->getURLImagen(), ventanas[idVentana]->paneles[idPanel]->getHayImagen(), true, ventanas[idVentana]->getImagenPadre());


                listaVentanas[ventana]->creaPanel(true, listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), listaVentanas[idVentana]->panelesCreados[idPanel]->getNumeroPanel(), listaVentanas[idVentana]->panelesCreados[idPanel]->getPosX(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getPosY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamX(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getR(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getG(), listaVentanas[idVentana]->panelesCreados[idPanel]->getB(), listaVentanas[idVentana]->panelesCreados[idPanel]->getURLImagen(), listaVentanas[idVentana]->panelesCreados[idPanel]->getHayImagen());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), idPanel);

            }
        }
        // ARRIBA - DERECHA
        else if (contadorVentanas > 2 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x > tamDivisionX*2 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x < tamDivisionX*3
        && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y > 0 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y < tamDivisionY)
        {
            int ventana = 2;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->paneles.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaPanel(false, ventanas[idVentana]->paneles[idPanel]->getNombre(), ventanas[idVentana]->paneles[idPanel]->getNumeroPanel(), ventanas[idVentana]->paneles[idPanel]->getPosX(),
                    ventanas[idVentana]->paneles[idPanel]->getPosY(), ventanas[idVentana]->paneles[idPanel]->getTamX(), ventanas[idVentana]->paneles[idPanel]->getTamY(),
                    ventanas[idVentana]->paneles[idPanel]->getR(), ventanas[idVentana]->paneles[idPanel]->getG(), ventanas[idVentana]->paneles[idPanel]->getB(),
                    ventanas[idVentana]->paneles[idPanel]->getURLImagen(), ventanas[idVentana]->paneles[idPanel]->getHayImagen(), true, ventanas[idVentana]->getImagenPadre());


                listaVentanas[ventana]->creaPanel(true, listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), listaVentanas[idVentana]->panelesCreados[idPanel]->getNumeroPanel(), listaVentanas[idVentana]->panelesCreados[idPanel]->getPosX(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getPosY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamX(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getR(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getG(), listaVentanas[idVentana]->panelesCreados[idPanel]->getB(), listaVentanas[idVentana]->panelesCreados[idPanel]->getURLImagen(), listaVentanas[idVentana]->panelesCreados[idPanel]->getHayImagen());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), idPanel);

            } 
        }
        // MEDIO - IZQUIERDA
        else if (contadorVentanas > 3 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x > 0 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x < tamDivisionX 
        && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y > tamDivisionY && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y < tamDivisionY*2)
        {
            int ventana = 3;
        
            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->paneles.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaPanel(false, ventanas[idVentana]->paneles[idPanel]->getNombre(), ventanas[idVentana]->paneles[idPanel]->getNumeroPanel(), ventanas[idVentana]->paneles[idPanel]->getPosX(),
                    ventanas[idVentana]->paneles[idPanel]->getPosY(), ventanas[idVentana]->paneles[idPanel]->getTamX(), ventanas[idVentana]->paneles[idPanel]->getTamY(),
                    ventanas[idVentana]->paneles[idPanel]->getR(), ventanas[idVentana]->paneles[idPanel]->getG(), ventanas[idVentana]->paneles[idPanel]->getB(),
                    ventanas[idVentana]->paneles[idPanel]->getURLImagen(), ventanas[idVentana]->paneles[idPanel]->getHayImagen(), true, ventanas[idVentana]->getImagenPadre());


                listaVentanas[ventana]->creaPanel(true, listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), listaVentanas[idVentana]->panelesCreados[idPanel]->getNumeroPanel(), listaVentanas[idVentana]->panelesCreados[idPanel]->getPosX(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getPosY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamX(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getR(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getG(), listaVentanas[idVentana]->panelesCreados[idPanel]->getB(), listaVentanas[idVentana]->panelesCreados[idPanel]->getURLImagen(), listaVentanas[idVentana]->panelesCreados[idPanel]->getHayImagen());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), idPanel);

            }
        }
        // MEDIO - MEDIO
        else if (contadorVentanas > 4 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x > tamDivisionX && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x < tamDivisionX*2
        && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y > tamDivisionY && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y < tamDivisionY*2)
        {
            int ventana = 4;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->paneles.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;
            
                ventanas[ventana]->creaPanel(false, ventanas[idVentana]->paneles[idPanel]->getNombre(), ventanas[idVentana]->paneles[idPanel]->getNumeroPanel(), ventanas[idVentana]->paneles[idPanel]->getPosX(),
                    ventanas[idVentana]->paneles[idPanel]->getPosY(), ventanas[idVentana]->paneles[idPanel]->getTamX(), ventanas[idVentana]->paneles[idPanel]->getTamY(),
                    ventanas[idVentana]->paneles[idPanel]->getR(), ventanas[idVentana]->paneles[idPanel]->getG(), ventanas[idVentana]->paneles[idPanel]->getB(),
                    ventanas[idVentana]->paneles[idPanel]->getURLImagen(), ventanas[idVentana]->paneles[idPanel]->getHayImagen(), true, ventanas[idVentana]->getImagenPadre());


                listaVentanas[ventana]->creaPanel(true, listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), listaVentanas[idVentana]->panelesCreados[idPanel]->getNumeroPanel(), listaVentanas[idVentana]->panelesCreados[idPanel]->getPosX(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getPosY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamX(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getR(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getG(), listaVentanas[idVentana]->panelesCreados[idPanel]->getB(), listaVentanas[idVentana]->panelesCreados[idPanel]->getURLImagen(), listaVentanas[idVentana]->panelesCreados[idPanel]->getHayImagen());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), idPanel);

            }
        }
        // MEDIO - DERECHA
        else if (contadorVentanas > 5 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x > tamDivisionX*2 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x < tamDivisionX*3
        && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y > tamDivisionY && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y < tamDivisionY*2)
        {
            int ventana = 5;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->paneles.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaPanel(false, ventanas[idVentana]->paneles[idPanel]->getNombre(), ventanas[idVentana]->paneles[idPanel]->getNumeroPanel(), ventanas[idVentana]->paneles[idPanel]->getPosX(),
                    ventanas[idVentana]->paneles[idPanel]->getPosY(), ventanas[idVentana]->paneles[idPanel]->getTamX(), ventanas[idVentana]->paneles[idPanel]->getTamY(),
                    ventanas[idVentana]->paneles[idPanel]->getR(), ventanas[idVentana]->paneles[idPanel]->getG(), ventanas[idVentana]->paneles[idPanel]->getB(),
                    ventanas[idVentana]->paneles[idPanel]->getURLImagen(), ventanas[idVentana]->paneles[idPanel]->getHayImagen(), true, ventanas[idVentana]->getImagenPadre());


                listaVentanas[ventana]->creaPanel(true, listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), listaVentanas[idVentana]->panelesCreados[idPanel]->getNumeroPanel(), listaVentanas[idVentana]->panelesCreados[idPanel]->getPosX(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getPosY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamX(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getR(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getG(), listaVentanas[idVentana]->panelesCreados[idPanel]->getB(), listaVentanas[idVentana]->panelesCreados[idPanel]->getURLImagen(), listaVentanas[idVentana]->panelesCreados[idPanel]->getHayImagen());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), idPanel);

            }
        }
        // BAJO - IZQUIERDA
        else if (contadorVentanas > 6 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x > 0 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x < tamDivisionX
        && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y > tamDivisionY*2 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y < tamDivisionY*3)
        {
            int ventana = 6;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->paneles.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaPanel(false, ventanas[idVentana]->paneles[idPanel]->getNombre(), ventanas[idVentana]->paneles[idPanel]->getNumeroPanel(), ventanas[idVentana]->paneles[idPanel]->getPosX(),
                    ventanas[idVentana]->paneles[idPanel]->getPosY(), ventanas[idVentana]->paneles[idPanel]->getTamX(), ventanas[idVentana]->paneles[idPanel]->getTamY(),
                    ventanas[idVentana]->paneles[idPanel]->getR(), ventanas[idVentana]->paneles[idPanel]->getG(), ventanas[idVentana]->paneles[idPanel]->getB(),
                    ventanas[idVentana]->paneles[idPanel]->getURLImagen(), ventanas[idVentana]->paneles[idPanel]->getHayImagen(), true, ventanas[idVentana]->getImagenPadre());


                listaVentanas[ventana]->creaPanel(true, listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), listaVentanas[idVentana]->panelesCreados[idPanel]->getNumeroPanel(), listaVentanas[idVentana]->panelesCreados[idPanel]->getPosX(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getPosY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamX(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getR(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getG(), listaVentanas[idVentana]->panelesCreados[idPanel]->getB(), listaVentanas[idVentana]->panelesCreados[idPanel]->getURLImagen(), listaVentanas[idVentana]->panelesCreados[idPanel]->getHayImagen());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), idPanel);

            }

        }
        // BAJO - MEDIO
        else if (contadorVentanas > 7 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x > tamDivisionX && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x < tamDivisionX*2
        && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y > tamDivisionY*2 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y < tamDivisionY*3)
        {
            int ventana = 7;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->paneles.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaPanel(false, ventanas[idVentana]->paneles[idPanel]->getNombre(), ventanas[idVentana]->paneles[idPanel]->getNumeroPanel(), ventanas[idVentana]->paneles[idPanel]->getPosX(),
                    ventanas[idVentana]->paneles[idPanel]->getPosY(), ventanas[idVentana]->paneles[idPanel]->getTamX(), ventanas[idVentana]->paneles[idPanel]->getTamY(),
                    ventanas[idVentana]->paneles[idPanel]->getR(), ventanas[idVentana]->paneles[idPanel]->getG(), ventanas[idVentana]->paneles[idPanel]->getB(),
                    ventanas[idVentana]->paneles[idPanel]->getURLImagen(), ventanas[idVentana]->paneles[idPanel]->getHayImagen(), true, ventanas[idVentana]->getImagenPadre());

            
                listaVentanas[ventana]->creaPanel(true, listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), listaVentanas[idVentana]->panelesCreados[idPanel]->getNumeroPanel(), listaVentanas[idVentana]->panelesCreados[idPanel]->getPosX(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getPosY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamX(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getR(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getG(), listaVentanas[idVentana]->panelesCreados[idPanel]->getB(), listaVentanas[idVentana]->panelesCreados[idPanel]->getURLImagen(), listaVentanas[idVentana]->panelesCreados[idPanel]->getHayImagen());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), idPanel);

            }
        }

        // BAJO - DERECHA
        else if (contadorVentanas > 8 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x > tamDivisionX*2 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().x < tamDivisionX*3
        && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y > tamDivisionY*2 && ventanas[idVentana]->paneles[idPanel]->GetScreenPosition().y < tamDivisionY*3)
        {
            int ventana = 8;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Panel " << idPanel << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->paneles.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Panel " << idPanel <<" en Ventana " << ventana << endl;

                ventanas[ventana]->creaPanel(false, ventanas[idVentana]->paneles[idPanel]->getNombre(), ventanas[idVentana]->paneles[idPanel]->getNumeroPanel(), ventanas[idVentana]->paneles[idPanel]->getPosX(),
                    ventanas[idVentana]->paneles[idPanel]->getPosY(), ventanas[idVentana]->paneles[idPanel]->getTamX(), ventanas[idVentana]->paneles[idPanel]->getTamY(),
                    ventanas[idVentana]->paneles[idPanel]->getR(), ventanas[idVentana]->paneles[idPanel]->getG(), ventanas[idVentana]->paneles[idPanel]->getB(),
                    ventanas[idVentana]->paneles[idPanel]->getURLImagen(), ventanas[idVentana]->paneles[idPanel]->getHayImagen(), true, ventanas[idVentana]->getImagenPadre());

            
                listaVentanas[ventana]->creaPanel(true, listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), listaVentanas[idVentana]->panelesCreados[idPanel]->getNumeroPanel(), listaVentanas[idVentana]->panelesCreados[idPanel]->getPosX(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getPosY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamX(), listaVentanas[idVentana]->panelesCreados[idPanel]->getTamY(), listaVentanas[idVentana]->panelesCreados[idPanel]->getR(),
                    listaVentanas[idVentana]->panelesCreados[idPanel]->getG(), listaVentanas[idVentana]->panelesCreados[idPanel]->getB(), listaVentanas[idVentana]->panelesCreados[idPanel]->getURLImagen(), listaVentanas[idVentana]->panelesCreados[idPanel]->getHayImagen());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->panelesCreados[idPanel]->getNombre(), idPanel);
            }   
        }    
    }


    void ventanaPantallas::onTextMouseUp(wxCommandEvent& evt)
    {
        //ReleaseMouse();
        bool borra = false;

        int tamDivisionX = tamVentana.x / FilasColumnas;
        int tamDivisionY = tamVentana.y / FilasColumnas;

        idVentana = evt.GetId();    // idVentana
        idTexto = evt.GetInt();     // idTexto

        cout << endl;
        cout << "Sucede Evento" << endl << endl;
        cout << "Texto X: " << ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x << endl;
        cout << "Texto Y: " << ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y << endl;
        cout << "Contador Ventanas: : " << contadorVentanas << endl;



        // ARRIBA - IZQUIERDA
        if (ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x > 0 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x < tamDivisionX
            && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y > 0 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y < tamDivisionY)
        {
            int ventana = 0;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->textos.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaTexto(ventanas[idVentana]->textos[idTexto]->getNombre(), ventanas[idVentana]->textos[idTexto]->getValorTexto(), ventanas[idVentana]->textos[idTexto]->getPosX(), ventanas[idVentana]->textos[idTexto]->getPosY());

                listaVentanas[ventana]->creaTexto(true, listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), listaVentanas[idVentana]->textosCreados[idTexto]->getNumeroPanel(), listaVentanas[idVentana]->textosCreados[idTexto]->getPosX(), 
                    listaVentanas[idVentana]->textosCreados[idTexto]->getPosY(), listaVentanas[idVentana]->textosCreados[idTexto]->getValorTexto());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), idTexto);

            }
        }
        // ARRIBA - MEDIO
        else if (contadorVentanas > 1 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x > tamDivisionX && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x < tamDivisionX * 2
            && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y > 0 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y < tamDivisionY)
        {
            int ventana = 1;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->textos.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaTexto(ventanas[idVentana]->textos[idTexto]->getNombre(), ventanas[idVentana]->textos[idTexto]->getValorTexto(), ventanas[idVentana]->textos[idTexto]->getPosX(), ventanas[idVentana]->textos[idTexto]->getPosY());

                listaVentanas[ventana]->creaTexto(true, listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), listaVentanas[idVentana]->textosCreados[idTexto]->getNumeroPanel(), listaVentanas[idVentana]->textosCreados[idTexto]->getPosX(),
                    listaVentanas[idVentana]->textosCreados[idTexto]->getPosY(), listaVentanas[idVentana]->textosCreados[idTexto]->getValorTexto());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), idTexto);

            }
        }
        // ARRIBA - DERECHA
        else if (contadorVentanas > 2 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x > tamDivisionX * 2 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x < tamDivisionX * 3
            && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y > 0 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y < tamDivisionY)
        {
            int ventana = 2;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->textos.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaTexto(ventanas[idVentana]->textos[idTexto]->getNombre(), ventanas[idVentana]->textos[idTexto]->getValorTexto(), ventanas[idVentana]->textos[idTexto]->getPosX(), ventanas[idVentana]->textos[idTexto]->getPosY());

                listaVentanas[ventana]->creaTexto(true, listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), listaVentanas[idVentana]->textosCreados[idTexto]->getNumeroPanel(), listaVentanas[idVentana]->textosCreados[idTexto]->getPosX(),
                    listaVentanas[idVentana]->textosCreados[idTexto]->getPosY(), listaVentanas[idVentana]->textosCreados[idTexto]->getValorTexto());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), idTexto);

            }
        }
        // MEDIO - IZQUIERDA
        else if (contadorVentanas > 3 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x > 0 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x < tamDivisionX
            && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y > tamDivisionY && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y < tamDivisionY * 2)
        {
            int ventana = 3;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->textos.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaTexto(ventanas[idVentana]->textos[idTexto]->getNombre(), ventanas[idVentana]->textos[idTexto]->getValorTexto(), ventanas[idVentana]->textos[idTexto]->getPosX(), ventanas[idVentana]->textos[idTexto]->getPosY());

                listaVentanas[ventana]->creaTexto(true, listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), listaVentanas[idVentana]->textosCreados[idTexto]->getNumeroPanel(), listaVentanas[idVentana]->textosCreados[idTexto]->getPosX(),
                    listaVentanas[idVentana]->textosCreados[idTexto]->getPosY(), listaVentanas[idVentana]->textosCreados[idTexto]->getValorTexto());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), idTexto);

            }
        }
        // MEDIO - MEDIO
        else if (contadorVentanas > 4 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x > tamDivisionX && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x < tamDivisionX * 2
            && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y > tamDivisionY && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y < tamDivisionY * 2)
        {
            int ventana = 4;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->textos.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaTexto(ventanas[idVentana]->textos[idTexto]->getNombre(), ventanas[idVentana]->textos[idTexto]->getValorTexto(), ventanas[idVentana]->textos[idTexto]->getPosX(), ventanas[idVentana]->textos[idTexto]->getPosY());

                listaVentanas[ventana]->creaTexto(true, listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), listaVentanas[idVentana]->textosCreados[idTexto]->getNumeroPanel(), listaVentanas[idVentana]->textosCreados[idTexto]->getPosX(),
                    listaVentanas[idVentana]->textosCreados[idTexto]->getPosY(), listaVentanas[idVentana]->textosCreados[idTexto]->getValorTexto());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), idTexto);

            }
        }
        // MEDIO - DERECHA
        else if (contadorVentanas > 5 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x > tamDivisionX * 2 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x < tamDivisionX * 3
            && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y > tamDivisionY && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y < tamDivisionY * 2)
        {
            int ventana = 5;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->textos.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaTexto(ventanas[idVentana]->textos[idTexto]->getNombre(), ventanas[idVentana]->textos[idTexto]->getValorTexto(), ventanas[idVentana]->textos[idTexto]->getPosX(), ventanas[idVentana]->textos[idTexto]->getPosY());

                listaVentanas[ventana]->creaTexto(true, listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), listaVentanas[idVentana]->textosCreados[idTexto]->getNumeroPanel(), listaVentanas[idVentana]->textosCreados[idTexto]->getPosX(),
                    listaVentanas[idVentana]->textosCreados[idTexto]->getPosY(), listaVentanas[idVentana]->textosCreados[idTexto]->getValorTexto());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), idTexto);

            }
        }
        // BAJO - IZQUIERDA
        else if (contadorVentanas > 6 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x > 0 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x < tamDivisionX
            && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y > tamDivisionY * 2 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y < tamDivisionY * 3)
        {
            int ventana = 6;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->textos.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaTexto(ventanas[idVentana]->textos[idTexto]->getNombre(), ventanas[idVentana]->textos[idTexto]->getValorTexto(), ventanas[idVentana]->textos[idTexto]->getPosX(), ventanas[idVentana]->textos[idTexto]->getPosY());

                listaVentanas[ventana]->creaTexto(true, listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), listaVentanas[idVentana]->textosCreados[idTexto]->getNumeroPanel(), listaVentanas[idVentana]->textosCreados[idTexto]->getPosX(),
                    listaVentanas[idVentana]->textosCreados[idTexto]->getPosY(), listaVentanas[idVentana]->textosCreados[idTexto]->getValorTexto());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), idTexto);

            }

        }
        // BAJO - MEDIO
        else if (contadorVentanas > 7 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x > tamDivisionX && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x < tamDivisionX * 2
            && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y > tamDivisionY * 2 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y < tamDivisionY * 3)
        {
            int ventana = 7;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->textos.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaTexto(ventanas[idVentana]->textos[idTexto]->getNombre(), ventanas[idVentana]->textos[idTexto]->getValorTexto(), ventanas[idVentana]->textos[idTexto]->getPosX(), ventanas[idVentana]->textos[idTexto]->getPosY());

                listaVentanas[ventana]->creaTexto(true, listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), listaVentanas[idVentana]->textosCreados[idTexto]->getNumeroPanel(), listaVentanas[idVentana]->textosCreados[idTexto]->getPosX(),
                    listaVentanas[idVentana]->textosCreados[idTexto]->getPosY(), listaVentanas[idVentana]->textosCreados[idTexto]->getValorTexto());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), idTexto);

            }
        }

        // BAJO - DERECHA
        else if (contadorVentanas > 8 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x > tamDivisionX * 2 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().x < tamDivisionX * 3
            && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y > tamDivisionY * 2 && ventanas[idVentana]->textos[idTexto]->GetScreenPosition().y < tamDivisionY * 3)
        {
            int ventana = 8;

            if (idVentana == ventana)
            {
                //MISMA VENTANA. NO HACE NADA.
                cout << "Misma Ventana." << endl;
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;
                cout << "Size Ventana Origen " << idVentana << ": " << ventanas[idVentana]->textos.size() << endl;
            }
            else
            {
                //DISTINTA VENTANA. HACE.
                cout << "Texto " << idTexto << " en Ventana " << ventana << endl;

                ventanas[ventana]->creaTexto(ventanas[idVentana]->textos[idTexto]->getNombre(), ventanas[idVentana]->textos[idTexto]->getValorTexto(), ventanas[idVentana]->textos[idTexto]->getPosX(), ventanas[idVentana]->textos[idTexto]->getPosY());

                listaVentanas[ventana]->creaTexto(true, listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), listaVentanas[idVentana]->textosCreados[idTexto]->getNumeroPanel(), listaVentanas[idVentana]->textosCreados[idTexto]->getPosX(),
                    listaVentanas[idVentana]->textosCreados[idTexto]->getPosY(), listaVentanas[idVentana]->textosCreados[idTexto]->getValorTexto());

                listaVentanas[idVentana]->borraItem(listaVentanas[idVentana]->textosCreados[idTexto]->getNombre(), idTexto);
            }
        }
    }


    void ventanaPantallas::creaPanel(wxPanel *padre, int nPadre, bool carga, wxString nombrePanel, int numPaneles, int posXPanel, int posYPanel, int tamXPanel, int tamYPanel, int colorRPanel, int colorGPanel, int colorBPanel, wxString urlImagenPanel, bool hayImagenPanel, bool movidoGlobal, bool imagenPadre)
    {
        int ventanaPadre = padre->GetId();
        ventanas[nPadre]->creaPanel(carga, nombrePanel, numPaneles, posXPanel * 0.3, posYPanel * 0.3, tamXPanel * 0.3, tamYPanel * 0.3, colorRPanel, colorGPanel, colorBPanel, urlImagenPanel, hayImagenPanel, movidoGlobal, imagenPadre);
    }


    void ventanaPantallas::creaTexto(wxPanel* padre, int nPadre, wxString nombre, int posXPanel, int posYPanel, wxString valorTexto)
    {

        int ventanaPadre = padre->GetId();
        ventanas[nPadre]->creaTexto(nombre, valorTexto, posXPanel*0.3, posYPanel*0.3);
    }


    void ventanaPantallas::cargaVentanas(wxString path)
    {
        wxFileInputStream input(path);
        wxTextInputStream texto(input, wxT("\x09"), wxConvUTF8);

        int contador = -1;

        for (int i = 0; i < contadorVentanas; i++)
        {
            ventanas[i]->Destroy();

            panelFondo->Update();
            panelFondo->Refresh();

            this->Update();
            this->Refresh();

            Update();
            Refresh();
        }

        contadorVentanas = 0;
        ventanas.clear();


        while (input.IsOk() && !input.Eof())
        {
            wxString linea = texto.ReadLine();

            if (linea == "Ventana")
            {
                wxString lineaDatosNumero = texto.ReadLine();
                int nVentanaArchivo = wxAtoi(lineaDatosNumero);

                wxString lineaTamX = texto.ReadLine();
                int globalTamX = wxAtoi(lineaTamX);

                wxString lineaTamY = texto.ReadLine();
                int globalTamY = wxAtoi(lineaTamY);

                wxString nombreVentanaArchivo = texto.ReadLine();

                wxString lineaDatosImagen = texto.ReadLine();
                bool hayImagenVentana;

                if (lineaDatosImagen == "0")
                {
                    hayImagenVentana = false;
                }
                else
                {
                    hayImagenVentana = true;
                }

                wxString urlImagenVentanaArchivo = texto.ReadLine();

                wxString lineaDatosNumeroElementos = texto.ReadLine();
                int nElementosVentana = wxAtoi(lineaDatosNumeroElementos);


                if (hayImagenVentana)
                {
                    creaVentanaImagen(urlImagenVentanaArchivo, globalTamX, globalTamY);
                    contador++;
                }
                else
                {
                    creaVentana(globalTamX, globalTamY);
                    contador++;
                }

                for (int j = 0; j < nElementosVentana; j++)
                {

                    wxString linea = texto.ReadLine();

                    if (linea == "Panel")
                    {
                        wxString lineaDatosNumero = texto.ReadLine();
                        int nObjetoArchivo = wxAtoi(lineaDatosNumero);

                        wxString nombreObjetoArchivo = texto.ReadLine();

                        wxString lineaDatosPosX = texto.ReadLine();
                        int posXArchivo = wxAtoi(lineaDatosPosX);

                        wxString lineaDatosPosY = texto.ReadLine();
                        int posYArchivo = wxAtoi(lineaDatosPosY);

                        wxString lineaDatosTamX = texto.ReadLine();
                        int tamXArchivo = wxAtoi(lineaDatosTamX);

                        wxString lineaDatosTamY = texto.ReadLine();
                        int tamYArchivo = wxAtoi(lineaDatosTamY);

                        wxString lineaDatosColorR = texto.ReadLine();
                        int colorRArchivo = wxAtoi(lineaDatosColorR);

                        wxString lineaDatosColorG = texto.ReadLine();
                        int colorGArchivo = wxAtoi(lineaDatosColorG);

                        wxString lineaDatosColorB = texto.ReadLine();
                        int colorBArchivo = wxAtoi(lineaDatosColorB);

                        wxString urlImagenArchivo = texto.ReadLine();

                        wxString lineaDatosImagen = texto.ReadLine();
                        bool hayImagenArchivo;

                        if (lineaDatosImagen == "0")
                        {
                            hayImagenArchivo = false;
                        }
                        else
                        {
                            hayImagenArchivo = true;
                        }

                        creaPanel(ventanas[contador], contador, true, nombreObjetoArchivo, nObjetoArchivo, posXArchivo, posYArchivo, tamXArchivo, tamYArchivo, colorRArchivo, colorGArchivo, colorBArchivo, urlImagenArchivo, hayImagenArchivo, false, hayImagenVentana);

                    }

                    else if (linea == "Texto")
                    {

                        //cout << "entra crea texto" << endl;

                        wxString lineaDatosNombre = texto.ReadLine();
                        int nObjetoArchivo = wxAtoi(lineaDatosNombre);

                        wxString nombreObjetoArchivo = texto.ReadLine();

                        wxString valorObjetoArchivo = texto.ReadLine();

                        wxString lineaDatosPosX = texto.ReadLine();
                        int posXArchivo = wxAtoi(lineaDatosPosX);

                        wxString lineaDatosPosY = texto.ReadLine();
                        int posYArchivo = wxAtoi(lineaDatosPosY);

                        creaTexto(ventanas[contador], contador, nombreObjetoArchivo, posXArchivo, posYArchivo, valorObjetoArchivo);
                    }
                }
            }
        }
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class MyApp : public wxApp
{
    public:
    
    virtual bool OnInit() wxOVERRIDE;

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class MyFrame : public wxFrame
{

    public:

        int contadorVentanas = 0;
        int contadorVentanasGeneral = 0;
        int numeroVentanas = 1;
        int contadorPanelesGeneral = 0;
        int limiteVentanas;

        wxString CurrentDocPath;
        wxSize sizeVentana;
        wxSize tamNotebook;
        wxSize tamVentana;
        wxAuiNotebook* ctrl;
        ventanaPantallas* ventanaPant;
        ventanaHUD* crearVentana;
        
        wxVector<ventanaHUD*> ventanasCreadas;


        // event handlers (these functions should _not_ be virtual)
        MyFrame(wxWindow*, wxWindowID, const wxString&, const wxPoint&, const wxSize&, long);

        void creaPanel(int, bool, wxString, int, int, int, int, int, int, int, int, wxString, bool);
        void creaTexto(int, bool, wxString, int, int, int, wxString);
        void creaVentana(bool, wxString, bool, wxString, wxSize);
        void guardaTodo(wxString);
        void cargaTodo(wxString);
        void importa3Pantallas(wxString);

        //void LimiteVentanas(wxCommandEvent& event);

        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void creaPanelMenu(wxCommandEvent& event);
        void creaTextoMenu(wxCommandEvent& event);
        void creaElementoMenu(wxCommandEvent& event);
        void creaVentanaMenu(wxCommandEvent& event);
        void cambiaFondoPantallaMenu(wxCommandEvent& event);
        void quitaFondoPantallaMenu(wxCommandEvent& event);
        void guardaMenu(wxCommandEvent& event);
        void guardaTodoMenu(wxCommandEvent& event);
        void cargaMenu(wxCommandEvent& event);
        void cargaTodoMenu(wxCommandEvent& event);
        void ActivaVistaVentanas(wxCommandEvent& event);
        void ImportarLayout(wxCommandEvent& event);
        void ImportarDisplay(wxCommandEvent& event);

        void cierraVentanasGlobal(wxCloseEvent& event);

        void OnNotebookPageClose(wxAuiNotebookEvent& evt);
        void OnNotebookPageClosed(wxAuiNotebookEvent& evt);
        void OnNotebookPageChanging(wxAuiNotebookEvent& evt);
        void OnNotebookPageChanged(wxAuiNotebookEvent& evt);

        wxTreeCtrl* CreateTreeCtrl();
        wxAuiNotebook* CreateNotebook(wxString);


    private:

        long m_notebook_style;
        long m_notebook_theme;
        wxAuiManager m_mgr;


        wxDECLARE_EVENT_TABLE();
};


    enum
    {
        Quit = wxID_EXIT,
        About = wxID_ABOUT,

        //menu item creados
        Panel = 100,
        Texto = 101,
        GridChange = 102,
        GridChanging = 103,
        Texto2 = 104,
        CambiaPantallas = 105,
        CambiaFondoPantallaMenu = 106,
        QuitaFondoPantallaMenu = 107,
        GuardarMenu = 108,
        CargarMenu = 109,
        Ventana = 110,
        GuardarTodoMenu = 111,
        CargarTodoMenu = 112,
        vistaVentanas = 113,
        ImportarLayoutMenu = 114,
        ImportarDisplayMenu = 115,
        Elemento = 116
        //maximasVentanas = 116

    };

    // ----------------------------------------------------------------------------
    // event tables and other macros for wxWidgets
    // ----------------------------------------------------------------------------

        // the event tables connect the wxWidgets events with the functions (event
        // handlers) which process them. It can be also done at run-time, but for the
        // simple menu events like this the static method is much simpler.

    wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
        EVT_MENU(Quit, MyFrame::OnQuit)
        EVT_MENU(About, MyFrame::OnAbout)
        EVT_MENU(Panel, MyFrame::creaPanelMenu)
        EVT_MENU(Texto, MyFrame::creaTextoMenu)
        EVT_MENU(Elemento, MyFrame::creaElementoMenu)
        EVT_MENU(Ventana, MyFrame::creaVentanaMenu)
        EVT_MENU(CambiaFondoPantallaMenu, MyFrame::cambiaFondoPantallaMenu)
        EVT_MENU(QuitaFondoPantallaMenu, MyFrame::quitaFondoPantallaMenu)
        EVT_MENU(GuardarMenu, MyFrame::guardaMenu)
        EVT_MENU(CargarMenu, MyFrame::cargaMenu)
        EVT_MENU(GuardarTodoMenu, MyFrame::guardaTodoMenu)
        EVT_MENU(CargarTodoMenu, MyFrame::cargaTodoMenu)
        EVT_MENU(vistaVentanas, MyFrame::ActivaVistaVentanas)
        EVT_MENU(ImportarLayoutMenu, MyFrame::ImportarLayout)
        EVT_MENU(ImportarDisplayMenu, MyFrame::ImportarDisplay)
        //EVT_MENU(maximasVentanas, MyFrame::LimiteVentanas)

        EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MyFrame::OnNotebookPageClose)
        EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, MyFrame::OnNotebookPageClosed)
        EVT_AUINOTEBOOK_PAGE_CHANGING(wxID_ANY, MyFrame::OnNotebookPageChanging)
        EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, MyFrame::OnNotebookPageChanged)
    wxEND_EVENT_TABLE()


// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)

wxIMPLEMENT_APP(MyApp);

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if (!wxApp::OnInit())
        return false;

    // create the main application window
    MyFrame* frame = new MyFrame(NULL, wxID_ANY, "HUDApp", wxDefaultPosition, wxWindow::FromDIP(wxSize(800, 600), NULL), wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);
    frame->EnableMaximizeButton(false);

    wxDisplay display;
    wxRect screen = display.GetClientArea();
    wxSize tam = wxSize(screen.width, screen.height);

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

// frame constructor
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
    CreateConsole();

    limiteVentanas = 9;

    ventanaPant = new ventanaPantallas(NULL, wxID_ANY, "Vista de Ventanas Global", wxDefaultPosition, wxWindow::FromDIP(tamDisplay, NULL), limiteVentanas, ventanasCreadas);
    ventanaPant->Show(false);
    ventanaPant->Maximize(true);
    ventanaPant->EnableMaximizeButton(false);
    ventanaPant->EnableMinimizeButton(false);
    ventanaPant->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrame::cierraVentanasGlobal), NULL, this);

    wxDisplay display(wxDisplay::GetFromWindow(this));

    wxRect screen = display.GetClientArea();
    tamVentana = wxSize(screen.width, screen.height);

    cout << "tamVentanaX: " << screen.width << endl;
    cout << "tamVentanaY: " << screen.height << endl;


    // tell wxAuiManager to manage this frame
    m_mgr.SetManagedWindow(this);

    // set frame icon
    //SetIcon(wxIcon(sample_xpm));

    // set up default notebook style
    m_notebook_style = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;
    m_notebook_theme = 0;

    tamVentanaX = screen.width;
    tamVentanaY = screen.height;

    this->SetSize(tamVentanaX, tamVentanaY);

    // set the frame icon
    SetIcon(wxICON(sample));

    //Initilize every Image Handler Type
    wxInitAllImageHandlers();

    this->Maximize(true);

    contadorVentanas = 0;
    numeroVentanas = 1;

   //////////// MENÚ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // create a menu bar
    wxMenu* archivoMenu = new wxMenu;
    wxMenu* vistaMenu = new wxMenu;
    wxMenu* componentesMenu = new wxMenu;
    wxMenu* ayudaMenu = new wxMenu;

    archivoMenu->Append(GuardarMenu, "Guardar Ventana Actual", "Guardar HUD");
    archivoMenu->Append(GuardarTodoMenu, "Guardar Todas las Ventanas", "Guardar HUD");
    archivoMenu->Append(CargarMenu, "Cargar una Distribución en esta Ventana", "Cargar HUD existente");
    archivoMenu->Append(CargarTodoMenu, "Cargar Varias Ventanas", "Cargar HUD de Varias Ventanas");
    archivoMenu->Append(ImportarLayoutMenu, "Importar Layout de Ventanas", "Importar un Layout de Ventanas Externo");
    archivoMenu->Append(ImportarDisplayMenu, "Importar Layout del Dispositivo", "Importar el Layout de tu Sistema");
    archivoMenu->Append(Quit, "Salir\tAlt-X", "Salir del Programa");

    vistaMenu->Append(Ventana, "Crear una Nueva Ventana", "Crea una nueva Ventana para añadir elementos");
    vistaMenu->Append(CambiaFondoPantallaMenu, "Cambiar Fondo de Ventana", "Cambia el fondo de la pantalla de Vista");
    vistaMenu->Append(QuitaFondoPantallaMenu, "Quitar Fondo de Ventana", "Elimina el fondo de la pantalla de Vista");
    vistaMenu->Append(vistaVentanas, "Vista Global de Ventanas Creadas", "Muestra una Perspectiva Global de las Ventanas Creadas");
    //vistaMenu->Append(maximasVentanas, "Establece el límite de Ventanas", "Establece el límite de Ventanas posibles para crear");

    componentesMenu->Append(Panel, "Panel", "Genera un panel para el HUD");
    componentesMenu->Append(Texto, "Texto", "Genera un texto para el HUD");
    componentesMenu->Append(Elemento, "Importar Imagen como Panel", "Importa una Imagen como elemento Panel");

    ayudaMenu->Append(About, "Sobre mí...\tF1", "Muestra al creador de la app");

    // now append the freshly created menu to the menu bar...
    wxMenuBar* barraMenu = new wxMenuBar();

    barraMenu->Append(archivoMenu, "Archivo");
    barraMenu->Append(vistaMenu, "Vista");
    barraMenu->Append(componentesMenu, "Componentes");
    barraMenu->Append(ayudaMenu, "Ayuda");


    // ... and attach this menu bar to the frame
    SetMenuBar(barraMenu);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    /////////// BARRA STATUS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Crea una Status Bar
    CreateStatusBar(2);
    SetStatusText("¡Bienvenido a la HUDApp!");
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // min size for the frame itself isn't completely done.
    // see the end up wxAuiManager::Update() for the test
    // code. For now, just hard code a frame minimum size
    SetMinSize(FromDIP(wxSize(400,300)));


    // Añade el Panel Central de Vista.
    tamNotebook = wxSize(screen.width * 0.8, screen.height * 0.9);
    ctrl = new wxAuiNotebook(this, wxID_ANY, wxPoint(0, 0), FromDIP(wxSize(tamNotebook.x, tamNotebook.y)), m_notebook_style);


    // Guarda los cambios en al AuiManager
    m_mgr.Update();
}


void MyFrame::cierraVentanasGlobal(wxCloseEvent& event)
{   
    ventanaPant->Destroy();

    ventanaPant = new ventanaPantallas(NULL, wxID_ANY, "Vista de Ventanas Global", wxDefaultPosition, wxWindow::FromDIP(tamDisplay, NULL), limiteVentanas, ventanasCreadas);
    ventanaPant->Show(false);
    ventanaPant->Maximize(true);
    ventanaPant->EnableMaximizeButton(false);
    ventanaPant->EnableMinimizeButton(false);
    ventanaPant->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrame::cierraVentanasGlobal), NULL, this);

    Update();
    Refresh();
}


/*void MyFrame::LimiteVentanas(wxCommandEvent& event)
{
    wxNumberEntryDialog dialog(this, wxT("This is some text, actually a lot of text\nEven two rows of text"), wxT("Enter a number:"), wxT("Numeric input test"), 50, 0, 100); if (dialog.ShowModal() == wxID_OK)
    {
        limiteVentanas = dialog.GetValue();
    }
}*/


void MyFrame::ActivaVistaVentanas(wxCommandEvent& event)
{
    if (contadorVentanas == 0)
    {
        int res = wxMessageBox("No hay ninguna Ventana Creada. No puedes ver la Vista Global.", "Cuidado", wxOK, this);
    }
    else 
    {
        ventanaPant->listaVentanas = ventanasCreadas;

        guardaTodo("ventanasTemp.txt");

        ventanaPant->cargaVentanas("ventanasTemp.txt");

        ventanaPant->Show(true);

        Update();
        Refresh();
    }
}


void MyFrame::creaVentanaMenu(wxCommandEvent& event)
{  
        creaVentana(false, "Nada", false, "", tamVentana);

        ventanaPant->Update();
        ventanaPant->Refresh();

        Update();
        Refresh();
}


void MyFrame::creaVentana(bool cargando, wxString nombre, bool hayImagenVista, wxString urlImagen, wxSize tamVentana)
{
    if (contadorVentanas < limiteVentanas)
    {
        if(cargando)
        {
            crearVentana = new ventanaHUD(this, &m_mgr, ctrl, nombre, contadorVentanasGeneral, hayImagenVista, urlImagen, tamNotebook, tamVentana);
        }
        else 
        {
            crearVentana = new ventanaHUD(this, &m_mgr, ctrl, "Pantalla " + wxString::Format(wxT("%d"), contadorVentanasGeneral+1), contadorVentanasGeneral, tamNotebook, tamVentana);
        }
    
        ventanasCreadas.insert(ventanasCreadas.begin() + contadorVentanas, crearVentana);

        m_mgr.Update();

        ctrl->SetSelection(contadorVentanas);

        contadorVentanas++;
        numeroVentanas++;
        contadorVentanasGeneral++;
    }
    else
    {
        int res = wxMessageBox("Has llegado al número máximo de Ventanas. No se pueden crear más", "Límite Superado", wxOK, this);
    }

}


void MyFrame::creaPanelMenu(wxCommandEvent& event)
{

    if (contadorVentanas == 0)
    {
        int res = wxMessageBox("No hay ninguna Ventana Creada. Créala.", "Cuidado", wxOK, this);
    }
    else 
    {
        int id;
        id = ctrl->GetSelection();

        creaPanel(id, false, "Panel " + contadorPanelesGeneral + 1, 0, 0, 0, 0, 0, 0, 0, 0, "NO HAY", false);
    }
}


void MyFrame::creaPanel(int id, bool carga, wxString nombre, int n, int posX, int posY, int tamX, int tamY, int r, int g, int b, wxString url, bool imagen)
{
    ventanasCreadas[id]->creaPanel(carga, nombre, n, posX, posY, tamX, tamY, r, g, b, url, imagen);
    contadorPanelesGeneral++;
}


void MyFrame::creaTextoMenu(wxCommandEvent& event)
{
    if (contadorVentanas == 0)
    {
        int res = wxMessageBox("No hay ninguna Ventana Creada. Créala.", "Cuidado", wxOK, this);
    }
    else 
    {
        int id;
        id = ctrl->GetSelection();

        wxFont fuente = wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

        ventanasCreadas[id]->creaTexto(false, "Texto " + contadorPanelesGeneral + 1, 0, 0, 0, "Lorem Ipsum");
        contadorPanelesGeneral++;
    }
}


void MyFrame::creaTexto(int id, bool carga, wxString nombre, int n, int posX, int posY, wxString valor)
{
    ventanasCreadas[id]->creaTexto(carga, nombre, n, posX, posY, valor);
    contadorPanelesGeneral++;
}


void MyFrame::creaElementoMenu(wxCommandEvent& event)
{
    if (contadorVentanas == 0)
    {
        int res = wxMessageBox("No hay ninguna Ventana Creada. Créala.", "Cuidado", wxOK, this);
    }
    else
    {
        int id;
        id = ctrl->GetSelection();

        wxFileDialog* OpenDialog = new wxFileDialog(this, _("Elige una imagen como elemento"), wxEmptyString, wxEmptyString, "JPG files (*.jpg;)|*.jpg;|BMP files (*.bmp;)|*.bmp;|PNG files (*.png;)|*.png|TIF files (*.tif;)|*.tif;|GIF files (*.gif;)|*.gif;|PNM files (*.pnm;)|*.pnm;|PCX files (*.pcx;)|*.pcx;|ICO files (*.ico;)|*.ico;|CUR files (*.cur;)|*.cur;|ANI files (*.ani;)|*.ani;|TGA files (*.tga;)|*.tga;|XPM files (*.xpm;)|*.xpm;|All files (*.;)|*.;", wxFD_OPEN, wxDefaultPosition);
        wxSize tamElemento;

        // Creates a "open file" dialog with 4 file types
        if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
        {
            CurrentDocPath = OpenDialog->GetPath();
            
            wxBitmap bitMap;
            bitMap.LoadFile(CurrentDocPath, wxBITMAP_TYPE_ANY);

            tamElemento = bitMap.GetSize();

            wxString nombre = "Panel ";
            nombre << (contadorPanelesGeneral + 1);

            cout << nombre;

            creaPanel(id, true, nombre, contadorPanelesGeneral, 0, 0, tamElemento.x, tamElemento.y, 0, 255, 0, CurrentDocPath, true);

        }
    }
}


void MyFrame::cambiaFondoPantallaMenu(wxCommandEvent& WXUNUSED(event))
{
    if (contadorVentanas == 0)
    {
        int res = wxMessageBox("No hay ninguna Ventana Creada. Créala.", "Cuidado", wxOK, this);
    }
    else 
    {
        int id;

        wxFileDialog* OpenDialog = new wxFileDialog(this, _("Elige una imagen de fondo"), wxEmptyString, wxEmptyString, "JPG files (*.jpg;)|*.jpg;|BMP files (*.bmp;)|*.bmp;|PNG files (*.png;)|*.png|TIF files (*.tif;)|*.tif;|GIF files (*.gif;)|*.gif;|PNM files (*.pnm;)|*.pnm;|PCX files (*.pcx;)|*.pcx;|ICO files (*.ico;)|*.ico;|CUR files (*.cur;)|*.cur;|ANI files (*.ani;)|*.ani;|TGA files (*.tga;)|*.tga;|XPM files (*.xpm;)|*.xpm;|All files (*.;)|*.;", wxFD_OPEN, wxDefaultPosition);

        // Creates a "open file" dialog with 4 file types
        if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
        {
            CurrentDocPath = OpenDialog->GetPath();

            id = ctrl->GetSelection();

            ventanasCreadas[id]->cambiaFondoPantalla(CurrentDocPath);

            OpenDialog->Destroy();
        }
    }
}


void MyFrame::quitaFondoPantallaMenu(wxCommandEvent& WXUNUSED(event))
{
    if (contadorVentanas == 0)
    {
        int res = wxMessageBox("No hay ninguna Ventana Creada. Créala.", "Cuidado", wxOK, this);
    }
    else
    {
        int id;
        id = ctrl->GetSelection();

        ventanasCreadas[id]->quitaFondoPantalla();
    }
}


void MyFrame::ImportarLayout(wxCommandEvent& event)
{
    //int res = wxMessageBox("ImportarLayout", "Cuidado", wxOK, this);

    wxFileDialog* OpenDialog = new wxFileDialog(this, _("Selecciona un Archivo de Layout a Cargar"), wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt", wxFD_OPEN, wxDefaultPosition);

    // Creates a "open file" dialog with 4 file types
    if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
    {
        CurrentDocPath = OpenDialog->GetPath();
        importa3Pantallas(CurrentDocPath);
    }

    // Clean up after ourselves
    OpenDialog->Destroy();
}


void MyFrame::importa3Pantallas(wxString path)
{
    wxFileInputStream input(path);
    wxTextInputStream texto(input, wxT("\x09"), wxConvUTF8);
    boolean archivoErroneo = false;

    int contadorVentansActual = contadorVentanasGeneral;

    while (input.IsOk() && !input.Eof())
    {
        wxString lineaVersion = texto.ReadLine();

        if (lineaVersion == "Panel" || lineaVersion == "Texto" || lineaVersion == "Ventana")
        {
            archivoErroneo = true; 
            break;
        }
        else
        {
            wxString lineaConfiguracionActual = texto.ReadLine();
            bool ConfiguracionActual = wxAtoi(lineaConfiguracionActual);

            wxString lineaNumWindows = texto.ReadLine();
            int NumWindows = wxAtoi(lineaNumWindows);

            wxString lineaWindowHUD = texto.ReadLine();
            bool WindowHUD = wxAtoi(lineaWindowHUD);

            wxString lineaExtendedDisplay = texto.ReadLine();
            bool ExtendedDisplay = wxAtoi(lineaExtendedDisplay);


            for (int j = 0; j < NumWindows; j++)
            {
                /////////////////////////////////////////////////////////

                wxString WindowsFullscreen = texto.ReadLine();
                wxStringTokenizer tokenizerWF(WindowsFullscreen, "/");

                wxString windowsString = tokenizerWF.GetNextToken();
                wxString fullscreenString = tokenizerWF.GetNextToken();

                bool Windows = wxAtoi(windowsString);
                bool Fullscreen = wxAtoi(fullscreenString);

                /////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////

                wxString posXY = texto.ReadLine();
                wxStringTokenizer tokenizerPos(posXY, "/");

                wxString posXString = tokenizerPos.GetNextToken();
                wxString posYString = tokenizerPos.GetNextToken();

                int posImportX = wxAtoi(posXString);
                int posImportY = wxAtoi(posYString);

                /////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////

                wxString WidthHeight = texto.ReadLine();
                wxStringTokenizer tokenizerWH(WidthHeight, "/");

                wxString widthString = tokenizerWH.GetNextToken();
                wxString heightString = tokenizerWH.GetNextToken();

                int widthImport = wxAtoi(widthString);
                int heightImport = wxAtoi(heightString);

                /////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////

                wxString decorationString = texto.ReadLine();
                bool decoration = wxAtoi(decorationString);

                /////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////

                wxString trackingString = texto.ReadLine();
                bool trackingView = wxAtoi(trackingString);

                /////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////

                wxString stereoString = texto.ReadLine();
                bool stereoMode = wxAtoi(stereoString);

                /////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////

                wxString topLeft = texto.ReadLine();
                wxStringTokenizer tokenizerTL(topLeft, "/");

                wxString topLeftStringX = tokenizerTL.GetNextToken();
                wxString topLeftStringY = tokenizerTL.GetNextToken();
                wxString topLeftStringZ = tokenizerTL.GetNextToken();

                double topLeftX;
                double topLeftY;
                double topLeftZ;

                topLeftStringX.ToDouble(&topLeftX);
                topLeftStringY.ToDouble(&topLeftY);
                topLeftStringZ.ToDouble(&topLeftZ);

                /////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////

                wxString bottomLeft = texto.ReadLine();
                wxStringTokenizer tokenizerBL(bottomLeft, "/");

                wxString bottomLeftStringX = tokenizerBL.GetNextToken();
                wxString bottomLeftStringY = tokenizerBL.GetNextToken();
                wxString bottomLeftStringZ = tokenizerBL.GetNextToken();

                double bottomLeftX;
                double bottomLeftY;
                double bottomLeftZ;

                bottomLeftStringX.ToDouble(&bottomLeftX);
                bottomLeftStringY.ToDouble(&bottomLeftY);
                bottomLeftStringZ.ToDouble(&bottomLeftZ);

                /////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////

                wxString bottomRight = texto.ReadLine();
                wxStringTokenizer tokenizerBR(bottomRight, "/");

                wxString bottomRightStringX = tokenizerBR.GetNextToken();
                wxString bottomRightStringY = tokenizerBR.GetNextToken();
                wxString bottomRightStringZ = tokenizerBR.GetNextToken();

                double bottomRightX;
                double bottomRightY;
                double bottomRightZ;

                bottomRightStringX.ToDouble(&bottomRightX);
                bottomRightStringY.ToDouble(&bottomRightY);
                bottomRightStringZ.ToDouble(&bottomRightZ);

                /////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////

                wxString externalString = texto.ReadLine();
                bool externalView = wxAtoi(externalString);

                /////////////////////////////////////////////////////////
                /////////////////////////////////////////////////////////

                wxString borderSizes = texto.ReadLine();
                wxStringTokenizer tokenizerBS(borderSizes, "/");

                wxString borderSizesStringX = tokenizerBS.GetNextToken();
                wxString borderSizesStringY = tokenizerBS.GetNextToken();
                wxString borderSizesStringZ = tokenizerBS.GetNextToken();
                wxString borderSizesStringW = tokenizerBS.GetNextToken();

                double borderSizesX;
                double borderSizesY;
                double borderSizesZ;
                double borderSizesW;

                borderSizesStringX.ToDouble(&borderSizesX);
                borderSizesStringY.ToDouble(&borderSizesY);
                borderSizesStringZ.ToDouble(&borderSizesZ);
                borderSizesStringW.ToDouble(&borderSizesW);

                /////////////////////////////////////////////////////////

                wxString nombreImport = "Ventana Import ";
                nombreImport << j + 1;

                creaVentana(true, nombreImport, false, "", wxSize(widthImport, heightImport));
            }
        }
    }

    if(archivoErroneo)
    {
        int res = wxMessageBox("No has Cargado un tipo de archivo apropiado. Inténtalo otra vez.", "Error de Archivo", wxOK, this);
    }
}


void MyFrame::ImportarDisplay(wxCommandEvent& event)
{
    //int res = wxMessageBox("ImportarDisplay", "Cuidado", wxOK, this);

    int displayCount = wxDisplay::GetCount();

    cout << "displayCount: " << displayCount << endl;

    for (int d = 0; d < displayCount; d++)
    {
        wxDisplay display(d);
        wxRect screen = display.GetClientArea();
        wxSize displaySize = wxSize(screen.width, screen.height);

        creaVentana(true, display.GetName(), false, "", displaySize);
    }
}


void MyFrame::guardaMenu(wxCommandEvent& WXUNUSED(event))
{
    int id = ctrl->GetSelection();

    if (contadorVentanas == 0)
    {
        int res = wxMessageBox("No hay ninguna Ventana Creada. Créala.", "Cuidado", wxOK, this);
    }
    else if (ventanasCreadas[id]->panelesCreados.size() == 0 && ventanasCreadas[id]->textosCreados.size() == 0)
    {
        int res = wxMessageBox("No hay ningún elemento en la Ventana Creada. Añade alguno.", "Cuidado", wxOK, this);
    }
    else
    {
        wxFileDialog* saveDialog = new wxFileDialog(this, _("Selecciona una ubicación para guardar el Archivo"), wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        // Creates a "save file" dialog with 4 file types
        if (saveDialog->ShowModal() == wxID_OK) // if the user click "Save" instead of "Cancel"
        {
            CurrentDocPath = saveDialog->GetPath();
            int id = ctrl->GetSelection();

            ventanasCreadas[id]->guardaVentana(CurrentDocPath);
        }

        // Clean up after ourselves
        saveDialog->Destroy();
    }
}


void MyFrame::guardaTodoMenu(wxCommandEvent& WXUNUSED(event))
{
    if (contadorVentanas == 0)
    {
        int res = wxMessageBox("No hay ninguna Ventana Creada. Créala.", "Cuidado", wxOK, this);
    }
    else
    {
        wxFileDialog* saveDialog = new wxFileDialog(this, _("Selecciona una ubicación para guardar el Archivo"), wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        // Creates a "save file" dialog with 4 file types
        if (saveDialog->ShowModal() == wxID_OK) // if the user click "Save" instead of "Cancel"
        {
            CurrentDocPath = saveDialog->GetPath();
            guardaTodo(CurrentDocPath);
        }

        // Clean up after ourselves
        saveDialog->Destroy();
    }
}

void MyFrame::guardaTodo(wxString path)
{
    wxFile file(path, wxFile::write);

    if (file.IsOpened())
    {
        for (int j = 0; j < contadorVentanas; j++)
        {
            file.Write("Ventana\n");                                                                                // Tipo

            file.Write(wxString::Format(wxT("%d"), ventanasCreadas[j]->getNumVentana()) + "\n");                    // Número de Ventana

            int tamX = ventanasCreadas[j]->getTamVentana().x;
            int tamY = ventanasCreadas[j]->getTamVentana().y;

            file.Write(wxString::Format(wxT("%d"), tamX) + "\n");                                                   // Tamaño en X de la Ventana
            file.Write(wxString::Format(wxT("%d"), tamY) + "\n");                                                   // Tamaño en Y de la Ventana

            wxString nombre = ventanasCreadas[j]->getNombreVentana();

            if (nombre == "Pantalla " + wxString::Format(wxT("%d"), ventanasCreadas[j]->getNumVentana()))
            {
                wxString nombreVentana = wxString::Format(wxT("%d"), ventanasCreadas[j]->getNumVentana() + 1);      // Nombre Ventana
                file.Write("Pantalla " + nombreVentana);                                                            
                file.Write("\n");
            }
            else 
            {
                file.Write(ventanasCreadas[j]->getNombreVentana() + "\n");                                          // Nombre Ventana si no entra en conflicto el nombre con la ventana por defecto.
            }
            file.Write(wxString::Format(wxT("%d"), ventanasCreadas[j]->getImagenVista()) + "\n");                   // Si tiene Imagen de Fondo la Ventana
            file.Write(ventanasCreadas[j]->getURLImagenVista() + "\n");                                             // URL de la IMAGEN; si no hay: "NO HAY"
            file.Write(wxString::Format(wxT("%d"), ventanasCreadas[j]->getContadorElementosJerarquia()) + "\n");    // Número de Elementos Pintados en la Ventana


            for (int i = 0; i < ventanasCreadas[j]->getContadorElementosJerarquia(); i++)
            {
                wxString nombre = ventanasCreadas[j]->getArbolJerarquia()->GetItemText(i);

                for (int k = 0; k < ventanasCreadas[j]->getContadorPaneles(); k++)
                {
                    if (ventanasCreadas[j]->panelesCreados[k]->getNombre() == nombre)
                    {
                        file.Write("Panel\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->panelesCreados[k]->getNumeroPanel()) + "\n");
                        file.Write(ventanasCreadas[j]->panelesCreados[k]->getNombre() + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->panelesCreados[k]->getPosX()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->panelesCreados[k]->getPosY()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->panelesCreados[k]->getTamX()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->panelesCreados[k]->getTamY()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->panelesCreados[k]->getR()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->panelesCreados[k]->getG()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->panelesCreados[k]->getB()) + "\n");
                        file.Write(ventanasCreadas[j]->panelesCreados[k]->getURLImagen() + "\n");
                        file.Write(wxString::Format(wxT("%d"), ventanasCreadas[j]->panelesCreados[k]->getHayImagen()) + "\n");
                        break;
                    }
                }

                for (int k = 0; k < ventanasCreadas[j]->getContadorTextos(); k++)
                {
                    if (ventanasCreadas[j]->textosCreados[k]->getNombre() == nombre)
                    {
                        file.Write("Texto\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->textosCreados[k]->getNumeroPanel()) + "\n");
                        file.Write(ventanasCreadas[j]->textosCreados[k]->getNombre() + "\n");
                        file.Write(ventanasCreadas[j]->textosCreados[k]->getValorTexto() + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->textosCreados[k]->getPosX()) + "\n");
                        file.Write(wxString::Format(wxT("%d"), (int)ventanasCreadas[j]->textosCreados[k]->getPosY()) + "\n");
                        break;
                    }
                }
            }
        }
    }

    file.Close();
}


void MyFrame::cargaMenu(wxCommandEvent& WXUNUSED(event))
{
    if (contadorVentanas == 0)
    {
        int res = wxMessageBox("No hay ninguna Ventana Creada. Créala.", "Cuidado", wxOK, this);
    }
    else 
    {
        wxFileDialog* OpenDialog = new wxFileDialog(this, _("Selecciona un archivo para Cargar una Distribución"), wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt", wxFD_OPEN, wxDefaultPosition);

        // Creates a "open file" dialog with 4 file types
        if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
        {
            CurrentDocPath = OpenDialog->GetPath();
            int id = ctrl->GetSelection();

            ventanasCreadas[id]->cargaVentana(CurrentDocPath);
        }

        // Clean up after ourselves
        OpenDialog->Destroy();
    }
}


void MyFrame::cargaTodoMenu(wxCommandEvent& WXUNUSED(event))
{
    if (contadorVentanas < limiteVentanas)
    {
        wxFileDialog* OpenDialog = new wxFileDialog(this, _("Selecciona un archivo para Cargar Ventanas"), wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt", wxFD_OPEN, wxDefaultPosition);

        // Creates a "open file" dialog with 4 file types
        if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
        {
            CurrentDocPath = OpenDialog->GetPath();
            int id = ctrl->GetSelection();

            cargaTodo(CurrentDocPath);
        }

        // Clean up after ourselves
        OpenDialog->Destroy();
    }
    else
    {
        int res = wxMessageBox("No puedes cargar ventanas nuevas. Has superado el límite.", "Límite Superado", wxOK, this);
    }
}

void MyFrame::cargaTodo(wxString path)
{

    wxFileInputStream input(path);
    wxTextInputStream texto(input, wxT("\x09"), wxConvUTF8);
    boolean archivoErroneo = false;

    int contadorVentansActual = contadorVentanasGeneral;

    while (input.IsOk() && !input.Eof())
    {
            wxString linea = texto.ReadLine();

            if (linea != "Ventana" && !input.Eof())
            {
                archivoErroneo = true;
                break;
            }
            else if (linea == "Ventana")
            {
                wxString lineaDatosNumero = texto.ReadLine();
                int nVentanaArchivo = wxAtoi(lineaDatosNumero);
                nVentanaArchivo = nVentanaArchivo + contadorVentansActual;

                wxString lineaDatosTamX = texto.ReadLine();
                int tamVentanaX = wxAtoi(lineaDatosTamX);

                wxString lineaDatosTamY = texto.ReadLine();
                int tamVentanaY = wxAtoi(lineaDatosTamY);

                wxString nombreVentanaArchivo = texto.ReadLine();

                wxString lineaDatosImagen = texto.ReadLine();
                bool hayImagenVentana;

                if (lineaDatosImagen == "0")
                {
                    hayImagenVentana = false;
                }
                else
                {
                    hayImagenVentana = true;
                }

                wxString urlImagenVentanaArchivo = texto.ReadLine();

                wxString lineaDatosNumeroElementos = texto.ReadLine();
                int nElementosVentana = wxAtoi(lineaDatosNumeroElementos);

            
            if (contadorVentanas < limiteVentanas)
            {
                creaVentana(true, nombreVentanaArchivo, hayImagenVentana, urlImagenVentanaArchivo, wxSize(tamVentanaX, tamVentanaY));


                for (int j = 0; j < nElementosVentana; j++)
                {

                    wxString linea = texto.ReadLine();

                    if (linea == "Panel")
                    {
                        wxString lineaDatosNumero = texto.ReadLine();
                        int nObjetoArchivo = wxAtoi(lineaDatosNumero);

                        wxString nombreObjetoArchivo = texto.ReadLine();

                        wxString lineaDatosPosX = texto.ReadLine();
                        int posXArchivo = wxAtoi(lineaDatosPosX);

                        wxString lineaDatosPosY = texto.ReadLine();
                        int posYArchivo = wxAtoi(lineaDatosPosY);

                        wxString lineaDatosTamX = texto.ReadLine();
                        int tamXArchivo = wxAtoi(lineaDatosTamX);

                        wxString lineaDatosTamY = texto.ReadLine();
                        int tamYArchivo = wxAtoi(lineaDatosTamY);

                        wxString lineaDatosColorR = texto.ReadLine();
                        int colorRArchivo = wxAtoi(lineaDatosColorR);

                        wxString lineaDatosColorG = texto.ReadLine();
                        int colorGArchivo = wxAtoi(lineaDatosColorG);

                        wxString lineaDatosColorB = texto.ReadLine();
                        int colorBArchivo = wxAtoi(lineaDatosColorB);

                        wxString urlImagenArchivo = texto.ReadLine();

                        wxString lineaDatosImagen = texto.ReadLine();
                        bool hayImagenArchivo;

                        if (lineaDatosImagen == "0")
                        {
                            hayImagenArchivo = false;
                        }
                        else
                        {
                            hayImagenArchivo = true;
                        }

                        creaPanel(contadorVentanasGeneral - 1, true, nombreObjetoArchivo, nObjetoArchivo, posXArchivo, posYArchivo, tamXArchivo, tamYArchivo, colorRArchivo, colorGArchivo, colorBArchivo, urlImagenArchivo, hayImagenArchivo);
                    }
                    else if (linea == "Texto")
                    {
                        wxString lineaDatosNombre = texto.ReadLine();
                        int nObjetoArchivo = wxAtoi(lineaDatosNombre);

                        wxString nombreObjetoArchivo = texto.ReadLine();

                        wxString valorObjetoArchivo = texto.ReadLine();

                        wxString lineaDatosPosX = texto.ReadLine();
                        int posXArchivo = wxAtoi(lineaDatosPosX);

                        wxString lineaDatosPosY = texto.ReadLine();
                        int posYArchivo = wxAtoi(lineaDatosPosY);

                        creaTexto(contadorVentanasGeneral - 1, true, nombreObjetoArchivo, nObjetoArchivo, posXArchivo, posYArchivo, valorObjetoArchivo);
                    }
                }
            }
            else
            {
                int res = wxMessageBox("No puedes cargar ventanas nuevas. Has superado el límite.", "Límite Superado", wxOK, this);
                break;
            }
        }
    }

    if (archivoErroneo)
    {
        int res = wxMessageBox("No has Cargado un tipo de archivo apropiado. Inténtalo otra vez.", "Error de Archivo", wxOK, this);
    }
}
   
    


wxTreeCtrl* MyFrame::CreateTreeCtrl()
{
    wxTreeCtrl* tree = new wxTreeCtrl(this, wxID_ANY, wxPoint(0,0), FromDIP(wxSize(160,250)), wxTR_DEFAULT_STYLE | wxNO_BORDER);

    wxSize size = FromDIP(wxSize(16, 16));
    wxImageList* imglist = new wxImageList(size.x, size.y, true, 2);
    imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, size));
    imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, size));
    tree->AssignImageList(imglist);

    wxTreeItemId root = tree->AddRoot("wxAUI Project", 0);
    wxArrayTreeItemIds items;

    items.Add(tree->AppendItem(root, "Item 1", 0));
    items.Add(tree->AppendItem(root, "Item 2", 0));
    items.Add(tree->AppendItem(root, "Item 3", 0));
    items.Add(tree->AppendItem(root, "Item 4", 0));
    items.Add(tree->AppendItem(root, "Item 5", 0));


    int i, count;
    for (i = 0, count = items.Count(); i < count; ++i)
    {
        wxTreeItemId id = items.Item(i);
        tree->AppendItem(id, "Subitem 1", 1);
        tree->AppendItem(id, "Subitem 2", 1);
        tree->AppendItem(id, "Subitem 3", 1);
        tree->AppendItem(id, "Subitem 4", 1);
        tree->AppendItem(id, "Subitem 5", 1);
    }

    tree->Expand(root);

    return tree;
}


wxAuiNotebook* MyFrame::CreateNotebook(wxString nombre)
{
   // create the notebook off-window to avoid flicker
   wxSize client_size = GetClientSize();

   wxAuiNotebook* ctrl = new wxAuiNotebook(this, wxID_ANY, wxPoint(client_size.x, client_size.y), FromDIP(wxSize(430,200)), m_notebook_style);
   return ctrl;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCIONES NOTEBOOK, EXIT y ABOUT /////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MyFrame::OnNotebookPageClose(wxAuiNotebookEvent& evt)
{

    wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
    int id = ctrl->GetSelection();

    wxString arbol = ventanasCreadas[id]->getNombreArbol();
    wxString propiedades = ventanasCreadas[id]->getNombrePropiedades();
    wxString ventana = ventanasCreadas[id]->getNombreVentana();

    wxAuiPaneInfo panelArbol = m_mgr.GetPane(arbol);
    wxAuiPaneInfo panelPropiedades = m_mgr.GetPane(propiedades);
    wxAuiPaneInfo panelVentana = m_mgr.GetPane(ventana);

    int res = wxMessageBox("¿Estás seguro de querer cerrar esta ventana?", "wxAUI", wxYES_NO, this);


    if (contadorVentanas == 1)
    {
        panelArbol.DestroyOnClose(true);
        panelPropiedades.DestroyOnClose(true);

        m_mgr.ClosePane(panelArbol);
        m_mgr.ClosePane(panelPropiedades);
        m_mgr.ClosePane(panelVentana);
       
        m_mgr.Update();
    
        ventanasCreadas.clear();
        contadorVentanas = 0;
        numeroVentanas = 1;

        this->Update();
        this->Refresh();
    }
    else
    {
        if (res != wxYES)
        {
            evt.Veto();
        }
        else
        {
            panelArbol.DestroyOnClose(true);
            panelPropiedades.DestroyOnClose(true);

            m_mgr.ClosePane(panelArbol);
            m_mgr.ClosePane(panelPropiedades);

            ventanasCreadas.erase(ventanasCreadas.begin() + id);
            contadorVentanas--;
            numeroVentanas--;
        }
    }
}


void MyFrame::OnNotebookPageClosed(wxAuiNotebookEvent& evt)
{
    wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
    wxUnusedVar(ctrl);

    // selection should always be a valid index
    wxASSERT_MSG( ctrl->GetSelection() < (int)ctrl->GetPageCount(), wxString::Format("Invalid selection %d, only %d pages left",ctrl->GetSelection(), (int)ctrl->GetPageCount()));

    evt.Skip();
}



void MyFrame::OnNotebookPageChanged(wxAuiNotebookEvent& evt)
{
    if (contadorVentanas > 0)
    {
        wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
        int id = ctrl->GetSelection();


        for (int i = 0; i < contadorVentanas; i++)
        {
            ventanasCreadas[i]->ocultaPanelesInformacion(true);
        }

        ventanasCreadas[id]->ocultaPanelesInformacion(false);

        if (ventanasCreadas[id]->getContadorElementosJerarquia() > 0)
        {
            ventanasCreadas[id]->seleccionaElementoLista(0);
        }
    }
}


void MyFrame::OnNotebookPageChanging(wxAuiNotebookEvent& evt)
{
    ////////////////////////////////////////////////////////
}


void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
    (
        "¡Bienvenido a la HUDApp!\n"
        "\n"
        "Creado por David Lozano Pascual\n"
        "running under %s.",
        wxVERSION_STRING,
        wxGetOsDescription()
    ),
        "Sobre mí...",
        wxOK | wxICON_INFORMATION,
        this);
}


void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}
