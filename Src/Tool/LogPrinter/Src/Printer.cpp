
#include "Global.h"
#include "printer.h"
#include <iostream>
#include <fstream>
#include "frame.h"
#include <wx/kbdstate.h>


using namespace std;

BEGIN_EVENT_TABLE( CPrinter, wxListCtrl)
	EVT_CONTEXT_MENU(CPrinter::OnContextMenu)
	EVT_TIMER(ID_REFRESH_TIMER, CPrinter::OnRefreshTimer)
	EVT_MENU(MENU_CLEAR, CPrinter::OnMenuClear)
	EVT_MENU(MENU_TOPMOST, CPrinter::OnMenuTopmost)
	EVT_MENU(MENU_SEARCH, CPrinter::OnMenuSearch)
	EVT_MENU(MENU_SCROLL, CPrinter::OnMenuScroll)
	EVT_MENU(MENU_NOSCROLL, CPrinter::OnMenuNoScroll)
END_EVENT_TABLE()


CPrinter::CPrinter(wxWindow *parent, const std::string &fileName) : 
	wxListCtrl(parent, -1, wxDefaultPosition, wxSize(400,150), 
	wxLC_REPORT)
,	m_fileName(fileName)
,	m_Scroll(true)
{
	InsertColumn(0, "Log Message");
	SetColumnWidth(0, 2000);
	SetBackgroundColour( GetFileName2Color(fileName) );

	m_Timer.SetOwner(this, ID_REFRESH_TIMER);
	m_Timer.Start( REFRESH_INTERVAL );

	m_InputFile.open( fileName );
	if (m_InputFile.is_open())
	{
		m_InputFile.seekg(0, ios::end);
		m_Pos = m_InputFile.tellg();
	}
	m_InputFile.close();

	m_SearchIndice.reserve(128);
	Connect(wxEVT_CHAR_HOOK, wxKeyEventHandler(CPrinter::OnKeyDown));
}

CPrinter::~CPrinter()
{
	m_Timer.Stop();
}


/**
 @brief refresh
 */
void CPrinter::OnRefreshTimer(wxTimerEvent& event)
{
	m_InputFile.open(m_fileName);
	if (!m_InputFile.is_open())
		return;

	m_InputFile.seekg( m_Pos );
	for (std::string line; std::getline(m_InputFile, line); )
	{
		const int idx = GetItemCount();
		const int logType = ParseLogType( line );
		InsertItem( idx, line );
		if (m_Scroll)
			EnsureVisible(idx);
		switch (logType)
		{
		case 0: SetItemBackgroundColour(idx, wxColour(255,100,0)); break;
		case 1: SetItemBackgroundColour(idx, wxColour(255,255,0)); break;
		case 2: SetItemBackgroundColour(idx, wxColour(154,205,50)); break;
		case 3: SetItemBackgroundColour(idx, wxColour(143,188,143)); break;
		}
		
		m_Pos = m_InputFile.tellg();
	}	
	m_InputFile.close();
}


/**
 @brief 
 */
void CPrinter::OnContextMenu(wxContextMenuEvent& event)
{
	wxPoint point = event.GetPosition();
	point = ScreenToClient(point);

	wxMenu menu;
	menu.AppendCheckItem(MENU_TOPMOST, wxT("&TopMost"));
	menu.AppendCheckItem(MENU_SEARCH, wxT("&Search"));
	menu.Append(MENU_CLEAR, wxT("&Clear"));
	menu.AppendSeparator();
	menu.AppendCheckItem(MENU_SCROLL, "&Scroll");
	menu.AppendCheckItem(MENU_NOSCROLL, "&NoScroll");
	menu.Check(MENU_SCROLL, m_Scroll);
	menu.Check(MENU_NOSCROLL, !m_Scroll);

	MyFrame *pFrame = dynamic_cast<MyFrame*>(wxTheApp->GetTopWindow());
	if (pFrame)
		menu.Check(MENU_TOPMOST, pFrame->IsTopMost());
	PopupMenu(&menu, point);
}


/**
 @brief 
 */
void CPrinter::OnMenuClear(wxCommandEvent& event)
{
	DeleteAllItems();
}


void CPrinter::OnMenuNoScroll(wxCommandEvent& event)
{
	m_Scroll = false;
}

void CPrinter::OnMenuScroll(wxCommandEvent& event)
{
	m_Scroll = true;
}

void ReplaceAll (string& strSrc, const string& strFind, const string& strDest)
{
	size_t j;
	while ((j = strSrc.find(strFind)) != string::npos)
		strSrc.replace(j, strFind.length(), strDest);
} 


/**
@brief   { type number } parse 
*/
int CPrinter::ParseLogType(string &msg)
{
	if (string::npos != msg.find( "Critical" ))
		return 0;

	if (string::npos != msg.find( "Problem" ))
		return 1;
	if (string::npos != msg.find( "Error" ))
		return 1;

	if (string::npos != msg.find("Success"))
		return 2;

	if (string::npos != msg.find("#highlight"))
	{
		ReplaceAll(msg, "#highlight", "");
		return 3;
	}

	const int pos1 = msg.find( "{" );
	const int pos2 = msg.find( "}" );
	if ((string::npos == pos1) || (string::npos == pos2))
		return -1;

	string number = msg.substr(pos1+1, pos2-pos1-1);
	if (number.empty())
		return -1;

	if (!isdigit( number[0] ))
		return -1;

	const int type = atoi( number.c_str() );
	return type;
}


