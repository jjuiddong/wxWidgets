// wxAppOnAPI.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "wxAppOnAPI.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/evtloop.h"

// Define a new application type
class MyApp: public wxApp
{
public:
	virtual bool OnInit();

	// we need to override this as the default behaviour only works when we're
	// running wxWidgets main loop, not MFC one
	virtual void ExitMainLoop();

	wxFrame *CreateFrame();
};

class MyCanvas: public wxScrolledWindow
{
public:
	MyCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size);
	void OnPaint(wxPaintEvent& event);
	void OnMouseEvent(wxMouseEvent& event);
	DECLARE_EVENT_TABLE()
};

class MyChild: public wxFrame
{
public:
	MyChild(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size, const long style);
	virtual ~MyChild();

	void OnQuit(wxCommandEvent& event);
	void OnNew(wxCommandEvent& event);
	void OnActivate(wxActivateEvent& event);

	MyCanvas *canvas;

	DECLARE_EVENT_TABLE()
};

// ID for the menu quit command
#define HELLO_QUIT 1
#define HELLO_NEW  2

DECLARE_APP(MyApp)

// notice use of IMPLEMENT_APP_NO_MAIN() instead of the usual IMPLEMENT_APP!
IMPLEMENT_APP_NO_MAIN(MyApp)

bool MyApp::OnInit()
{
	if ( !wxApp::OnInit() )
		return false;

	// as we're not inside wxWidgets main loop, the default logic doesn't work
	// in our case and we need to do this explicitly
	SetExitOnFrameDelete(true);

	(void) CreateFrame();

	return true;
}

void MyApp::ExitMainLoop()
{
	// instead of existing wxWidgets main loop, terminate the MFC one
	::PostQuitMessage(0);
}

wxFrame *MyApp::CreateFrame()
{
	MyChild *subframe = new MyChild(NULL, wxT("Canvas Frame"), wxPoint(10, 10), wxSize(300, 300),
		wxDEFAULT_FRAME_STYLE);

	subframe->SetTitle(wxT("wxWidgets canvas frame"));

	// Give it a status line
	subframe->CreateStatusBar();

	// Make a menubar
	wxMenu *file_menu = new wxMenu;

	file_menu->Append(HELLO_NEW, wxT("&New MFC Window"));
	file_menu->Append(HELLO_QUIT, wxT("&Close"));

	wxMenuBar *menu_bar = new wxMenuBar;

	menu_bar->Append(file_menu, wxT("&File"));

	// Associate the menu bar with the frame
	subframe->SetMenuBar(menu_bar);

	int width, height;
	subframe->GetClientSize(&width, &height);

	MyCanvas *canvas = new MyCanvas(subframe, wxPoint(0, 0), wxSize(width, height));
//	canvas->SetCursor(wxCursor(wxCURSOR_PENCIL));
	subframe->canvas = canvas;
	subframe->Show(true);

	// Return the main frame window
	return subframe;
}

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
EVT_PAINT(MyCanvas::OnPaint)
EVT_MOUSE_EVENTS(MyCanvas::OnMouseEvent)
END_EVENT_TABLE()

// Define a constructor for my canvas
MyCanvas::MyCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size)
: wxScrolledWindow(parent, -1, pos, size)
{
}

// Define the repainting behaviour
void MyCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);

	dc.SetFont(* wxSWISS_FONT);
	dc.SetPen(* wxGREEN_PEN);
	dc.DrawLine(0, 0, 200, 200);
	dc.DrawLine(200, 0, 0, 200);

	dc.SetBrush(* wxCYAN_BRUSH);
	dc.SetPen(* wxRED_PEN);
	dc.DrawRectangle(100, 100, 100, 50);
	dc.DrawRoundedRectangle(150, 150, 100, 50, 20);

	dc.DrawEllipse(250, 250, 100, 50);
	dc.DrawLine(50, 230, 200, 230);
	dc.DrawText(wxT("This is a test string"), 50, 230);
}

// This implements a tiny doodling program! Drag the mouse using
// the left button.
void MyCanvas::OnMouseEvent(wxMouseEvent& event)
{
	static long s_xpos = -1;
	static long s_ypos = -1;

	wxClientDC dc(this);
	dc.SetPen(* wxBLACK_PEN);
	wxPoint pos = event.GetPosition();
	if (s_xpos > -1 && s_ypos > -1 && event.Dragging())
	{
		dc.DrawLine(s_xpos, s_ypos, pos.x, pos.y);
	}

	s_xpos = pos.x;
	s_ypos = pos.y;
}

BEGIN_EVENT_TABLE(MyChild, wxFrame)
EVT_MENU(HELLO_QUIT, MyChild::OnQuit)
EVT_MENU(HELLO_NEW, MyChild::OnNew)
EVT_ACTIVATE(MyChild::OnActivate)
END_EVENT_TABLE()

MyChild::MyChild(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size, const long style)
: wxFrame(frame, -1, title, pos, size, style)
{
	canvas = NULL;
}

MyChild::~MyChild()
{
	if ( IsLastBeforeExit() )
		PostQuitMessage(0);
}

void MyChild::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MyChild::OnNew(wxCommandEvent& WXUNUSED(event))
{
// 	CMainWindow *mainWin = new CMainWindow();
// 	mainWin->ShowWindow( TRUE );
// 	mainWin->UpdateWindow();
}

void MyChild::OnActivate(wxActivateEvent& event)
{
	if (event.GetActive() && canvas)
		canvas->SetFocus();
}

/////////////////////////////////////////////////////////////////////



#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WXAPPONAPI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WXAPPONAPI));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if ( wxTheApp )
		wxTheApp->OnExit();
	wxEntryCleanup();

	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WXAPPONAPI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WXAPPONAPI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   WXDLLIMPEXP_BASE void wxSetInstance(HINSTANCE hInst);
   wxSetInstance(hInstance);
   wxApp::m_nCmdShow = nCmdShow;
   int argc = 0;
   wxChar **argv = NULL;
   wxEntryStart(argc, argv);
   if ( !wxTheApp || !wxTheApp->CallOnInit() )
	   return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd); 

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴��� ���� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
