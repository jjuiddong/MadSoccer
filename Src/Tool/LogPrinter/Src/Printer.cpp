
#include "Global.h"
#include "printer.h"
#include <iostream>
#include <fstream>

using namespace std;

BEGIN_EVENT_TABLE( CPrinter, wxListCtrl)
	EVT_CONTEXT_MENU(CPrinter::OnContextMenu)
	EVT_TIMER(ID_REFRESH_TIMER, CPrinter::OnRefreshTimer)
	EVT_MENU(MENU_CLEAR, CPrinter::OnMenuClear)
END_EVENT_TABLE()


CPrinter::CPrinter(wxWindow *parent, const std::string &fileName) : 
	//wxTextCtrl(parent, -1, fileName + "\n", wxDefaultPosition, wxSize(400,150),
		//wxTE_READONLY | wxTE_MULTILINE | wxSUNKEN_BORDER)
wxListCtrl(parent, -1, wxDefaultPosition, wxSize(400,150), 
	wxLC_REPORT)
,	m_fileName(fileName)
{
	InsertColumn(0, "Log Message");
	SetColumnWidth(0, 1000);

	m_Timer.SetOwner(this, ID_REFRESH_TIMER);
	m_Timer.Start( REFRESH_INTERVAL );

	m_InputFile.open( fileName );
	if (m_InputFile.is_open())
	{
		m_InputFile.seekg(0, ios::end);
		m_Pos = m_InputFile.tellg();
	}
	m_InputFile.close();
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
		InsertItem( idx, line );
		EnsureVisible(idx);
		const int logType = ParseLogType( line );
		switch (logType)
		{
		case 0: SetItemBackgroundColour(idx, wxColour(255,100,0)); break;
		case 1: SetItemBackgroundColour(idx, wxColour(255,255,0)); break;
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
	menu.Append(MENU_CLEAR, wxT("&Clear"));
	PopupMenu(&menu, point);
}


/**
 @brief 
 */
void CPrinter::OnMenuClear(wxCommandEvent& event)
{
	DeleteAllItems();
}


/**
@brief   { type number } parse 
*/
int CPrinter::ParseLogType(const string &msg)
{
	const int critical_pos = msg.find( "Critical" );
	if (string::npos != critical_pos)
		return 0;

	const int problem_pos = msg.find( "Problem" );
	if (string::npos != problem_pos)
		return 1;

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
