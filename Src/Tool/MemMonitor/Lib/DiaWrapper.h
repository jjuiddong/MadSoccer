//------------------------------------------------------------------------
// Name:    DiaWrapper.h
// Author:  jjuiddong
// Date:    1/10/2013
// 
// Dia SDK 래퍼클래스
//------------------------------------------------------------------------
#ifndef __DIAWRAPPER_H__
#define __DIAWRAPPER_H__

#include <cvconst.h>


struct IDiaDataSource;
struct IDiaSession;
struct IDiaSymbol;

namespace dia
{
	typedef struct _SSymbolInfo
	{
		enum SymTagEnum type;
		enum DataKind kind;
		enum BasicType btype;
		enum UdtKind utype;
		LONG offset;			// offset (byte단위)
		ULONGLONG length;		// 데이타 길이 (byte 단위)
		ULONGLONG element_length;// type == SymTagArrayType 일때 유효함, 타입의 크기
		std::string name;		// 주로 변수 이름이 저장된다
		std::string typeName;	// 타입 이름
		std::list<_SSymbolInfo> childs;

		_SSymbolInfo()
		{
			type = SymTagNull;
			kind = DataIsUnknown;
			btype = btNoType;
			utype = UdtStruct;
			offset = 0;
			length = 0;
			element_length = 0;
		}
		_SSymbolInfo(const _SSymbolInfo &rhs)
		{
			operator=(rhs);
		}
		_SSymbolInfo& operator=(const _SSymbolInfo &rhs)
		{
			if (this != &rhs)
			{
				type = rhs.type;
				kind = rhs.kind;
				btype = rhs.btype;
				utype = rhs.utype;
				offset = rhs.offset;
				length = rhs.length;
				element_length = rhs.element_length;
				name = rhs.name;
				childs = rhs.childs;
			}
			return *this;
		}
	} SSymbolInfo;


	// Wrapper class
	class CDiaWrapper : public common::CSingleton<CDiaWrapper>
	{
	public:
		CDiaWrapper();
		virtual ~CDiaWrapper();

	protected:
		IDiaDataSource	*m_pDiaDataSource;
		IDiaSession		*m_pDiaSession;
		IDiaSymbol		*m_pGlobalSymbol;
		DWORD			m_dwMachineType;

	public:
		bool			Init(const std::string &pdbFileName);
		bool			FindType(const std::string &typeName, OUT SSymbolInfo &out);
		IDiaSymbol*		FindType_Dia(const std::string &typeName);

	protected:
		bool			SetSymbolInfo(IDiaSymbol *pSymbol, SSymbolInfo &info, bool procTypedef=false);
		bool			SetSymbolName(IDiaSymbol *pSymbol, SSymbolInfo &info);
		bool			SetSymbolLocation(IDiaSymbol *pSymbol, SSymbolInfo &info);
		bool			SetSymbolType(IDiaSymbol *pSymbol, SSymbolInfo &info);
		bool			SetSymbolChild(IDiaSymbol *pSymbol, SSymbolInfo &info);

		bool			SetSymbol_UDT(IDiaSymbol *pSymbol, SSymbolInfo &info);
		bool			SetSymbol_Data(IDiaSymbol *pSymbol, SSymbolInfo &info);
		bool			SetSymbol_Array(IDiaSymbol *pSymbol, SSymbolInfo &info);

		ULONGLONG		GetSymbolLength(IDiaSymbol *pSymbol);
		std::string		GetSymbolTypeName(IDiaSymbol *pSymbol);
		BasicType		GetSymbolBasicType(IDiaSymbol *pSymbol);
	};


	// dia Global Functions
	std::string			GetTypeName(const SSymbolInfo &info);
	_variant_t			GetValueFromAddress(void *ptr, const SSymbolInfo &info);

}

#endif // __DIAWRAPPER_H__
