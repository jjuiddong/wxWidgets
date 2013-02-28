// wxAppOnAPI.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
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

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WXAPPONAPI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WXAPPONAPI));

	// 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
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
		// 메뉴의 선택 영역을 구문 분석합니다.
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
		// TODO: 여기에 그리기 코드를 추가합니다.
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

// 정보 대화 상자의 메시지 처리기입니다.
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
