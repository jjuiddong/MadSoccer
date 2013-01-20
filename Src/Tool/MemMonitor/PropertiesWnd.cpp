
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "MemMonitor.h"
#include "Lib/DiaWrapper.h"
#include "dia2.h"
#include <boost/interprocess/streams/bufferstream.hpp>

using namespace dia;
using namespace sharedmemory;
using namespace boost::interprocess;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd() :
	m_CurrentSymbolName(L"None")
{
}

CPropertiesWnd::~CPropertiesWnd()
{
	KillTimer(ID_TIMER);
	BOOST_FOREACH(SPropItem *item, m_PropList)
	{
		SAFE_DELETE(item);
	}
	m_PropList.clear();
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_CONTROL_RANGE(CBN_SELCHANGE, ID_COMBOBOX, ID_COMBOBOX, OnComboBoxSelChange)
	ON_WM_TIMER()

	//	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	//	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	// 	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	// 	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	// 	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	// 	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	// 	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar �޽��� ó����

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, 
		SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, 
		SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), 
		rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER 
													 | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_COMBOBOX))
	{
		TRACE0("�Ӽ� �޺� ���ڸ� ������ ���߽��ϴ�. \n");
		return -1;
	}
	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
	InitComboBox();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("�Ӽ� ǥ�� ������ ���߽��ϴ�. \n");
		return -1;      // ������ ���߽��ϴ�.
	}
	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ��� */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* ��� */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// ��� ����� �θ� �������� �ƴ� �� ��Ʈ���� ���� ����õ˴ϴ�.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Ÿ�̸� ����
	SetTimer(ID_TIMER, REFRESH_TIME, NULL);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

// void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
// {
// }

// void CPropertiesWnd::OnSortProperties()
// {
// 	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
// }
// 
// void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
// {
// 	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
// }
//
// void CPropertiesWnd::OnProperties1()
// {
// 	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
// }
// 
// void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
// {
// 	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
// }
// 
// void CPropertiesWnd::OnProperties2()
// {
// 	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
// }
// 
// void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
// {
// 	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
// }

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

/*
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("���"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D ���"), (_variant_t) false, _T("â�� ���� �۲��� �ƴ� �۲��� �����ϰ�, ��Ʈ�ѿ� 3D �׵θ��� �����մϴ�.")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("�׵θ�"), _T("Dialog Frame"), _T("None, Thin, Resizable �Ǵ� Dialog Frame �� �ϳ��� �����մϴ�."));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("ĸ��"), (_variant_t) _T("����"), _T("â�� ���� ǥ���ٿ� ǥ�õǴ� �ؽ�Ʈ�� �����մϴ�.")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("â ũ��"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("����"), (_variant_t) 250l, _T("â�� ���̸� �����մϴ�."));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("�ʺ�"), (_variant_t) 150l, _T("â�� �ʺ� �����մϴ�."));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("�۲�"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("���� ���"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("�۲�"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("â�� �⺻ �۲��� �����մϴ�.")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("�ý��� �۲��� ����մϴ�."), (_variant_t) true, _T("â���� MS Shell Dlg �۲��� ����ϵ��� �����մϴ�.")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("��Ÿ"));
	pProp = new CMFCPropertyGridProperty(_T("(�̸�)"), _T("���� ���α׷�"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("â ����"), RGB(210, 192, 254), NULL, _T("â�� �⺻ ������ �����մϴ�."));
	pColorProp->EnableOtherButton(_T("��Ÿ..."));
	pColorProp->EnableAutomaticButton(_T("�⺻��"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static TCHAR BASED_CODE szFilter[] = _T("������ ����(*.ico)|*.ico|��� ����(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("������"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("â �������� �����մϴ�.")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("����"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("����"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("ù��° ���� ����"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("�� ��° ���� ����"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�׸� 1"), (_variant_t) _T("�� 1"), _T("�����Դϴ�.")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�׸� 2"), (_variant_t) _T("�� 2"), _T("�����Դϴ�.")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�׸� 3"), (_variant_t) _T("�� 3"), _T("�����Դϴ�.")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
/**/
}


