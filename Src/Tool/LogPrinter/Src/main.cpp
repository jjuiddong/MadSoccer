
#include "Global.h"

#include "Printer.h"
#include "Common/Common.h"
#include "Frame.h"

using namespace std;

namespace {
	enum {
		MENU_OPENFILE,
	};
}

BEGIN_EVENT_TABLE( MyFrame, wxFrame )
	EVT_TIMER(ID_REFRESH_TIMER, MyFrame::OnRefreshTimer)
	EVT_CONTEXT_MENU(MyFrame::OnContextMenu)
	EVT_MENU(MENU_OPENFILE, MyFrame::OnMenuOpenFile)
END_EVENT_TABLE()

LONG WINAPI ExceptionFilter(__in PEXCEPTION_POINTERS pExceptionPointer)
{
	exit(0);
	return 0;
}


MyFrame::MyFrame(wxWindow* parent) : wxFrame(parent, -1, _("LogPrinter"),
	wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE)
,	m_IsTopMost(false)
{
	// notify wxAUI which frame to use
	m_mgr.SetManagedWindow(this);

	wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window"),
		wxDefaultPosition, wxSize(200,150),
		wxNO_BORDER | wxTE_MULTILINE);
	
	m_mgr.AddPane(text3, wxCENTER);

	// tell the manager to "commit" all the changes just made
	m_mgr.Update();

	m_Timer.SetOwner(this, ID_REFRESH_TIMER);
	m_Timer.Start( REFRESH_INTERVAL );

	DragAcceptFiles(true);
	Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(MyFrame::OnDropFiles), NULL, this);
	Connect(wxEVT_CHAR_HOOK, wxKeyEventHandler(MyFrame::OnKeyDown));

	SetUnhandledExceptionFilter(ExceptionFilter);	
	
}


/**
 @brief 
 */
void MyFrame::OnRefreshTimer(wxTimerEvent& event)
{
	list<string> fileList;
	if (m_CmdLine.empty())
	{
		//fileList = common::FindFileList( "*.log" );
	}
	else
	{
		auto it = m_CmdLine.begin();
		while (m_CmdLine.end() != it)
		{
			list<string> files = common::FindFileList( *it++ );
			copy(files.begin(), files.end(), back_inserter(fileList));
		}
	}
	fileList.sort();

	set<string> result;
	std::set_difference(fileList.begin(), fileList.end(), m_fileList.begin(), m_fileList.end(),
		insert_iterator<set<string> >(result, result.end()) );

	if (!result.empty())
	{
		std::copy(result.begin(), result.end(), inserter(m_fileList, m_fileList.end()));
		m_fileList.sort();

		auto it = result.begin();
		while (result.end() != it)
		{
			CPrinter *prt = new CPrinter(this, *it);
			m_mgr.AddPane(prt, wxLEFT, *it);
			wxAuiPaneInfo& pane = m_mgr.GetPane(prt);
			pane.MinSize(1000,0);
			it++;
		}
		m_mgr.Update();
	}
}


/**
@brief  
*/
void MyFrame::OnContextMenu(wxContextMenuEvent& event)
{
	wxPoint point = event.GetPosition();
	point = ScreenToClient(point);

	wxMenu menu;
	menu.AppendCheckItem(MENU_OPENFILE, wxT("&OpenFile"));
	PopupMenu(&menu, point);
}


/**
@brief  
*/
void MyFrame::OnMenuOpenFile(wxCommandEvent& event)
{
	const string filename = wxFileSelector("Choose a file to open");
	if (!filename.empty())
	{
		CPrinter *prt = new CPrinter(this, filename);
		m_mgr.AddPane(prt, wxLEFT, filename);
		wxAuiPaneInfo& pane = m_mgr.GetPane(prt);
		pane.MaximizeButton();
		pane.MinSize(2000,0);
		m_mgr.Update();
	}
}


/**
 @brief 
 */
void MyFrame::OnDropFiles(wxDropFilesEvent& event)    
{
	if (event.GetNumberOfFiles() > 0) 
	{
		wxString* dropped = event.GetFiles();
		wxASSERT(dropped);

		wxBusyCursor busyCursor;
		wxWindowDisabler disabler;      
		wxBusyInfo busyInfo(_("Adding files, wait please..."));

		wxString name;
		wxArrayString files;
		for (int i = 0; i < event.GetNumberOfFiles(); i++) {
			name = dropped[i];
			if (wxFileExists(name))
				files.push_back(name);
			else if (wxDirExists(name))
				wxDir::GetAllFiles(name, &files);                                    
		}

		auto it = files.begin();
		while (files.end() != it)
		{
			std::string fileName = *it;
			CPrinter *prt = new CPrinter(this, fileName);
			m_mgr.AddPane(prt, wxLEFT, fileName);
			wxAuiPaneInfo& pane = m_mgr.GetPane(prt);
			pane.MaximizeButton();
			pane.MinSize(2000,0);
			it++;
		}
		m_mgr.Update();
	}
}

/**
@brief  
*/
void MyFrame::OnKeyDown(wxKeyEvent& event)
{
	wxWindow *pWnd = NULL;
	if (WXK_F5 == event.GetKeyCode())
	{
		if (IsMaximized())
			Restore();
		else
			Maximize();
	}
	else
	{
		event.Skip();
	}
}


/**
 @brief 
 */
void MyFrame::ToggleTopMost()
{
	if (!m_IsTopMost)
		SetWindowStyle( wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP );
	else
		SetWindowStyle( wxDEFAULT_FRAME_STYLE );
	m_IsTopMost = !m_IsTopMost;
}



// our normal wxApp-derived class, as usual
class MyApp : public wxApp {
public:
	bool OnInit()
	{
		MyFrame* frame = new MyFrame(NULL);
		
		if (argc >1)
		{
			for (int i=1; i < argc; ++i)
			{
				string str = argv[ i].c_str();
				frame->m_CmdLine.push_back( str );
			}
		}

		SetTopWindow(frame);
		frame->Show();
		return true;
	}
};

DECLARE_APP(MyApp);
IMPLEMENT_APP(MyApp);

