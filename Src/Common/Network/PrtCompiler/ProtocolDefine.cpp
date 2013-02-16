
#include "stdafx.h"
#include "ProtocolDefine.h"

using namespace network;


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void network::PrintToken( Tokentype token, char *szTokenString )
{
	switch( token )
	{
	case ID:
	case STRING:	printf( "string = %s\n", szTokenString ); break;

	case ASSIGN:	printf( "=" ); break;
	case TIMES:		printf( "*" ); break;
	case LPAREN:	printf( "(" ); break;
	case RPAREN:	printf( ")" ); break;
	case LBRACE:	printf( "{" ); break;
	case RBRACE:	printf( "}" ); break;
	case COMMA:		printf( "," ); break;
	case NUM:		printf( "number" ); break;
	case OR:		printf( "||" ); break;
	case AND:		printf( "&&" ); break;

	default: 		printf( "UnKnown token %d, %s\n", token, szTokenString ); break;
	}
}


//------------------------------------------------------------------------
// typeStr: sArg->var->type
// ��Ʈ���� Ÿ������ �����Ѵ�.
//------------------------------------------------------------------------
_variant_t network::GetTypeStr2Type(const std::string &typeStr)
{
	if (typeStr == "std::string")
	{
		char *v=NULL;
		return _variant_t(v);
	}
	else if (typeStr == "string")
	{
		char *v=NULL;
		return _variant_t(v);
	}
	else if (typeStr == "float")
	{
		float v=0.f;
		return _variant_t(v);
	}
	else if (typeStr == "double")
	{
		double v=0.f;
		return _variant_t(v);
	}
	else if (typeStr == "int")
	{
		int v=0;
		return _variant_t(v);
	}
	else if (typeStr == "char")
	{
		char v='a';
		return _variant_t(v);
	}
	else if (typeStr == "short")
	{
		short v=0;
		return _variant_t(v);
	}
	else if (typeStr == "long")
	{
		long v=0;
		return _variant_t(v);
	}
	else if (typeStr == "bool")
	{
		bool v=true;
		return _variant_t(v);
	}
	else if (typeStr == "BOOL")
	{
		bool v=true;
		return _variant_t(v);
	}

	return _variant_t(1);
}


//------------------------------------------------------------------------
// ���ڷ� �Ѿ�� value ��Ʈ���� valueType ��Ʈ�� Ÿ������ ��ȯ�ؼ� ���Ͻ�Ų��.
//------------------------------------------------------------------------
// _variant_t	network::String2Variant(const std::string &valueType, const std::string &value)
// {
// 	_variant_t varType = GetTypeStr2Type(valueType);
// 	_variant_t var = varType;
// 	switch (varType.vt)
// 	{
// 	case VT_I2: var.iVal = (short)atoi(value.c_str()); break;
// 	case VT_I4: var.lVal = (long)atoi(value.c_str()); break;
// 	case VT_R4: var.fltVal = (float)atof(value.c_str()); break;
// 	case VT_R8: var.dblVal = atof(value.c_str()); break;
// 
// 	case VT_BSTR:
// 		{
// #ifdef _UNICODE
// 			var.bstrVal = (_bstr_t)common::string2wstring(value).c_str();
// #else
// 			var.bstrVal = (_bstr_t)value.c_str();
// #endif
// 		}
// 		break;
// 
// 	case VT_DECIMAL:
// 	case VT_I1:
// 	case VT_UI1:
// 	case VT_UI2:
// 	case VT_UI4:
// 		break;
// 
// 	case VT_INT: var.intVal = (int)atoi(value.c_str()); break;
// 	case VT_UINT: var.uintVal = strtoul(value.c_str(),NULL,0); break;
// 	default:
// 		break;
// 	}
// 
// 	return var;
// }


//------------------------------------------------------------------------
// packetID�� �����Ѵ�. ���⼭ Packet�̶� sProtocol protocol�� �ǹ��ϰ�, 
// sRmi �� �ڽ����� ������� ��ȣ�� �Ű��� ���� Packet ID�̴�.
//------------------------------------------------------------------------
int	network::GetPacketID(sRmi *rmi, sProtocol *packet)
{
	if (!rmi) return 0;
	if (!packet) return 0;

	int id = rmi->number + 1;
	sProtocol *p = rmi->protocol;
	while (p)
	{
		if (p == packet)
			break;
		++id;
		p = p->next;
	}
	return id;
}


//------------------------------------------------------------------------
// ��Ŷ������ ��Ʈ������ ��ȯ �Ѵ�.
//------------------------------------------------------------------------
std::string network::Packet2String(const CPacket &packet, sProtocol *protocol)
{
	if (!protocol) return "";

	std::stringstream ss;
	CPacket tempPacket = packet;

	int protocolID, packetID;
	tempPacket >> protocolID >> packetID;

	ss << protocol->name << " ";

	sArg *arg = protocol->argList;
	while (arg)
	{
		const _variant_t varType = GetTypeStr2Type(arg->var->type);
		const _variant_t var = tempPacket.GetVariant(varType);

		ss << arg->var->var + " = ";
		ss << common::variant2str(var);
		ss << ", ";

		arg = arg->next;
	}

	return ss.str();
}
