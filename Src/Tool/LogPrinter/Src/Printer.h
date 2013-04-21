/**
Name:   Printer.h
Author:  jjuiddong
Date:    4/20/2013

	Printer
*/
#pragma once

class CPrinter : public wxTextCtrl
{
	enum  {
		ID_REFRESH_TIMER = 100,
		REFRESH_INTERVAL = 1000,
	};

public:
	CPrinter(wxWindow *parent, const std::string &fileName);
	virtual ~CPrinter();

protected:
	// Event Handler
	DECLARE_EVENT_TABLE()
	void OnRefreshTimer(wxTimerEvent& event);

private:
	wxTimer	m_Timer;
	std::string m_fileName;
	std::ifstream m_InputFile;
	std::streampos m_Pos;

};
