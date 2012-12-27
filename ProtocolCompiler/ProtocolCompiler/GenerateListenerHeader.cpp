
#include "Global.h"
#include "GenerateListenerHeader.h"

namespace compiler
{
	string n_className; // Protocol, ProtocolListener 공동으로 사용하고 있다.
	string n_protocolId;
	string n_fileName;

	// Write Protocol Listener Code
	bool WriteListenerCpp(sRmi *rmi, bool IsAddStdafxHeader);
	void WriteDeclProtocolList(FILE *fp, sProtocol *pProtocol, bool isVirtual, bool isImpl, bool isTarget);
	void WriteFirstArg(FILE *fp, sArg*p, bool isTarget);
	void WriteArg(FILE *fp, sArg *arg, bool comma);
	void WriteArgVar(FILE *fp, sArg *arg, bool comma);
	void WriteProtocolDispatchFunc(FILE *fp, sRmi *rmi);
	void WriteDispatchSwitchCase(FILE *fp, sProtocol *pProtocol, int packetId);
	void WriteDispatchImpleArg(FILE *fp, sArg*p);
	void WriteLastDispatchSwitchCase(FILE *fp, sProtocol *pProtocol);


	// Write Protocol Code
	bool WriteProtocolClassHeader(FILE *fp, sRmi *rmi);
	bool WriteProtocolCpp(FILE *fp, sRmi *rmi);
	void WriteImplProtocolList(FILE *fp, sProtocol *pProtocol, int packetId);
	void WriteFirstImpleProtocol(FILE *fp, sArg*p, int packetId);
	void WriteImpleArg(FILE *fp, sArg*p);
	void WriteLastImpleProtocol(FILE *fp);

}

using namespace compiler;


//------------------------------------------------------------------------
// ProtocolListener 클래스 헤더파일을 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteProtocolCode(sRmi *rmi)
{
	if (!rmi)
		return true;

	n_fileName = rmi->name;
	n_fileName += "Protocol.h";

	n_className = rmi->name + "ProtocolListener";
	n_protocolId = n_className + "_ID";

	FILE *fp;
	fopen_s(&fp, n_fileName.c_str(), "w");
	if (!fp) return false;

	fprintf( fp, "//------------------------------------------------------------------------\n" );
	fprintf( fp, "// Name:    %s\n", n_fileName.c_str());
	fprintf( fp, "// Author:  ProtocolCompiler (by jjuiddong)\n");
	fprintf( fp, "// Date:    \n");
	fprintf( fp, "//------------------------------------------------------------------------\n" );

	fprintf( fp, "#pragma once\n" );
	fprintf( fp, "\n" );
	fprintf( fp, "static const int %s = %d;\n", n_protocolId.c_str(), rmi->number );
	fprintf( fp, "\n" );

	// CProtocolListener class
	fprintf( fp, "// ProtocolListener\n" );
	fprintf( fp, "class %s : public network::IProtocolListener\n", n_className.c_str() );
	fprintf( fp, "{\n" );
	fprintf( fp, "public:\n" );
	fprintf( fp, "\t%s() : IProtocolListener(%s) {}\n", n_className.c_str(), n_protocolId.c_str() );
	fprintf( fp, "protected:\n");
	fprintf( fp, "\tvirtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;\n" );
	WriteDeclProtocolList( fp, rmi->protocol, true, true, false);
	fprintf( fp, "};\n" );
	fprintf( fp, "\n" );

	// CProtocol class
	fprintf( fp, "\n" );
	fprintf( fp, "// Protocol\n" );
	WriteProtocolClassHeader(fp, rmi);
	fclose(fp);

	const bool result = WriteListenerCpp(rmi, true);
	if (!result) return false;

	WriteProtocolCode(rmi->next);

	return result;
}


//------------------------------------------------------------------------
// Protocol 헤더 소스코드를 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteProtocolClassHeader(FILE *fp, sRmi *rmi)
{
	n_className = rmi->name + "Protocol";

	fprintf( fp, "class %s : public network::IProtocol\n", n_className.c_str() );
	fprintf( fp, "{\n" );
	fprintf( fp, "public:\n" );
	fprintf( fp, "\t%s() : IProtocol(%s) {}\n", n_className.c_str(), n_protocolId.c_str() );
	// print protocol list
	WriteDeclProtocolList( fp, rmi->protocol, false, false, true);
	fprintf( fp, "};\n" );

	return true;
}