//------------------------------------------------------------------------
// �޺��ڽ� �ʱ�ȭ
// �����޸𸮿� ��ϵ� �����͸� �޺��ڽ��� �߰��Ѵ�.
//------------------------------------------------------------------------
void	CPropertiesWnd::InitComboBox()
{
	sharedmemory::MemoryList memList;
	sharedmemory::EnumerateMemoryInfo(memList);

	m_wndObjectCombo.AddString( L"None" ); // default
	BOOST_FOREACH(sharedmemory::SMemoryInfo &info, memList)
	{
		std::wstring wstr = common::string2wstring( info.name );
		m_wndObjectCombo.AddString( wstr.c_str() );
	}	

	m_wndObjectCombo.SetCurSel(0);
}


//------------------------------------------------------------------------
// symbolName �� �̸��� ������ �޺��ڽ� ��Ʈ���� 
// ȭ�鿡 ���̰� �Ѵ�.
//------------------------------------------------------------------------
void	CPropertiesWnd::ChangeComboBoxFocus(const CString &symbolName)
{
	const int nItem = m_wndObjectCombo.FindString(0, symbolName);
	if (nItem == CB_ERR)
	{
		m_wndObjectCombo.SetCurSel(0);
		AfxMessageBox( common::formatw("\"%s\" Item�� ã�� ���߽��ϴ�.",
			(LPCWSTR)symbolName).c_str() );
		return;
	}
	m_wndObjectCombo.SetCurSel( nItem );
}


void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

void CPropertiesWnd::SetVSDotNetLook(BOOL bSet)
{
	m_wndPropList.SetVSDotNetLook(bSet);
	m_wndPropList.SetGroupNameFullWidth(bSet);
}


//------------------------------------------------------------------------
// ���ڷ� �Ѿ�� symbolType��� Property�� �����Ѵ�.
//------------------------------------------------------------------------
void CPropertiesWnd::UpdateProperty(const CString &symbolName)
{
	if (m_CurrentSymbolName == symbolName)
		return;

	m_wndPropList.RemoveAll();

	std::string tmpStr = common::wstring2string((LPCWSTR)symbolName);
	std::string str = sharedmemory::ParseObjectName(tmpStr);
	IDiaSymbol *pSymbol = CDiaWrapper::Get()->FindType(str);
	if (!pSymbol)
	{
		::AfxMessageBox(
			common::formatw("\"%s\" �ش��ϴ� �ɺ��� Pdb���Ͽ� �����ϴ�.", 
				tmpStr.c_str()).c_str() );
		SetWindowTextW(L"Not Found");
		
		return;
	}

	sharedmemory::SMemoryInfo memInfo;
	if (!sharedmemory::FindMemoryInfo(tmpStr, memInfo))
	{
		::AfxMessageBox( 
				common::formatw("�����޸𸮿� %s Ÿ���� ������ �����ϴ�.",
					tmpStr.c_str()).c_str() );
		SetWindowTextW(L"Not Found");
		return;
	}

	// Outputâ�� ���
	CMainFrame *pFrm = (CMainFrame*)::AfxGetMainWnd();
	CString output = L"[ " + symbolName;
	output += L" ] �ɺ� ����";
	pFrm->GetOutputWnd().AddString( output );
	dbg::Print(  "%s �ɺ� ����", tmpStr.c_str() );
	//

	SetWindowTextW(symbolName);
	m_CurrentSymbolName = symbolName;
	ChangeComboBoxFocus(symbolName);

	MakeProperty(NULL, pSymbol, memInfo);
}


//------------------------------------------------------------------------
// Property ����
//------------------------------------------------------------------------
void CPropertiesWnd::MakeProperty(CMFCPropertyGridProperty *pParentProp, IDiaSymbol *pSymbol,
								  const sharedmemory::SMemoryInfo &memInfo)
{
	enum SymTagEnum symtag;
	if (S_OK != pSymbol->get_symTag((DWORD*)&symtag))
		return;

	switch (symtag)
	{
	case SymTagData: 
		MakeProperty_Data(pParentProp, pSymbol, memInfo);
		break;

	case SymTagArrayType:
		MakeProperty_Array(pParentProp, pSymbol, memInfo);
		break;

	case SymTagPointerType:
		MakeProperty_Pointer(pParentProp, pSymbol, memInfo);
		break;

	case SymTagTypedef:
	case SymTagBaseClass:
	case SymTagVTable:
	case SymTagFunction:
		break;

	case SymTagUDT:
		MakeProperty_UDT(pParentProp, pSymbol, memInfo);
		break;

	default:
		{
			std::string name = dia::GetSymbolName(pSymbol);
			CMainFrame *pFrm = (CMainFrame*)::AfxGetMainWnd();
			std::stringstream ss;
			ss << name << "���� ����Ÿ�� Property�� �߰����� ���߽��ϴ�.";
			pFrm->GetOutputWnd().AddString( common::string2wstring(ss.str()).c_str() );
		}
		break;
	}
}


