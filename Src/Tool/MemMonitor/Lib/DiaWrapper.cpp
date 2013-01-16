
#include "stdafx.h"
#include "DiaWrapper.h"
#include "dia2.h"

using namespace std;
using namespace dia;


CDiaWrapper::CDiaWrapper() :
	m_pDiaDataSource(NULL)
,	m_pDiaSession(NULL)
,	m_pGlobalSymbol(NULL)
,	m_dwMachineType(CV_CFL_80386)
{

}

CDiaWrapper::~CDiaWrapper()
{
	SAFE_RELEASE(m_pGlobalSymbol);
	SAFE_RELEASE(m_pDiaSession);
	SAFE_RELEASE(m_pDiaDataSource);

	CoUninitialize();
}


//------------------------------------------------------------------------
// DIA 초기화
// pdbFileName : PDB 파일명
//------------------------------------------------------------------------
bool CDiaWrapper::Init(const string &pdbFileName)
{
//	wchar_t wszExt[MAX_PATH];
	string wszSearchPath = "SRV**\\\\symbols\\symbols"; // Alternate path to search for debug data

	HRESULT hr = CoInitialize(NULL);

	// Obtain access to the provider
	hr = CoCreateInstance(__uuidof(DiaSource),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IDiaDataSource),
		(void **) &m_pDiaDataSource);
	if (FAILED(hr)) 
	{
// 		wprintf(L"CoCreateInstance failed - HRESULT = %08X\n", hr);
 		return false;
 	}

	hr = m_pDiaDataSource->loadDataFromPdb( common::string2wstring(pdbFileName).c_str() );
	if (FAILED(hr)) 
		return false;

	// Open a session for querying symbols
	hr = m_pDiaDataSource->openSession(&m_pDiaSession);
	if (FAILED(hr)) {
//	    wprintf(L"openSession failed - HRESULT = %08X\n", hr);
		return false;
	}

	// Retrieve a reference to the global scope
	hr = m_pDiaSession->get_globalScope(&m_pGlobalSymbol);
	if (hr != S_OK) {
//		wprintf(L"get_globalScope failed\n");
		return false;
	}

	// Set Machine type for getting correct register names
	DWORD dwMachType = 0;
	if (m_pGlobalSymbol->get_machineType(&dwMachType) == S_OK) {
		switch (dwMachType) {
	  case IMAGE_FILE_MACHINE_I386 : m_dwMachineType = CV_CFL_80386; break;
	  case IMAGE_FILE_MACHINE_IA64 : m_dwMachineType = CV_CFL_IA64; break;
	  case IMAGE_FILE_MACHINE_AMD64 : m_dwMachineType = CV_CFL_AMD64; break;
		}
	}

	return true;
}


//------------------------------------------------------------------------
// 타입정보를 얻는다. 
//------------------------------------------------------------------------
bool CDiaWrapper::FindType(const std::string &typeName, OUT SSymbolInfo &out)
{
	if (!m_pGlobalSymbol)
		return false;

	IDiaEnumSymbols *pEnumSymbols;
	if (FAILED(m_pGlobalSymbol->findChildren(SymTagNull, common::string2wstring(typeName).c_str(), 
		nsRegularExpression, &pEnumSymbols))) 
		return false;

	IDiaSymbol *pSymbol;
	ULONG celt = 0;
	// 첫번째로 발견되는 정보만 찾아서 리턴한다.
	while (SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1)) 
	{
		SetSymbolInfo(pSymbol, out, true);
		pSymbol->Release();
		break;
	}

	pEnumSymbols->Release();
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
IDiaSymbol* CDiaWrapper::FindType_Dia(const std::string &typeName)
{
	IDiaEnumSymbols *pEnumSymbols;
	if (FAILED(m_pGlobalSymbol->findChildren(SymTagNull, common::string2wstring(typeName).c_str(), 
		nsRegularExpression, &pEnumSymbols))) 
		return NULL;

	IDiaSymbol *pSymbol;
	ULONG celt = 0;
	// 첫번째로 발견되는 정보만 찾아서 리턴한다.
	while (SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1)) 
	{
		pEnumSymbols->Release();
		return pSymbol;
	}
	return NULL;
}