//------------------------------------------------------------------------
// 헤더 클래스파일에 있을 프로토콜 리스트를 출력한다.
//------------------------------------------------------------------------
void compiler::WriteDeclProtocolList(FILE *fp, sProtocol *pProtocol, bool isVirtual, bool isImpl, bool isTarget)
{
	if (!pProtocol) return;
	fprintf(fp, "\t" );
	if (isVirtual)
		fprintf(fp, "virtual ");
	fprintf( fp, "void %s(", pProtocol->name.c_str() );
	WriteFirstArg(fp, pProtocol->argList, isTarget);
	fprintf( fp, ")" );
	if (isImpl)
		fprintf(fp, "{}");
	else
		fprintf(fp, ";");
	fprintf(fp, "\n");
	WriteDeclProtocolList(fp, pProtocol->next, isVirtual, isImpl, isTarget);
}


//------------------------------------------------------------------------
// Cpp 파일에 있을 프로토콜 리스트를 출력한다.
//------------------------------------------------------------------------
void compiler::WriteImplProtocolList(FILE *fp, sProtocol *pProtocol, int packetId)
{
	if (!pProtocol) return;
	fprintf( fp, "//------------------------------------------------------------------------\n");
	fprintf( fp, "// Protocol: %s\n", pProtocol->name.c_str());
	fprintf( fp, "//------------------------------------------------------------------------\n");
	fprintf( fp, "void %s::%s(", n_className.c_str(), pProtocol->name.c_str() );
	WriteFirstArg(fp, pProtocol->argList, true);
	fprintf( fp, ")\n" );
	fprintf( fp, "{\n" );
	WriteFirstImpleProtocol(fp, pProtocol->argList, packetId);
	fprintf( fp, "}\n" );
	fprintf( fp, "\n" );
	WriteImplProtocolList(fp, pProtocol->next, packetId+1);
}


//------------------------------------------------------------------------
// 프로토콜 인자리스트 출력
//------------------------------------------------------------------------
void compiler::WriteFirstArg(FILE *fp, sArg*p, bool isTarget)
{
	if (isTarget)
		fprintf( fp, "netid targetId" );
	else
		fprintf( fp, "netid senderId" );
	WriteArg(fp, p, true);
}
void compiler::WriteArg(FILE *fp, sArg *arg, bool comma)
{
	if (!arg) return;
	if (comma)
		fprintf( fp, ", " );
	fprintf( fp, "const %s &%s", arg->var->type.c_str(), arg->var->var.c_str());
	WriteArg(fp, arg->next, true);
}

//------------------------------------------------------------------------
// 프로토콜 인자값의 변수 이름만 출력한다. (함수 호출시 사용)
//------------------------------------------------------------------------
void compiler::WriteArgVar(FILE *fp, sArg *arg, bool comma)
{
	if (!arg) return;
	if (comma)
		fprintf( fp, ", " );
	fprintf( fp, "%s", arg->var->var.c_str());
	WriteArgVar(fp, arg->next, true);
}


//------------------------------------------------------------------------
// Cpp 파일에 들어갈 프로토콜 함수의 처음에 나올 코드
//------------------------------------------------------------------------
void compiler::WriteFirstImpleProtocol(FILE *fp, sArg*p, int packetId)
{
	fprintf( fp, "\tCPacket packet;\n" );
	fprintf( fp, "\tpacket << GetId();\n" );
	fprintf( fp, "\tpacket << %d;\n", packetId );
	WriteImpleArg(fp, p);
	WriteLastImpleProtocol(fp);
}


//------------------------------------------------------------------------
// Cpp 파일에 들어갈 프로토콜 코드에서 패킷에 인자값을 넣는 코드
//------------------------------------------------------------------------
void compiler::WriteImpleArg(FILE *fp, sArg*p)
{
	if (!p) return;
	fprintf( fp, "\tpacket << %s;\n", p->var->var.c_str());
	WriteImpleArg(fp, p->next);
}


//------------------------------------------------------------------------
// Cpp 파일에 들어갈 프로토콜 함수의 마지막에 나올 코드
//------------------------------------------------------------------------
void compiler::WriteLastImpleProtocol(FILE *fp)
{
	fprintf( fp, "\tGetNetConnector()->Send(targetId, packet);\n" );
}



//------------------------------------------------------------------------
// ProtocolListener Cpp 파일 생성
//------------------------------------------------------------------------
bool compiler::WriteListenerCpp(sRmi *rmi, bool IsAddStdafxHeader)
{
	string fileName = rmi->name;
	fileName += "Protocol.cpp";
	string headerFileName = rmi->name;
	headerFileName += "Protocol.h";

	FILE *fp;
	fopen_s(&fp, fileName.c_str(), "w");
	if (!fp) return false;

	if (IsAddStdafxHeader)
		fprintf( fp, "#include \"stdafx.h\"\n");
	fprintf( fp, "#include \"%s\"", headerFileName.c_str());
	fprintf( fp, "\n");
	fprintf( fp, "using namespace network;\n");
	fprintf( fp, "\n");

	n_className = rmi->name + "ProtocolListener";
	WriteProtocolDispatchFunc(fp, rmi);
	fprintf( fp, "\n" );
	fprintf( fp, "\n" );

	// Protocol class Cpp
	WriteProtocolCpp(fp, rmi);
	fclose(fp);
	return true;
}


