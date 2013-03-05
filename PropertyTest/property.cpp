// source
// http://wiki.wxwidgets.org/WxAUI
// 

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/evtloop.h"
#include "wx/treectrl.h"
#include <wx/aui/aui.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>

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

		int style = // default style
			wxPG_BOLD_MODIFIED |
			wxPG_SPLITTER_AUTO_CENTER |
			wxPG_AUTO_SORT |
			//wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER |
			//wxPG_TOOLTIPS |
			//wxPG_HIDE_CATEGORIES |
			//wxPG_LIMITED_EDITING |
			wxPG_TOOLBAR |
			wxPG_DESCRIPTION;

		//wxPropertyGrid *prop = new wxPropertyGrid(this, -1, wxDefaultPosition, wxSize(200,150), style);
		wxPropertyGridManager *prop = new wxPropertyGridManager(this, -1, wxDefaultPosition, wxSize(200,150), style);
		prop->Append( new wxPGProperty( _("label2"), _("name1")) );
		prop->Append( new wxPGProperty( _("label1"), _("name3")) );
		prop->Append( new wxPGProperty( _("label3"), _("name4")) );
		//wxPropertyGridPage *prop = new wxPropertyGridPage();
		int extraStyle = wxPG_EX_MODE_BUTTONS |
			wxPG_EX_MULTIPLE_SELECTION;
		prop->SetExtraStyle(extraStyle);

		// add the panes to the manager
		m_mgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
		m_mgr.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
		m_mgr.AddPane(text3, wxCENTER);
		m_mgr.AddPane(prop, wxRIGHT, wxT("property"));

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

DECLARE_APP(MyApp);
IMPLEMENT_APP(MyApp);
