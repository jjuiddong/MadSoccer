
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
// CResourceViewBar 메시지 처리기

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
		TRACE0("속성 콤보 상자를 만들지 못했습니다. \n");
		return -1;
	}
	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
	InitComboBox();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("속성 표를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}
	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 타이머 설정
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
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// }
// 
// void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
// {
// 	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
// }
// 
// void CPropertiesWnd::OnProperties2()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// }
// 
// void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
// {
// 	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
// }

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

/*
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("모양"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D 모양"), (_variant_t) false, _T("창에 굵은 글꼴이 아닌 글꼴을 지정하고, 컨트롤에 3D 테두리를 지정합니다.")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("테두리"), _T("Dialog Frame"), _T("None, Thin, Resizable 또는 Dialog Frame 중 하나를 지정합니다."));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("캡션"), (_variant_t) _T("정보"), _T("창의 제목 표시줄에 표시되는 텍스트를 지정합니다.")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("창 크기"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("높이"), (_variant_t) 250l, _T("창의 높이를 지정합니다."));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("너비"), (_variant_t) 150l, _T("창의 너비를 지정합니다."));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("글꼴"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("맑은 고딕"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("글꼴"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("창의 기본 글꼴을 지정합니다.")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("시스템 글꼴을 사용합니다."), (_variant_t) true, _T("창에서 MS Shell Dlg 글꼴을 사용하도록 지정합니다.")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("기타"));
	pProp = new CMFCPropertyGridProperty(_T("(이름)"), _T("응용 프로그램"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("창 색상"), RGB(210, 192, 254), NULL, _T("창의 기본 색상을 지정합니다."));
	pColorProp->EnableOtherButton(_T("기타..."));
	pColorProp->EnableAutomaticButton(_T("기본값"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static TCHAR BASED_CODE szFilter[] = _T("아이콘 파일(*.ico)|*.ico|모든 파일(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("아이콘"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("창 아이콘을 지정합니다.")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("폴더"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("계층"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("첫번째 하위 수준"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("두 번째 하위 수준"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("항목 1"), (_variant_t) _T("값 1"), _T("설명입니다.")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("항목 2"), (_variant_t) _T("값 2"), _T("설명입니다.")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("항목 3"), (_variant_t) _T("값 3"), _T("설명입니다.")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
/**/
}


//------------------------------------------------------------------------
// 콤보박스 초기화
// 공유메모리에 등록된 포인터를 콤보박스에 추가한다.
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
// symbolName 의 이름과 동일한 콤보박스 스트링을 
// 화면에 보이게 한다.
//------------------------------------------------------------------------
void	CPropertiesWnd::ChangeComboBoxFocus(const CString &symbolName)
{
	const int nItem = m_wndObjectCombo.FindString(0, symbolName);
	if (nItem == CB_ERR)
	{
		m_wndObjectCombo.SetCurSel(0);
		AfxMessageBox( common::formatw("\"%s\" Item을 찾지 못했습니다.",
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
// 인자로 넘어온 symbolType대로 Property를 생성한다.
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
			common::formatw("\"%s\" 해당하는 심볼은 Pdb파일에 없습니다.", 
				tmpStr.c_str()).c_str() );
		SetWindowTextW(L"Not Found");
		
		return;
	}

	sharedmemory::SMemoryInfo memInfo;
	if (!sharedmemory::FindMemoryInfo(tmpStr, memInfo))
	{
		::AfxMessageBox( 
				common::formatw("공유메모리에 %s 타입의 정보가 없습니다.",
					tmpStr.c_str()).c_str() );
		SetWindowTextW(L"Not Found");
		return;
	}

	// Output창에 출력
	CMainFrame *pFrm = (CMainFrame*)::AfxGetMainWnd();
	CString output = L"[ " + symbolName;
	output += L" ] 심볼 변경";
	pFrm->GetOutputWnd().AddString( output );
	dbg::Print(  "%s 심볼 변경", tmpStr.c_str() );
	//

	SetWindowTextW(symbolName);
	m_CurrentSymbolName = symbolName;
	ChangeComboBoxFocus(symbolName);

	MakeProperty(NULL, pSymbol, memInfo);
}


//------------------------------------------------------------------------
// Property 생성
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
			ss << name << "값의 데이타를 Property애 추가하지 못했습니다.";
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

//	pProp->Expand(FALSE); // 일단 접어놓는다.
	AddProperty(pParentProp, pProp, STypeData(SymTagUDT, VT_EMPTY, memInfo.ptr));
}


//------------------------------------------------------------------------
// Pointer 타입 출력 
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
// Property창에 Control을 추가한다.
// 변수 이름과 타입, 값을 설정한다.
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
			MakeProperty(pProp, pBaseType, ptrMemInfo); // pBaseType을 인자로 한다.
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
			MakeProperty(pProp, pBaseType, udtMemInfo); // pBaseType을 인자로 한다.
		}
		break;
	}
}


//------------------------------------------------------------------------
// pSymbol : Array Type을 가리킨다. 
//------------------------------------------------------------------------
void CPropertiesWnd::MakeProperty_Array(CMFCPropertyGridProperty *pParentProp, 
										IDiaSymbol *pSymbol,
										const sharedmemory::SMemoryInfo &memInfo)
{
	ULONGLONG length=0;
	ULONGLONG element_length=0;
	pSymbol->get_length(&length);

	CComPtr<IDiaSymbol> pElementType;	// 배열 개개의 타입
	if (S_FALSE == pSymbol->get_type(&pElementType))
	{
		// pSymbol은 array타입이기 때문에 basetype을 가져야 한다.
		// 그렇지 않다면 pdb파일이 깨졌거나, 시스템상 오류이다.
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

		pParentProp->Expand(FALSE); // 일단 접어놓는다.
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

			pParentProp->Expand(FALSE); // 일단 접어놓는다.
	}
}


//------------------------------------------------------------------------
// m_wndPropList 에 Row 를 추가한다.
// pSymbol 은 데이타를 가르키는 심볼이어야 한다.
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
			ss << valueName << "값의 데이타를 Property애 추가하지 못했습니다.";
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
// Property 데이타에 value 값을 설정한다. 
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
			// Err!!, Property에 value 타입의 값을 넣을 수 없습니다. 
			assert(0);
			return false;
		}
		break;
	}
	return true;
}


//------------------------------------------------------------------------
// pSymbol 타입의 정보를 memInfo 주소에서 가져온다.
// pSymbol 은 SymTagData 타입이어야 한다.
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
// Property추가
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
// ComboBox 아이템 위치가 바뀌면 호출된다.
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

	// Output창에 출력
	CMainFrame *pFrm = (CMainFrame*)::AfxGetMainWnd();
	pFrm->GetOutputWnd().AddString( L"Refresh" );
	//	
}


//------------------------------------------------------------------------
// 공유메모리 정보를 갱신한다.
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
// 공유메모리 정보를 갱신한다.
//------------------------------------------------------------------------
void	CPropertiesWnd::Refresh_Property(CMFCPropertyGridProperty *pProp )
{
	RET(!pProp);

	if (pProp->IsInPlaceEditing()) // Edit중인 값은 갱신하지 않는다.
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
//  일정 시간마다 정보를 갱신한다.
//------------------------------------------------------------------------
void CPropertiesWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (ID_TIMER == nIDEvent)
	{
		// 화면에 보이지 않거나, 드래그 중일때는 리프레쉬하지 않는다.
		if (IsDragMode() || !CWnd::IsWindowVisible())
			return;
		Refresh();
	}
	CDockablePane::OnTimer(nIDEvent);
}