//------------------------------------------------------------------------
// ProtocolListener Cpp 파일 생성
//------------------------------------------------------------------------
bool compiler::WriteProtocolCpp(FILE *fp, sRmi *rmi)
{
	n_className = rmi->name + "Protocol";
	WriteImplProtocolList( fp, rmi->protocol, rmi->number );
	return true;
}


//------------------------------------------------------------------------
// Listener::Dispatch() 함수 코드 생성
//------------------------------------------------------------------------
void compiler::WriteProtocolDispatchFunc(FILE *fp, sRmi *rmi)
{
	fprintf( fp, "//------------------------------------------------------------------------\n");
	fprintf( fp, "// // 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.\n" );
	fprintf( fp, "//------------------------------------------------------------------------\n");
	fprintf( fp, "void %s::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)\n", 
		n_className.c_str() );
	fprintf( fp, "{\n" );
	fprintf( fp, "\tBOOST_FOREACH(ProtocolListenerPtr p, listeners)\n");
	fprintf( fp, "\t{\n" );
		fprintf( fp, "\t\tIProtocolListener *ptmp = p;\n");
		fprintf( fp, "\t\t%s *lstr = static_cast<%s*>(ptmp);\n", n_className.c_str(), n_className.c_str());
			fprintf( fp, "\t\tif (!lstr)\n");
			fprintf( fp, "\t\t{\n");
				fprintf( fp, "\t\t\terror::ErrorLog( \"%s::Dispatch Convert Error\" );\n", n_className.c_str());
				fprintf( fp, "\t\t\tcontinue;\n");
			fprintf( fp, "\t\t}\n");
	fprintf( fp, "\n" );
		fprintf( fp, "\t\tint protocolId, packetId;\n");
		fprintf( fp, "\t\tpacket >> protocolId >> packetId;\n");

	if (rmi->protocol)
	{
			fprintf( fp, "\t\tswitch (packetId)\n");
			fprintf( fp, "\t\t{\n");

			WriteDispatchSwitchCase(fp, rmi->protocol, rmi->number);

			fprintf( fp, "\t\tdefault:\n");
				fprintf( fp, "\t\t\tassert(0);\n");
				fprintf( fp, "\t\t\tbreak;\n");
			fprintf( fp, "\t\t}\n");
	}
	fprintf( fp, "\t}\n");
	fprintf( fp, "}\n" );	
}


//------------------------------------------------------------------------
// Dispatch 함수에서case 문 코드 생성
//------------------------------------------------------------------------
void compiler::WriteDispatchSwitchCase(FILE *fp, sProtocol *pProtocol, int packetId)
{
	if (!pProtocol) return;

	fprintf( fp, "\t\tcase %d:\n", packetId );
	fprintf( fp, "\t\t\t{\n" );

	WriteDispatchImpleArg(fp, pProtocol->argList);
	WriteLastDispatchSwitchCase(fp, pProtocol);

	fprintf( fp, "\t\t\t}\n" );
	fprintf( fp, "\t\t\tbreak;\n" );
	fprintf( fp, "\n" );

	WriteDispatchSwitchCase(fp, pProtocol->next, packetId+1);
}


//------------------------------------------------------------------------
// Dispatch 함수의 switch case 문에 들어가는 각 패킷 인자를 얻어오는 코드를
// 생성한다.
//------------------------------------------------------------------------
void compiler::WriteDispatchImpleArg(FILE *fp, sArg*p)
{
	if (!p) return;
	// 변수 선언
	fprintf( fp, "\t\t\t\t%s %s;\n", p->var->type.c_str(), p->var->var.c_str() );
	// 패킷에서 데이타 얻음
	fprintf( fp, "\t\t\t\tpacket >> %s;\n", p->var->var.c_str() );
	WriteDispatchImpleArg(fp, p->next);
}


//------------------------------------------------------------------------
// Dispatch 함수의 switch case 문의 마지막에 들어가는 코드로, 리스너의 
// 해당하는 프로토콜을 호출하는 코드를 생성한다.
//------------------------------------------------------------------------
void compiler::WriteLastDispatchSwitchCase(FILE *fp, sProtocol *pProtocol)
{
	fprintf( fp, "\t\t\t\tlstr->%s(packet.GetSenderId()", pProtocol->name.c_str() );
	WriteArgVar(fp, pProtocol->argList, true );
	fprintf( fp, ");\n" );
}
