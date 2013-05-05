/**
Name:   Frame.h
Author:  jjuiddong
Date:    5/4/2013

*/
#pragma once

#include "Global.h"


class MyFrame : public wxFrame 
{
	enum  {
		ID_REFRESH_TIMER = 100,
		REFRESH_INTERVAL = 1000,
	};
public:
	MyFrame(wxWindow* parent);
	~MyFrame() { m_mgr.UnInit(); }
protected:
	// Event Handler
	DECLARE_EVENT_TABLE()
	void OnRefreshTimer(wxTimerEvent& event);
	void OnDropFiles(wxDropFilesEvent& event);
	void OnKeyDown(wxKeyEvent& event);

public:
	wxAuiManager m_mgr;
	wxTimer	m_Timer;
	std::list<std::string> m_fileList;
	std::list<std::string> m_CmdLine;
};