//------------------------------------------------------------------------
// User Define Type 
//------------------------------------------------------------------------
void CPropertiesWnd::MakeProperty_UDT(CMFCPropertyGridProperty *pParentProp, 
									  IDiaSymbol *pSymbol, const SMemoryInfo &memInfo)
{
	const std::string name = dia::GetSymbolName(pSymbol);

 	CMFCPropertyGridProperty *pProp = 
 		new CMFCPropertyGridProperty( common::string2wstring(name).c_str() ); 

	ULONGLONG offset = dia::GetSymbolLocation(pSymbol);
	ULONGLONG length = 0;
	HRESULT hr = pSymbol->get_length(&length);
	assert(S_OK == hr);

	CComPtr<IDiaEnumSymbols> pEnumChildren;
	IDiaSymbol *pChild;
	ULONG celt = 0;
	if (SUCCEEDED(pSymbol->findChildren(SymTagNull, NULL, nsNone, &pEnumChildren))) 
	{
		while (SUCCEEDED(pEnumChildren->Next(1, &pChild, &celt)) && (celt == 1)) 
		{
			void *ptr= (BYTE*)memInfo.ptr + offset;
			SMemoryInfo udtMemInfo = SMemoryInfo(name.c_str(), ptr, (size_t)length);

			MakeProperty(pProp, pChild, udtMemInfo);
			pChild->Release();
		}
	}

//	pProp->Expand(FALSE); // �ϴ� ������´�.
	AddProperty(pParentProp, pProp, STypeData(SymTagUDT, VT_EMPTY, memInfo.ptr));
}


//------------------------------------------------------------------------
// Pointer Ÿ�� ��� 
//------------------------------------------------------------------------
void	CPropertiesWnd::MakeProperty_Pointer(CMFCPropertyGridProperty *pParentProp, 
								 IDiaSymbol *pSymbol, const SMemoryInfo &memInfo)
{
	CComPtr<IDiaSymbol> pBaseType;
	HRESULT hr = pSymbol->get_type(&pBaseType);
	RETA(hr == S_OK);

	ULONGLONG length = 0;
	hr = pBaseType->get_length(&length);
	RETA(hr == S_OK);

	void *srcPtr = (void*)*(DWORD*)memInfo.ptr;
	void *newPtr = sharedmemory::MemoryMapping(srcPtr);
	if (newPtr)
	{
		SMemoryInfo ptrMemInfo(memInfo.name.c_str(), newPtr, (size_t)length);
//		MakeProperty(pParentProp, pBaseType, memInfo);
	}

// 	const std::string name = dia::GetSymbolName(pSymbol);
// 	CMFCPropertyGridProperty *pProp = 
// 		new CMFCPropertyGridProperty( common::string2wstring(name).c_str() ); 

//	AddProperty(pParentProp, pProp, STypeData(SymTagPointerType, VT_EMPTY, NULL));
}


