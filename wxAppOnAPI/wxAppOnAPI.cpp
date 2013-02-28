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

class MyApp: public wxApp
{
public:
	virtual bool OnInit();
	virtual void ExitMainLoop();
	wxFrame *CreateFrame();
};

class MyChild: public wxFrame
{
public:
	MyChild(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size, const long style);
	virtual ~MyChild();
	DECLARE_EVENT_TABLE()
};

DECLARE_APP(MyApp)
IMPLEMENT_APP_NO_MAIN(MyApp)

bool MyApp::OnInit()
{
	if ( !wxApp::OnInit() )
		return false;
	SetExitOnFrameDelete(true);
	CreateFrame();
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
 	subframe->Show(true);
	return subframe;
}

BEGIN_EVENT_TABLE(MyChild, wxFrame)
END_EVENT_TABLE()

MyChild::MyChild(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size, const long style)
: wxFrame(frame, -1, title, pos, size, style)
{
}

MyChild::~MyChild()
{
	if ( IsLastBeforeExit() )
		PostQuitMessage(0);
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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 300, 300, NULL, NULL, hInstance, NULL);
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