//------------------------------------------------------------------------
// 심볼 타입 정보 설정
// procTypedef : true 일때만 typedef 를 처리한다.
//------------------------------------------------------------------------
bool CDiaWrapper::SetSymbolInfo(IDiaSymbol *pSymbol, SSymbolInfo &info, bool procTypedef) // procTypedef=false
{
	DWORD dwSymTag;
	if (pSymbol->get_symTag(&dwSymTag) != S_OK)
		return false;
	info.type = (enum SymTagEnum)dwSymTag;

	switch (dwSymTag) 
	{
	case SymTagData:
 		SetSymbol_Data(pSymbol, info);
		break;

	case SymTagTypedef:
	case SymTagVTable:
		if (procTypedef)
			SetSymbolType(pSymbol, info);
		break;

	case SymTagEnum:
	case SymTagUDT:
 		SetSymbol_UDT(pSymbol, info);
		SetSymbolChild(pSymbol, info);
		break;

	case SymTagArrayType:
		SetSymbol_Array(pSymbol, info);
		break;

	case SymTagFunction:
		break;
	case SymTagPointerType:
		break;
	case SymTagBaseType:
	case SymTagFunctionArgType:
	case SymTagUsingNamespace:
	case SymTagCustom:
	case SymTagFriend:
		break;
	case SymTagVTableShape:
	case SymTagBaseClass:
		break;
	case SymTagFunctionType:
		break;
	case SymTagThunk:
		break;

	default:
//		wprintf(L"ERROR - PrintTypeInDetail() invalid SymTag\n");
		break;
	}

	return true;
}


//------------------------------------------------------------------------
// 심볼이름 설정
//------------------------------------------------------------------------
bool CDiaWrapper::SetSymbolName(IDiaSymbol *pSymbol, SSymbolInfo &info)
{
	BSTR bstrName;
	BSTR bstrUndName;

	if (pSymbol->get_name(&bstrName) != S_OK)
		return false;

	if (pSymbol->get_undecoratedName(&bstrUndName) == S_OK) {
		if (wcscmp(bstrName, bstrUndName) == 0) {
			info.name = common::wstring2string(bstrName);
		}
		else {
			info.name = common::wstring2string(bstrName) +
						"(" + common::wstring2string(bstrName) + ")";
		}

		SysFreeString(bstrUndName);
	}
	else {
		info.name = common::wstring2string(bstrName);
	}

	SysFreeString(bstrName);
	return true;
}


//------------------------------------------------------------------------
// user define type 설정
//------------------------------------------------------------------------
bool CDiaWrapper::SetSymbol_UDT(IDiaSymbol *pSymbol, SSymbolInfo &info)
{
	const bool r1 = SetSymbolName(pSymbol, info);
	const bool r2 = SetSymbolType(pSymbol, info);

	// get byte size
	ULONGLONG length=0;
	if (pSymbol->get_length(&length) != S_OK) {
		return false;
	}
	info.length = length;

	return r1 && r2;
}


//------------------------------------------------------------------------
// 타입 정보 설정
//------------------------------------------------------------------------
bool CDiaWrapper::SetSymbolType(IDiaSymbol *pSymbol, SSymbolInfo &info)
{
	IDiaSymbol *pType;
	if (pSymbol->get_type(&pType) == S_OK) {
		SetSymbolInfo(pType, info);
		pType->Release();
	}
	return true;
}


//------------------------------------------------------------------------
// 심볼 자식 설정
//------------------------------------------------------------------------
bool CDiaWrapper::SetSymbolChild(IDiaSymbol *pSymbol, SSymbolInfo &info)
{
	IDiaEnumSymbols *pEnumChildren;
	IDiaSymbol *pChild;
	ULONG celt = 0;

	if (SUCCEEDED(pSymbol->findChildren(SymTagNull, NULL, nsNone, &pEnumChildren))) {
		while (SUCCEEDED(pEnumChildren->Next(1, &pChild, &celt)) && (celt == 1)) {

			SSymbolInfo data;
			SetSymbolInfo(pChild, data);
			info.childs.push_back(data);

			pChild->Release();
		}

		pEnumChildren->Release();
	}

	return true;
}