//------------------------------------------------------------------------
// Propertyâ�� Control�� �߰��Ѵ�.
// ���� �̸��� Ÿ��, ���� �����Ѵ�.
//------------------------------------------------------------------------
void CPropertiesWnd::MakeProperty_Data(CMFCPropertyGridProperty *pParentProp, 
									   IDiaSymbol *pSymbol,
									   const SMemoryInfo &memInfo)
{
	CComPtr<IDiaSymbol> pBaseType;
	HRESULT hr = pSymbol->get_type(&pBaseType);
	RETA(hr == S_OK);

	enum SymTagEnum baseSymTag;
	hr = pBaseType->get_symTag((DWORD*)&baseSymTag);
	RETA(hr == S_OK);

	switch (baseSymTag)
	{
	case SymTagBaseType:
		{
			std::string valueTypeName = dia::GetSymbolName(pSymbol) + " (" + dia::GetSymbolTypeName(pSymbol) + ")";
			MakeProperty_Final( pParentProp, valueTypeName, pSymbol, memInfo );
		}
		break;

	case SymTagPointerType:
		{
			const std::string name = dia::GetSymbolName(pSymbol) ;
			const std::string typeName = dia::GetSymbolTypeName(pSymbol);
			std::string valueTypeName = name + " (" + typeName + ")";
			CMFCPropertyGridProperty *pProp = MakeProperty_Final( pParentProp, valueTypeName, pSymbol, memInfo );

			ULONGLONG offset = dia::GetSymbolLocation(pSymbol);
			ULONGLONG length = 0;
			HRESULT hr = pBaseType->get_length(&length);
			assert(S_OK == hr);	
			void *newPtr =  (BYTE*)memInfo.ptr + offset;
			SMemoryInfo ptrMemInfo(typeName.c_str(), newPtr, (size_t)length);
			MakeProperty(pProp, pBaseType, ptrMemInfo); // pBaseType�� ���ڷ� �Ѵ�.
		}
		break;

	case SymTagUDT:
	case SymTagArrayType:
		{
			std::string name = dia::GetSymbolName(pSymbol);
			std::string typeName = dia::GetSymbolTypeName(pBaseType);
			std::string valueTypeName =  name + " (" +  typeName + ")";
			CMFCPropertyGridProperty *pProp = 
				new CMFCPropertyGridProperty( common::string2wstring(valueTypeName).c_str() ); 
			AddProperty(pParentProp, pProp, STypeData(SymTagUDT, VT_EMPTY, NULL));

			ULONGLONG offset = dia::GetSymbolLocation(pSymbol);
			ULONGLONG length = 0;
			HRESULT hr = pBaseType->get_length(&length);
			assert(S_OK == hr);	

			void *ptr = (BYTE*)memInfo.ptr + offset;
			SMemoryInfo udtMemInfo = SMemoryInfo(typeName.c_str(), ptr, (size_t)length);
			MakeProperty(pProp, pBaseType, udtMemInfo); // pBaseType�� ���ڷ� �Ѵ�.
		}
		break;
	}
}


//------------------------------------------------------------------------
// pSymbol : Array Type�� ����Ų��. 
//------------------------------------------------------------------------
void CPropertiesWnd::MakeProperty_Array(CMFCPropertyGridProperty *pParentProp, 
										IDiaSymbol *pSymbol,
										const sharedmemory::SMemoryInfo &memInfo)
{
	ULONGLONG length=0;
	ULONGLONG element_length=0;
	pSymbol->get_length(&length);

	CComPtr<IDiaSymbol> pElementType;	// �迭 ������ Ÿ��
	if (S_FALSE == pSymbol->get_type(&pElementType))
	{
		// pSymbol�� arrayŸ���̱� ������ basetype�� ������ �Ѵ�.
		// �׷��� �ʴٸ� pdb������ �����ų�, �ý��ۻ� �����̴�.
		assert(0);
		return;
	}

	HRESULT result = pElementType->get_length(&element_length);
	assert(S_OK == result);

	enum SymTagEnum elemSymTag;
	result = pElementType->get_symTag((DWORD*)&elemSymTag);
	assert(S_OK == result);

	char valueName[ 128];
	basic_bufferstream<char> formatter(valueName, sizeof(valueName));

	if (SymTagData == elemSymTag || SymTagBaseType == elemSymTag)
	{
		const std::string typeName = dia::GetSymbolTypeName(pElementType);

		for (ULONGLONG i=0; i < length; i += element_length)
		{
			void *ptr = (BYTE*)memInfo.ptr + i;
			SMemoryInfo arrayElem(typeName.c_str(), ptr, (size_t)element_length);

			formatter.seekp(0);
			formatter << "[" << i / element_length << "]" << std::ends;
			MakeProperty_Final(pParentProp, valueName, pSymbol, arrayElem);
		}

		pParentProp->Expand(FALSE); // �ϴ� ������´�.
	}
	else // UDT, Array
	{
// 			const std::string name = dia::GetSymbolName(pSymbol);
 			const std::string typeName = dia::GetSymbolTypeName(pElementType);
// 			std::string valueTypeName = name + " (" + typeName + ")";
			for (ULONGLONG i=0; i < length; i += element_length)
			{
				formatter.seekp(0);
				formatter << "[" << i / element_length << "]" << std::ends;
 				CMFCPropertyGridProperty *pProp =
					new CMFCPropertyGridProperty( common::string2wstring(valueName).c_str() );
				AddProperty( pParentProp, pProp, STypeData(SymTagUDT,VT_EMPTY,NULL));

				void *ptr = (BYTE*)memInfo.ptr + i;
				SMemoryInfo arrayElem(typeName.c_str(), ptr, (size_t)element_length);
				MakeProperty(pProp, pElementType, arrayElem);
			}

			pParentProp->Expand(FALSE); // �ϴ� ������´�.
	}
}


