
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
// 
//------------------------------------------------------------------------
IDiaSymbol* CDiaWrapper::FindType(const std::string &typeName)
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
// BasicType을 리턴한다.
//------------------------------------------------------------------------
// BasicType CDiaWrapper::GetSymbolBasicType(IDiaSymbol *pSymbol)
// {
// 	BasicType reval = btNoType;
// 	CComPtr<IDiaSymbol> pBaseType;
// 	if (pSymbol->get_type( &pBaseType ) == S_OK)
// 	{
// 		HRESULT hr = pBaseType->get_baseType((DWORD *)&reval);
// 		//assert(S_OK == hr);
// 	}
// 	else
// 	{
// 		HRESULT hr = pSymbol->get_baseType((DWORD *)&reval);
// 		//assert(S_OK == hr);
// 	}
// 	return reval;
// }


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
// std::string	CDiaWrapper::GetSymbolTypeName(IDiaSymbol *pSymbol)
// {
// 	std::string reval;
// 
// 	CComPtr<IDiaSymbol> pBaseType;
// 	if (pSymbol->get_type( &pBaseType ) == S_OK)
// 	{
// 		BSTR name;
// 		if (pBaseType->get_name(&name) == S_OK)
// 		{
// 			reval = common::wstring2string(name);	
// 			SysFreeString(name);
// 		}
// 	}
// 	if (reval.empty())
// 	{
// 		BSTR name;
// 		if (pSymbol->get_name(&name) == S_OK)
// 		{
// 			reval = common::wstring2string(name);	
// 			SysFreeString(name);
// 		}
// 	}
// 	return reval;
// }