//------------------------------------------------------------------------
// basedata 설정
//------------------------------------------------------------------------
bool CDiaWrapper::SetSymbol_Data(IDiaSymbol *pSymbol, SSymbolInfo &info)
{
	DWORD dwDataKind;
// 	DWORD dwLiveRanges = 0;
// 	DWORD dwRVAStart, dwSectStart, dwOffsetStart;
// 	DWORD dwRVAEnd, dwSectEnd, dwOffsetEnd;
//	BSTR bstrProgram;

	// offset, length 설정
	SetSymbolLocation(pSymbol, info);

	if (pSymbol->get_dataKind(&dwDataKind) != S_OK) {
//		wprintf(L"ERROR - PrintData() get_dataKind");
		return false;
	}
	info.kind = (DataKind)dwDataKind;

//	wprintf(L", %s", SafeDRef(rgDataKind, dwDataKind));
//	SetSymbolType(pSymbol, info);

//	wprintf(L", ");
	SetSymbolName(pSymbol, info);
	info.btype = GetSymbolBasicType(pSymbol);
	if (btNoType == info.btype)
	{
		// array data, udt data
		SetSymbolType(pSymbol, info);
	}
	else
	{
//		info.typeName = GetSymbolTypeName(pSymbol);
	}


// 	if (pSymbol->get_liveLVarInstances(0, dwLiveRanges, &dwLiveRanges, NULL) == S_OK) {
// 		// Symbol has live ranges
// 
// //		putwchar(L'\n');
// 
// 		// Dump Live ranges
// 
// 		IDiaLVarInstance **rgpLiveRanges = new IDiaLVarInstance*[dwLiveRanges];
// 
// 		if (pSymbol->get_liveLVarInstances(0, dwLiveRanges, &dwLiveRanges, rgpLiveRanges) != S_OK) {
// //			wprintf(L"ERROR - PrintData() get_liveLVarInstances\n");
// 			return false;
// 		}
// 
// 		for (DWORD i = 0; i < dwLiveRanges; i++) {
// 			IDiaLVarInstance *pRange = rgpLiveRanges[i];
// 
// 			wprintf(L"             ");
// 			for (DWORD j = 0; j < dwIndent; j++) {
// 				wprintf(L"  ");
// 			}
// 
// 			if ((pRange->get_rvaStart(&dwRVAStart) == S_OK) &&
// 				(pRange->get_sectionStart(&dwSectStart) == S_OK) &&
// 				(pRange->get_offsetStart(&dwOffsetStart) == S_OK) &&
// 				(pRange->get_rvaEnd(&dwRVAEnd) == S_OK) &&
// 				(pRange->get_sectionEnd(&dwSectEnd) == S_OK) &&
// 				(pRange->get_offsetEnd(&dwOffsetEnd) == S_OK) &&
// 				(pRange->get_program(&bstrProgram) == S_OK)) {
// 					wprintf(L"[%08X][%04X:%08X]-[%08X][%04X:%08X] %s\n",
// 						dwRVAStart, dwSectStart, dwOffsetStart, dwRVAEnd, dwSectEnd, dwOffsetEnd, bstrProgram);
// 
// 					SysFreeString(bstrProgram);
// 			}
// 
// 			rgpLiveRanges[i]->Release();
// 		}
// 
// 		delete [] rgpLiveRanges;
// 	}

	return true;
}


//------------------------------------------------------------------------
// Array 타입 심볼설정
//------------------------------------------------------------------------
bool CDiaWrapper::SetSymbol_Array(IDiaSymbol *pSymbol, SSymbolInfo &info)
{
	info.element_length = GetSymbolLength(pSymbol);

	CComPtr<IDiaSymbol> pBaseType;
	if (pSymbol->get_type( &pBaseType ) == S_OK)
	{
		BasicType btBaseType;
		if (pBaseType->get_baseType((DWORD *)&btBaseType) == S_OK)
		{
			info.btype = btBaseType;
		}
	}

	return true;
}


