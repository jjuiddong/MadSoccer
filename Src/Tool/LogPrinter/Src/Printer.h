/**
Name:   Printer.h
Author:  jjuiddong
Date:    4/20/2013

	Printer
*/
#pragma once

#include <wx/listctrl.h>

class CPrinter : public wxListCtrl
{
	enum  {
		MENU_CLEAR = 100,
		MENU_TOPMOST,
		MENU_SEARCH,
		MENU_SCROLL,
		MENU_NOSCROLL,
		ID_REFRESH_TIMER = 100,
		REFRESH_INTERVAL = 1000,		
	};


public:
	CPrinter(wxWindow *parent, const std::string &fileName);
	virtual ~CPrinter();


protected:
	int ParseLogType(std::string &msg );	
	wxColour GetFileName2Color(const std::string &fileName);
	void NextSearchText(const bool IsFindPrev=false);
	int GetFocusItem();
	void SelectAllSearchText();

	// Event Handler
	DECLARE_EVENT_TABLE()
	void OnRefreshTimer(wxTimerEvent& event);
	void OnContextMenu(wxContextMenuEvent& event);
	void OnMenuClear(wxCommandEvent& event);
	void OnMenuTopmost(wxCommandEvent& event);
	void OnMenuSearch(wxCommandEvent& event);
	void OnMenuScroll(wxCommandEvent& event);
	void OnMenuNoScroll(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent& event);

private:
	wxTimer	m_Timer;
	std::string m_fileName;
	std::ifstream m_InputFile;
	std::streampos m_Pos;
	bool m_Scroll;

	// search
	vector<int> m_SearchIndice;

};