//------------------------------------------------------------------------
// m_wndPropList �� Row �� �߰��Ѵ�.
// pSymbol �� ����Ÿ�� ����Ű�� �ɺ��̾�� �Ѵ�.
//------------------------------------------------------------------------
CMFCPropertyGridProperty* CPropertiesWnd::MakeProperty_Final(
							CMFCPropertyGridProperty *pParentProp, 
							   const std::string valueName, IDiaSymbol *pSymbol, 
							   const SMemoryInfo &memInfo )
{
	_variant_t value = GetValue(pSymbol, memInfo);

	CMFCPropertyGridProperty *pProp = NULL;
	switch (value.vt)
	{
	case VT_I2:
	case VT_I4:
	case VT_R4:
	case VT_R8:
	case VT_BOOL:
	case VT_DECIMAL:
	case VT_UI2:
	case VT_UI4:
	case VT_INT:
	case VT_UINT:
		{
			pProp = new CMFCPropertyGridProperty( common::string2wstring(valueName).c_str(), value, _T("") );

			_variant_t v1 = pProp->GetValue();
			assert(v1.vt == value.vt);
		}
		break;

	case VT_I1:
	case VT_UI1:
			pProp = new CMFCPropertyGridProperty( common::string2wstring(valueName).c_str(), 
				(_variant_t)(int)0, _T("") );
		break;

	default:
		{
			CMainFrame *pFrm = (CMainFrame*)::AfxGetMainWnd();
			std::stringstream ss;
			ss << valueName << "���� ����Ÿ�� Property�� �߰����� ���߽��ϴ�.";
			pFrm->GetOutputWnd().AddString( common::string2wstring(ss.str()).c_str() );
		}
		break;
	}
	RETV(!pProp, NULL);

	if (!SetPropertyValue(pProp, value ))
	{
		delete pProp;
		return NULL;
	}

	const LONG offset = dia::GetSymbolLocation(pSymbol);
	void *ptr = (BYTE*)memInfo.ptr + offset;
	AddProperty(pParentProp, pProp, STypeData(SymTagData, value.vt, ptr));
	return pProp;
}


//------------------------------------------------------------------------
// Property ����Ÿ�� value ���� �����Ѵ�. 
//------------------------------------------------------------------------
bool CPropertiesWnd::SetPropertyValue(CMFCPropertyGridProperty *pProp, _variant_t value)
{
	switch (value.vt)
	{
	case VT_I2:
	case VT_I4:
	case VT_R4:
	case VT_R8:
	case VT_BOOL:
	case VT_DECIMAL:
	case VT_UI2:
	case VT_UI4:
	case VT_INT:
	case VT_UINT:
	case VT_I1:
	case VT_UI1:
		{
 			_variant_t var;
			var.ChangeType(pProp->GetValue().vt, &value);
			pProp->SetValue(var);
		}
		break;

//		{
//			_variant_t tmp  = (VT_I1 == value.vt)? (int)value.cVal : (int)value.bVal;
// 			_variant_t var;
// 			var.ChangeType(pProp->GetValue().vt, &value);
// 			pProp->SetValue(tmp);
//  		}
//  		break;

		// 	case VT_BSTR:
		// 		{
		// 			std::string str;
		// 			operator>>(str);
		// #ifdef _UNICODE
		// 			var.bstrVal = (_bstr_t)common::string2wstring(str).c_str();
		// #else
		// 			var.bstrVal = (_bstr_t)str.c_str();
		// #endif
		// 		}
		// 		break;

	default:
		{
			// Err!!, Property�� value Ÿ���� ���� ���� �� �����ϴ�. 
			assert(0);
			return false;
		}
		break;
	}
	return true;
}


