/**
Name:   Global.h
Author:  jjuiddong
Date:    4/20/2013

Global
*/

#define _AFX_SECURE_NO_WARNINGS 
#define _ATL_SECURE_NO_WARNINGS

#pragma warning (disable: 4996)	// strcpy 경고 제거


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/evtloop.h"
#include <wx/aui/aui.h>
#include <wx/dir.h>
#include <wx/busyinfo.h>

#include <string>
#include <list>
#include <algorithm>
#include <fstream>
#include <set>


class CPrinter;
bool ToggleWindow( CPrinter* pActiveWnd, int key );

