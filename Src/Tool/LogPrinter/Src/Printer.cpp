
#include "Global.h"
#include "printer.h"
#include <iostream>
#include <fstream>

using namespace std;

BEGIN_EVENT_TABLE( CPrinter, wxTextCtrl )
	EVT_TIMER(ID_REFRESH_TIMER, CPrinter::OnRefreshTimer)
END_EVENT_TABLE()


CPrinter::CPrinter(wxWindow *parent, const std::string &fileName) : 
	wxTextCtrl(parent, -1, fileName + "\n", wxDefaultPosition, wxSize(400,150),
		wxTE_READONLY | wxTE_MULTILINE | wxSUNKEN_BORDER)
,	m_fileName(fileName)
{
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
		AppendText( line );
		AppendText( "\n" );
		m_Pos = m_InputFile.tellg();
	}	
	m_InputFile.close();
}