//------------------------------------------------------------------------
// pSymbol Ÿ���� ������ memInfo �ּҿ��� �����´�.
// pSymbol �� SymTagData Ÿ���̾�� �Ѵ�.
//------------------------------------------------------------------------
_variant_t CPropertiesWnd::GetValue(IDiaSymbol *pSymbol, const SMemoryInfo &memInfo)
{
	_variant_t value;

	CMFCPropertyGridProperty *pProp = NULL;
	LONG offset = dia::GetSymbolLocation(pSymbol);
	void *ptr = (BYTE*)memInfo.ptr + offset;

	CComPtr<IDiaSymbol> pBaseType;
	HRESULT hr = pSymbol->get_type(&pBaseType);
	RETAV((S_OK == hr), value);
//	assert( S_OK == hr );
//	if (hr == S_FALSE) return value;

	enum SymTagEnum baseSymTag;
	hr = pBaseType->get_symTag((DWORD*)&baseSymTag);
	RETAV(S_OK==hr, value);

	BasicType btype;
	switch (baseSymTag)
	{
	case SymTagBaseType:
		hr = pBaseType->get_baseType((DWORD*)&btype);
		RETAV((S_OK == hr), value );
		break;

	case SymTagPointerType:
		btype = btULong;
		break;

	default:
		return value;
	}

	ULONGLONG length = 0;
	hr = pBaseType->get_length(&length);
	RETAV((S_OK == hr), value );

	 value = dia::GetValueFromAddress(ptr, btype, length);
	 return value;
}


//------------------------------------------------------------------------
// Property�߰�
//------------------------------------------------------------------------
void CPropertiesWnd::AddProperty(CMFCPropertyGridProperty *pParentProp, CMFCPropertyGridProperty *prop, 
								 const STypeData &typeData)
{
	RET(!prop);

	SPropItem *p = new SPropItem;
	p->prop = prop;
	p->typeData = typeData;
	m_PropList.push_back(p);

	prop->SetData((DWORD_PTR)p);

	if (pParentProp)
	{
		pParentProp->AddSubItem(prop);
	}
	else
	{
		m_wndPropList.AddProperty(prop);
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
LRESULT CPropertiesWnd::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;
	SPropItem *pItem = (SPropItem*)pProp->GetData();
	if (pItem && pItem->typeData.ptr)
	{
		_variant_t curVar = pProp->GetValue();
 		_variant_t var;
 		var.ChangeType(pItem->typeData.vt, &curVar);
		dia::SetValue( pItem->typeData.ptr, var);
	}
	return 0;
}


//------------------------------------------------------------------------
// ComboBox ������ ��ġ�� �ٲ�� ȣ��ȴ�.
//------------------------------------------------------------------------
void CPropertiesWnd::OnComboBoxSelChange(UINT nID)
{
	const int nItem = m_wndObjectCombo.GetCurSel();
	CString curText;
	m_wndObjectCombo.GetLBText(nItem , curText);
	UpdateProperty( curText );
}


//------------------------------------------------------------------------
// Clicked Refresh Button
//------------------------------------------------------------------------
void CPropertiesWnd::OnRefresh()
{
	Refresh();

	// Outputâ�� ���
	CMainFrame *pFrm = (CMainFrame*)::AfxGetMainWnd();
	pFrm->GetOutputWnd().AddString( L"Refresh" );
	//	
}


//------------------------------------------------------------------------
// �����޸� ������ �����Ѵ�.
//------------------------------------------------------------------------
void CPropertiesWnd::Refresh()
{
	const int cnt = m_wndPropList.GetPropertyCount();
	for (int i=0; i < cnt; ++i)
	{
		CMFCPropertyGridProperty *pProp = m_wndPropList.GetProperty(i);
		Refresh_Property(pProp);
	}
}


//------------------------------------------------------------------------
// �����޸� ������ �����Ѵ�.
//------------------------------------------------------------------------
void	CPropertiesWnd::Refresh_Property(CMFCPropertyGridProperty *pProp )
{
	RET(!pProp);

	if (pProp->IsInPlaceEditing()) // Edit���� ���� �������� �ʴ´�.
		return;

	SPropItem *pItem = (SPropItem*)pProp->GetData();
	if (pItem && SymTagData == pItem->typeData.symtag 
		&& pItem->typeData.ptr)
	{
		_variant_t value = dia::GetValue(pItem->typeData.ptr, pItem->typeData.vt);
		SetPropertyValue(pProp, value);
	}

	const int subCnt = pProp->GetSubItemsCount();
	for (int k=0; k < subCnt; ++k)
	{
		CMFCPropertyGridProperty *p = pProp->GetSubItem(k);
		Refresh_Property(p);
	}
}


//------------------------------------------------------------------------
//  ���� �ð����� ������ �����Ѵ�.
//------------------------------------------------------------------------
void CPropertiesWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (ID_TIMER == nIDEvent)
	{
		// ȭ�鿡 ������ �ʰų�, �巡�� ���϶��� ������������ �ʴ´�.
		if (IsDragMode() || !CWnd::IsWindowVisible())
			return;
		Refresh();
	}
	CDockablePane::OnTimer(nIDEvent);
}