//------------------------------------------------------------------------
// 심볼의 타입정보를 리턴한다.
// pSymbol 은 데이타를 가르키든, 타입을 가르키든 
// 그 심볼에 해당하는 타입이름을 리턴한다.
//------------------------------------------------------------------------
std::string	dia::GetSymbolTypeName(IDiaSymbol *pSymbol)
{
	HRESULT hr;
	std::string typeName;

 	enum SymTagEnum symtag;
 	hr =  pSymbol->get_symTag((DWORD*)&symtag);
	RETV(S_FALSE == hr, typeName);

	switch (symtag)
	{
	case SymTagBaseType:
		{
			BasicType btype;
			hr = pSymbol->get_baseType((DWORD*)&btype);
			RETV(S_FALSE == hr, typeName);

			ULONGLONG length;
			hr = pSymbol->get_length(&length);
			RETV(S_FALSE == hr, typeName);

			typeName = GetBasicTypeName(btype, length);
		}
		break;

	case SymTagEnum:
	case SymTagUDT:
		typeName = GetSymbolName(pSymbol);
		break;

	case SymTagData:
	case SymTagArrayType:
	case SymTagPointerType:
		{
			CComPtr<IDiaSymbol> pBaseType;
			hr = pSymbol->get_type(&pBaseType);
			RETV(S_FALSE == hr, typeName);

			typeName = GetSymbolTypeName(pBaseType);
		}
		break;

	default:
 		typeName = "NoType";
		break;
	}

 	if (SymTagArrayType == symtag)
 		typeName += " array";
	else if (SymTagPointerType == symtag)
		typeName += " *";

	return typeName;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
std::string dia::GetBasicTypeName(BasicType btype, ULONGLONG length)
{
	std::string typeName;
	switch (btype)
	{
	case btNoType:	typeName = "NoType"; break;
	case btVoid:	typeName = "void";  break;
	case btChar:	typeName = "char";  break;
	case btWChar:	typeName = "wchar";  break;
	case btInt:
		switch(length)
		{
		case 1: typeName = "char";  break;
		case 2: typeName = "short";  break;
		default: typeName = "int";  break;
		}
		break;

	case btUInt:
		switch(length)
		{
		case 1: typeName = "BYTE";  break;
		case 2: typeName = "u_short";  break;
		default: typeName = "u_int";  break;
		}
		break;

	case btFloat:	
		switch(length)
		{
		case 4: typeName = "float";  break;
		case 8: typeName = "double";  break;
		default: typeName = "float";  break;
		}
		break;
	case btBCD:		typeName = "bcd";  break;
	case btBool:	typeName = "bool";  break;
	case btLong:	typeName = "long";  break;
	case btULong:	typeName = "u_long";  break;
	case btCurrency:typeName = "currency";  break;
	case btDate:	typeName = "date";  break;
	case btVariant:	typeName = "variant";  break;
	case btComplex:	typeName = "complex";  break;
	case btBit:		typeName = "bit";  break;
	case btBSTR:	typeName = "bstr";  break;
	case btHresult:	typeName = "hresult";  break;
	default: typeName = "NoType";  break;
	}
	return typeName;
}


// pSymbol 은 데이타를 가리키는 심볼이어야 한다.
_variant_t dia::GetValueFromAddress(void *srcPtr, const BasicType btype, const ULONGLONG length )
{
	_variant_t value;	
	switch (btype)
	{
	case btBool: value = *(bool*)srcPtr; break;
	case btChar: value = *(char*)srcPtr; break;
	case btInt:
		switch(length)
		{
		case 1: value = *(char*)srcPtr; break;
		case 2: value = *(short*)srcPtr; break;
		default: value = *(int*)srcPtr; break;
		}
		break;		

	case btUInt:
		switch(length)
		{
		case 1: value = *(unsigned char*)srcPtr; break;
		case 2: value = *(unsigned short*)srcPtr; break;
		default: value = *(unsigned int*)srcPtr; break;
		}
		break;

	case btLong: value = *(long*)srcPtr; break;
	case btULong: value = *(unsigned long*)srcPtr; break;
	case btFloat: 
		switch(length)
		{
		case 4: value = *(float*)srcPtr; break;
		case 8: value = *(double*)srcPtr; break;
		default: value = *(float*)srcPtr; break;
		}
		break;

	case btBSTR:
	default:
		{
		}
		break;
	}	
	return value;
}


//------------------------------------------------------------------------
// srcPtr 메모리에 있는 데이타를 varType 형태로 얻어와 리턴한다.
//------------------------------------------------------------------------
_variant_t dia::GetValue(void *srcPtr, VARTYPE varType)
{
	_variant_t value;
	switch (varType)
	{
	case VT_I1: value  = *(char*)srcPtr; break;
	case VT_I2: value  = *(short*)srcPtr; break;
	case VT_I4: value  = *(long*)srcPtr ; break;
	case VT_R4: value  = *(float*)srcPtr; break;
	case VT_R8: value  = *(double*)srcPtr; break;
	case VT_BOOL: value  = *(bool*)srcPtr; break;
	case VT_DECIMAL: break;
	case VT_UI1: value  = *(unsigned char*)srcPtr; break;
	case VT_UI2: value  = *(unsigned short*)srcPtr; break;
	case VT_UI4: value  = *(unsigned long*)srcPtr; break;
	case VT_INT: value  = *(int*)srcPtr; break;
	case VT_UINT: value  = *(unsigned int*)srcPtr; break;
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
		break;
	}
	assert( value.vt == varType);
	return value;
}


//------------------------------------------------------------------------
// destPtr 에 value를 저장한다.
//------------------------------------------------------------------------
void	dia::SetValue(void *destPtr, _variant_t value)
{
	switch (value.vt)
	{
	case VT_I2: *(short*)destPtr = value.iVal; break;
	case VT_I4: *(long*)destPtr = value.lVal; break;
	case VT_R4: *(float*)destPtr = value.fltVal; break;
	case VT_R8: *(double*)destPtr = value.dblVal; break;
	case VT_BOOL: *(bool*)destPtr = value.boolVal? true : false; break;
	case VT_DECIMAL: break;
	case VT_UI2: *(unsigned short*)destPtr = value.uiVal; break;
	case VT_UI4: *(unsigned int*)destPtr = value.uintVal; break;
	case VT_INT: *(int*)destPtr = value.intVal; break;
	case VT_UINT: *(unsigned int*)destPtr = value.uintVal; break;
	case VT_I1: *(char*)destPtr = value.cVal; break;
	case VT_UI1: *(unsigned char*)destPtr = value.bVal; break;
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
		break;
	}
}


//------------------------------------------------------------------------
// 심볼 이름을 리턴한다.
//------------------------------------------------------------------------
std::string dia::GetSymbolName(IDiaSymbol *pSymbol)
{
	BSTR bstrName;
	BSTR bstrUndName;

	if (pSymbol->get_name(&bstrName) == S_FALSE)
		return "";

	std::string name;
	if (pSymbol->get_undecoratedName(&bstrUndName) == S_OK) {
		if (wcscmp(bstrName, bstrUndName) == 0) {
			name = common::wstring2string(bstrName);
		}
		else {
			name = common::wstring2string(bstrName) +
				"(" + common::wstring2string(bstrName) + ")";
		}

		SysFreeString(bstrUndName);
	}
	else {
		name = common::wstring2string(bstrName);
	}

	SysFreeString(bstrName);
	return name;
}


//------------------------------------------------------------------------
// offset 값을 리턴한다.
//------------------------------------------------------------------------
LONG dia::GetSymbolLocation(IDiaSymbol *pSymbol, OUT LocationType *pLocType) // pLocType=NULL
{
	// 	DWORD dwRVA, dwSect, dwOff, dwReg, dwBitPos, dwSlot;
	LONG lOffset = 0;
	ULONGLONG ulLen = 0;
	VARIANT vt = { VT_EMPTY };

	if (pLocType)
		*pLocType = LocIsNull;

	// 타입에 따라 실패할 수 있다. 예를들면 BaseClass Type
	LocationType locType;
	HRESULT hr = pSymbol->get_locationType((DWORD*)&locType); 

	switch (locType) 
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
//			info.offset = lOffset;

// 			IDiaSymbol *psymType;
// 			if (pSymbol->get_type(&psymType) == S_OK)
// 			{
// 				if (psymType->get_length(&ulLen) == S_OK)
// 				{
// 					info.length = ulLen;
// 				}
// 				psymType->Release();
// 			}
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

	if (pLocType)
		*pLocType = locType;

	return lOffset;
}



//------------------------------------------------------------------------
// BasicType을 리턴한다.
//------------------------------------------------------------------------
// BasicType dia::GetSymbolBasicType(IDiaSymbol *pSymbol)
// {
// 	BasicType reval = btNoType;
// 	HRESULT hr;
// 	CComPtr<IDiaSymbol> pBaseType;
// 	if (pSymbol->get_type( &pBaseType ) == S_OK)
// 	{
// 		hr = pBaseType->get_baseType((DWORD *)&reval);
// 	}
// 	return reval;
// }
