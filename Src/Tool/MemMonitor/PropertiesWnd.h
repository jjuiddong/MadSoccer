
#pragma once


#include "Lib/DiaWrapper.h"

// class CPropertiesToolBar : public CMFCToolBar
// {
// public:
// 	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
// 	{
// 		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
// 	}
// 
// 	virtual BOOL AllowShowOnList() const { return FALSE; }
// };



class CPropertiesWnd : public CDockablePane
{
// 생성입니다.
public:
	CPropertiesWnd();
	virtual ~CPropertiesWnd();

protected:
	typedef struct _SPropItem
	{
		CMFCPropertyGridProperty *prop;
	} SPropItem;
	typedef std::list<SPropItem*> PropList;
	typedef PropList::iterator PropItor;

	PropList				m_PropList;
	CFont					m_fntPropList;
//	CComboBox m_wndObjectCombo;
//	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl	m_wndPropList;

// 구현입니다.
public:
	void	UpdateProperty(const CString &symbolTypeName);
	void	AdjustLayout();
	void	SetVSDotNetLook(BOOL bSet);

protected:
	void		MakeProperty(CMFCPropertyGridProperty *pParentProp, 
						dia::SSymbolInfo &symbol, 
						const sharedmemory::SMemoryInfo &memInfo);

	void		MakeProperty_UDT(CMFCPropertyGridProperty *pParentProp, 
						dia::SSymbolInfo &symbol, 
						const sharedmemory::SMemoryInfo &memInfo);

	void		MakeProperty_Data(CMFCPropertyGridProperty *pParentProp, 
						dia::SSymbolInfo &symbol, 
						const sharedmemory::SMemoryInfo &memInfo);

	void		MakeProperty_Array(CMFCPropertyGridProperty *pParentProp, 
						dia::SSymbolInfo &symbol, 
						const sharedmemory::SMemoryInfo &memInfo);

	void		MakeProperty_Final(CMFCPropertyGridProperty *pParentProp, 
						const std::string valueName, _variant_t value );

	void		AddProperty(CMFCPropertyGridProperty *prop);

	_variant_t	GetValue( dia::SSymbolInfo &symbol, 
						const sharedmemory::SMemoryInfo &memInfo);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

