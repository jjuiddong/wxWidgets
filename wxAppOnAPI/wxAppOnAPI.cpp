// wxAppOnAPI.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "wxAppOnAPI.h"

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/evtloop.h"
#include <wx/aui/aui.h>


class MyFrame : public wxFrame 
{
public:
	MyFrame(wxWindow* parent) : wxFrame(parent, -1, _("wxAUI Test"),
		wxDefaultPosition, wxSize(800,600),
		wxDEFAULT_FRAME_STYLE)
	{
		// notify wxAUI which frame to use
		m_mgr.SetManagedWindow(this);

		// create several text controls
		wxTextCtrl* text1 = new wxTextCtrl(this, -1, _("Pane 1 - sample text"),
			wxDefaultPosition, wxSize(200,150),
			wxNO_BORDER | wxTE_MULTILINE);

		wxTextCtrl* text2 = new wxTextCtrl(this, -1, _("Pane 2 - sample text"),
			wxDefaultPosition, wxSize(200,150),
			wxNO_BORDER | wxTE_MULTILINE);

		wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window"),
			wxDefaultPosition, wxSize(200,150),
			wxNO_BORDER | wxTE_MULTILINE);

		// add the panes to the manager
		m_mgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
		m_mgr.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
		m_mgr.AddPane(text3, wxCENTER);

		// tell the manager to "commit" all the changes just made
		m_mgr.Update();
	}

	~MyFrame()
	{
		// deinitialize the frame manager
		m_mgr.UnInit();
	}

private:
	wxAuiManager m_mgr;
};

// our normal wxApp-derived class, as usual
class MyApp : public wxApp {
public:

	bool OnInit()
	{
		wxFrame* frame = new MyFrame(NULL);
		SetTopWindow(frame);
		frame->Show();
		return true;                    
	}
};

DECLARE_APP(MyApp)
IMPLEMENT_APP_NO_MAIN(MyApp)


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

		wxEventLoop * const
			evtLoop = static_cast<wxEventLoop *>(wxEventLoop::GetActive());
		if ( evtLoop && evtLoop->PreProcessMessage(&msg) )
			break;

		if (wxTheApp) 
			wxTheApp->ProcessIdle(); // 이 함수를 호출해야 Docking이 작동한다.
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
	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam)
			DestroyWindow(hWnd);
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
