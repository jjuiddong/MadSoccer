
#include "Global.h"
#include "Frame.h"
#include "Printer.h"


void ToggleWindow( CPrinter* pActiveWnd, int key )
{
	if (!pActiveWnd)
		return;

	if (key == 344 || key == 345) // f5, f6
	{
		MyFrame *pFrame = dynamic_cast<MyFrame*>(wxTheApp->GetTopWindow());
		if (!pFrame)
			return;

		wxAuiPaneInfo& pane = pFrame->m_mgr.GetPane(pActiveWnd);
		if (pFrame->IsMaximized())
		{
			wxAuiPaneInfoArray &panes = pFrame->m_mgr.GetAllPanes();
			for (u_int i=0; i < panes.size(); ++i)
			{
				panes[ i].Show(true);
			}
			pFrame->Restore();
		}
		else
		{
			if (key == 345)
			{
				wxAuiPaneInfoArray &panes = pFrame->m_mgr.GetAllPanes();
				for (u_int i=0; i < panes.size(); ++i)
				{
					if (panes[ i].window != pActiveWnd)
						panes[ i].Show(false);
				}
			}
			pFrame->Maximize();
		}
		pFrame->m_mgr.Update();
	}

}