// wxAppOnAPI.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

		wxEventLoop * const
			evtLoop = static_cast<wxEventLoop *>(wxEventLoop::GetActive());
		if ( evtLoop && evtLoop->PreProcessMessage(&msg) )
			break;

		if (wxTheApp) 
			wxTheApp->ProcessIdle(); // �� �Լ��� ȣ���ؾ� Docking�� �۵��Ѵ�.
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
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.
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
		// �޴��� ���� ������ ���� �м��մϴ�.
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