//------------------------------------------------------------------------
// BasicType을 리턴한다.
//------------------------------------------------------------------------
BasicType CDiaWrapper::GetSymbolBasicType(IDiaSymbol *pSymbol)
{
	BasicType reval = btNoType;
	CComPtr<IDiaSymbol> pBaseType;
	if (pSymbol->get_type( &pBaseType ) == S_OK)
	{
		HRESULT hr = pBaseType->get_baseType((DWORD *)&reval);
		//assert(S_OK == hr);
	}
	else
	{
		HRESULT hr = pSymbol->get_baseType((DWORD *)&reval);
		//assert(S_OK == hr);
	}
	return reval;
}


//------------------------------------------------------------------------
// pSymbol의 데이타 길이를 리턴한다.
//------------------------------------------------------------------------
ULONGLONG CDiaWrapper::GetSymbolLength(IDiaSymbol *pSymbol)
{
	ULONGLONG len = 0;
	CComPtr<IDiaSymbol> psymType;
	if (pSymbol->get_type(&psymType) == S_OK)
	{
		HRESULT hr = psymType->get_length(&len);
		assert( hr == S_OK );
	}
	else
	{
		HRESULT hr = pSymbol->get_length(&len);
		assert( hr == S_OK );
	}
	return len;
}


//------------------------------------------------------------------------
// 타입이름을 리턴한다.
//------------------------------------------------------------------------
std::string	CDiaWrapper::GetSymbolTypeName(IDiaSymbol *pSymbol)
{
	std::string reval;

	CComPtr<IDiaSymbol> pBaseType;
	if (pSymbol->get_type( &pBaseType ) == S_OK)
	{
		BSTR name;
		if (pBaseType->get_name(&name) == S_OK)
		{
			reval = common::wstring2string(name);	
			SysFreeString(name);
		}
	}
	if (reval.empty())
	{
		BSTR name;
		if (pSymbol->get_name(&name) == S_OK)
		{
			reval = common::wstring2string(name);	
			SysFreeString(name);
		}
	}
	return reval;
}



//------------------------------------------------------------------------
// 심볼의 오프셋 설정
//------------------------------------------------------------------------
bool CDiaWrapper::SetSymbolLocation(IDiaSymbol *pSymbol, SSymbolInfo &info )
{
 	DWORD dwLocType;
// 	DWORD dwRVA, dwSect, dwOff, dwReg, dwBitPos, dwSlot;
 	LONG lOffset;
	ULONGLONG ulLen;
	VARIANT vt = { VT_EMPTY };

	info.offset = 0;
	if (pSymbol->get_locationType(&dwLocType) != S_OK) {
		// It must be a symbol in optimized code
//		wprintf(L"symbol in optmized code");
		return false;
	}

	switch (dwLocType) 
	{
	case LocIsStatic:
// 		if ((pSymbol->get_relativeVirtualAddress(&dwRVA) == S_OK) &&
// 			(pSymbol->get_addressSection(&dwSect) == S_OK) &&
// 			(pSymbol->get_addressOffset(&dwOff) == S_OK)) {
// 				wprintf(L"%s, [%08X][%04X:%08X]", SafeDRef(rgLocationTypeString, dwLocType), dwRVA, dwSect, dwOff);
// 		}
		break;

	case LocIsTLS:
	case LocInMetaData:
	case LocIsIlRel:
// 		if ((pSymbol->get_relativeVirtualAddress(&dwRVA) == S_OK) &&
// 			(pSymbol->get_addressSection(&dwSect) == S_OK) &&
// 			(pSymbol->get_addressOffset(&dwOff) == S_OK)) {
// 				wprintf(L"%s, [%08X][%04X:%08X]", SafeDRef(rgLocationTypeString, dwLocType), dwRVA, dwSect, dwOff);
// 		}
		break;

	case LocIsRegRel:
// 		if ((pSymbol->get_registerId(&dwReg) == S_OK) &&
// 			(pSymbol->get_offset(&lOffset) == S_OK)) {
// 				wprintf(L"%s Relative, [%08X]", SzNameC7Reg((USHORT) dwReg), lOffset);
// 		}
		break;

	case LocIsThisRel:
		if (pSymbol->get_offset(&lOffset) == S_OK) {
			info.offset = lOffset;

			IDiaSymbol *psymType;
			if (pSymbol->get_type(&psymType) == S_OK)
			{
				if (psymType->get_length(&ulLen) == S_OK)
				{
					info.length = ulLen;
				}
				psymType->Release();
			}
		}
		break;

	case LocIsBitField:
// 		if ((pSymbol->get_offset(&lOffset) == S_OK) &&
// 			(pSymbol->get_bitPosition(&dwBitPos) == S_OK) &&
// 			(pSymbol->get_length(&ulLen) == S_OK)) {
// 				wprintf(L"this(bf)+0x%X:0x%X len(0x%X)", lOffset, dwBitPos, ulLen);
// 		}
		break;

	case LocIsEnregistered:
// 		if (pSymbol->get_registerId(&dwReg) == S_OK) {
// 			wprintf(L"enregistered %s", SzNameC7Reg((USHORT) dwReg));
// 		}
		break;

	case LocIsNull:
//		wprintf(L"pure");
		break;

	case LocIsSlot:
// 		if (pSymbol->get_slot(&dwSlot) == S_OK) {
// 			wprintf(L"%s, [%08X]", SafeDRef(rgLocationTypeString, dwLocType), dwSlot);
//		}
		break;

	case LocIsConstant:
//		wprintf(L"constant");

		if (pSymbol->get_value(&vt) == S_OK) {
//			PrintVariant(vt);
		}
		break;

	default :
//		wprintf(L"Error - invalid location type: 0x%X", dwLocType);
		return false;
		break;
	}
	return true;
}


