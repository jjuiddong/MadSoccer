//------------------------------------------------------------------------
// Name:    DiaWrapper.h
// Author:  jjuiddong
// Date:    1/10/2013
// 
// Dia SDK 래퍼클래스
//------------------------------------------------------------------------
#pragma once

#include <cvconst.h>


typedef struct _SSymbolInfo
{
	enum SymTagEnum type;
	enum DataKind kind;
	enum BasicType btype;
	enum UdtKind utype;
	LONG offset;			// offset (byte단위)
	ULONGLONG length;		// 데이타 길이 (byte 단위)
	std::string name;
	std::list<_SSymbolInfo> childs;

	_SSymbolInfo()
	{
		type = SymTagNull;
		kind = DataIsUnknown;
		btype = btNoType;
		utype = UdtStruct;
		offset = 0;
		length = 0;
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
			name = rhs.name;
			childs = rhs.childs;
		}
		return *this;
	}
} SSymbolInfo;


struct IDiaDataSource;
struct IDiaSession;
struct IDiaSymbol;
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

protected:
	bool			SetSymbolInfo(IDiaSymbol *pSymbol, SSymbolInfo &info, bool procTypedef=false);
	bool			SetSymbolName(IDiaSymbol *pSymbol, SSymbolInfo &info);
	bool			SetSymbolUDT(IDiaSymbol *pSymbol, SSymbolInfo &info);
	bool			SetSymbolType(IDiaSymbol *pSymbol, SSymbolInfo &info);
	bool			SetSymbolChild(IDiaSymbol *pSymbol, SSymbolInfo &info);
	bool			SetSymbolData(IDiaSymbol *pSymbol, SSymbolInfo &info);
	bool			SetSymbolLocation(IDiaSymbol *pSymbol, SSymbolInfo &info);

};
