
#include "Global.h"
#include "Frame.h"
#include "Printer.h"


void ToggleWindow( CPrinter* pActiveWnd, int key )
{
	if (!pActiveWnd)
		return;

	if (key ==  WXK_F5) // f5, maximize, minimize toggle
	{
		MyFrame *pFrame = dynamic_cast<MyFrame*>(wxTheApp->GetTopWindow());
		if (!pFrame)
			return;

		wxAuiPaneInfo& pane = pFrame->m_mgr.GetPane(pActiveWnd);
		if (pFrame->IsMaximized())
		{
			pFrame->Restore();
		}
		else
		{
			pFrame->Maximize();
		}
	}


	if (key == WXK_F6) // f6, toggle active panel
	{
		static bool isMinimized = true;
		MyFrame *pFrame = dynamic_cast<MyFrame*>(wxTheApp->GetTopWindow());
		if (!pFrame)
			return;

		wxAuiPaneInfo& activePane = pFrame->m_mgr.GetPane(pActiveWnd);
		wxAuiPaneInfoArray &panes = pFrame->m_mgr.GetAllPanes();
		if (isMinimized)
		{
			for (u_int i=0; i < panes.size(); ++i)
			{
				if (panes[ i].window != pActiveWnd)
					panes[ i].Show(false);
			}
		}
		else
		{
			for (u_int i=0; i < panes.size(); ++i)
			{
				panes[ i].Show(true);
			}
		}
		isMinimized = !isMinimized;
		pFrame->m_mgr.Update();
	}

}