/**
 @brief GetFileName2Color
 */
wxColour CPrinter::GetFileName2Color(const std::string &fileName)
{
	if (fileName.empty())
		return wxColour(255,255,255);
	if (std::string::npos != fileName.find("Farm"))
		return wxColour(233,233,200);
	if (std::string::npos != fileName.find("Login"))
		return wxColour(233,200,233);
	if (std::string::npos != fileName.find("Certify"))
		return wxColour(200,233,233);

	int n = 0;
	for (u_int i=0; i < fileName.size(); ++i)
	{
		n += fileName[ i];
	}
	n %= 105;
	int c = n / 20;
	switch (c)
	{
	case 0: return wxColour(255,255,150+n); break;
	case 1: return wxColour(255,150+n,255); break;
	case 2: return wxColour(150+n,255,255); break;
	case 3: return wxColour(200,233,150+n); break;
	case 4: return wxColour(200,255,150+n); break;
	default: return wxColour(255,255,150+n); break;
	}	
}


/**
 @brief OnKeyDown
 */
void CPrinter::OnKeyDown(wxKeyEvent& event)
{
	//event.Skip();
	if (!ToggleWindow(this, event.GetKeyCode()))
	{
		switch(event.GetKeyCode())
		{
		case WXK_F3: NextSearchText(event.ShiftDown()); break;
		}

		event.Skip();
	}
}


/**
 @brief 
 */
void CPrinter::OnMenuTopmost(wxCommandEvent& event)
{
	MyFrame *pFrame = dynamic_cast<MyFrame*>(wxTheApp->GetTopWindow());
	if (pFrame)
		pFrame->ToggleTopMost();	
}


/**
@brief  
*/
void CPrinter::OnMenuSearch(wxCommandEvent& event)
{
	m_SearchIndice.clear();

	string searchText = wxGetTextFromUser("Text", "Search");
	if (searchText.empty())
		return;

	const string srcSearchText = searchText;
	std::transform(searchText.begin(), searchText.end(), searchText.begin(), tolower);

	const int size = GetItemCount();
	for (int i=0; i < size; ++i)
	{
		string text = GetItemText(i);
		std::transform(text.begin(), text.end(), text.begin(), tolower);

		if (text.find(searchText) != string::npos)
			m_SearchIndice.push_back(i);
	}

	if (m_SearchIndice.empty())
	{// Not Found
		wxMessageBox( wxString::Format("Not Found '%s'", srcSearchText.c_str()) );
	}
	else
	{
		// Select 된 모든 라인을 초기화 하고, 첫 번째 검색 라인으로 이동한다.
		SelectAllSearchText();

		const int row = m_SearchIndice[0];
		EnsureVisible(row);
		SetItemState(row, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
	}
}


/**
@brief  
*/
void CPrinter::NextSearchText(const bool IsFindPrev) // IsFindPrev=false
{
	if (m_SearchIndice.empty() || (GetItemCount()==0))
	{
		wxMessageBox( "Not Exist Search Text" );
		return;
	}

	int focusIdx = GetFocusItem();

	const int first = (IsFindPrev)? m_SearchIndice.size()-1 : 0;
	const int last = (IsFindPrev)? -1 : m_SearchIndice.size();
	const int add = (IsFindPrev)? -1 : 1;

	bool IsFindNext = false;
	for (int i=first; i != last; i+=add)
	{
		if ((IsFindPrev && (m_SearchIndice[ i] < focusIdx)) ||
			(!IsFindPrev && (m_SearchIndice[ i] > focusIdx)))
		{
			IsFindNext = true;

			SelectAllSearchText();

			const int row = m_SearchIndice[ i];
			EnsureVisible(row);
			SetItemState(row, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
			break;
		}
	}

	if (!IsFindNext)
	{
		if (focusIdx > 0)
		{
			SetItemState(focusIdx, 0, wxLIST_STATE_SELECTED);
			SetItemState(focusIdx, 0, wxLIST_STATE_FOCUSED);
		}

		if (IsFindPrev)
		{
			wxMessageBox( "Go To Last Line" );
			SetItemState(GetItemCount()-1, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
		}
		else
		{
			wxMessageBox( "Go To First Line" );
			SetItemState(0, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
		}
	}
}


/**
@brief  
*/
int CPrinter::GetFocusItem()
{
	const int size = GetItemCount();
	for (int i=0; i < size; ++i)
	{
		if (wxLIST_STATE_FOCUSED == GetItemState(i, wxLIST_STATE_FOCUSED))
		{
			return i;
		}
	}
	return -1;
}


/**
@brief  선택한 모든 라인을 초기화 하고, 검색 스트링이 포함된 라인을 선택해서 출력한다.
*/
void CPrinter::SelectAllSearchText()
{
	const int size = GetItemCount();
	for (int i=0; i < size; ++i)
	{
		if (wxLIST_STATE_SELECTED == GetItemState(i, wxLIST_STATE_SELECTED))
			SetItemState(i, 0, wxLIST_STATE_SELECTED);
	}

	for (u_int i=0; i < m_SearchIndice.size(); ++i)
		SetItemState(m_SearchIndice[ i], wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}