//------------------------------------------------------------------------
// 심볼정보 info 의 BasicType 정보를 참조해서 타입이름을 리턴한다.
//------------------------------------------------------------------------
std::string	dia::GetTypeName(const SSymbolInfo &info)
{
	std::string typeName;
	if (SymTagData == info.type || SymTagArrayType == info.type)
	{
		switch (info.btype)
		{
		case btNoType:	typeName = "NoType";
		case btVoid:	typeName = "void";
		case btChar:	typeName = "char";
		case btWChar:	typeName = "wchar";
		case btInt:
			switch(info.length)
			{
			case 1: typeName = "char";
			case 2: typeName = "short";
			default: typeName = "int";
			}
			break;

		case btUInt:
			switch(info.length)
			{
			case 1: typeName = "BYTE";
			case 2: typeName = "u_short";
			default: typeName = "u_int";
			}
			break;
			
		case btFloat:	typeName = "float";
		case btBCD:		typeName = "bcd";
		case btBool:	typeName = "bool";
		case btLong:	typeName = "long";
		case btULong:	typeName = "u_long";
		case btCurrency:typeName = "currency";
		case btDate:	typeName = "date";
		case btVariant:	typeName = "variant";
		case btComplex:	typeName = "complex";
		case btBit:		typeName = "bit";
		case btBSTR:	typeName = "bstr";
		case btHresult:	typeName = "hresult";
		default: typeName = "NoType";
		}
	}
	else
	{
		return "NoType";
	}

	if (SymTagArrayType == info.type)
		typeName += " array";

	return typeName;
}


//------------------------------------------------------------------------
// ptr 메모리 주소에 있는 정보를 SSymbolInfo 형태로 분석해서 리턴한다.
// ptr 은 offset까지 최종 계산된 주소여야 한다.
//------------------------------------------------------------------------
_variant_t dia::GetValueFromAddress(void *ptr, const SSymbolInfo &info)
{
	_variant_t value;
	switch (info.btype)
	{
	case btBool: value = *(bool*)ptr; break;
	case btChar: value = *(char*)ptr; break;
	case btInt:
		switch(info.length)
		{
		case 1: value = *(char*)ptr; break;
		case 2: value = *(short*)ptr; break;
		default: value = *(int*)ptr; break;
		}
		break;		

	case btUInt:
		switch(info.length)
		{
		case 1: value = *(unsigned char*)ptr; break;
		case 2: value = *(unsigned short*)ptr; break;
		default: value = *(unsigned int*)ptr; break;
		}
		break;

	case btLong: value = *(long*)ptr; break;
	case btULong: value = *(unsigned long*)ptr; break;
	case btFloat: value = *(float*)ptr; break;

	case btBSTR:
	default:
		{
		}
		break;
	}	
	return value;

}
