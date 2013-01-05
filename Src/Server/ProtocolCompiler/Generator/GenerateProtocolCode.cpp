
#include "stdafx.h"
#include "GenerateProtocolCode.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi")

using namespace network;
using namespace std;

namespace compiler
{
	string n_className; // Protocol, ProtocolListener 공동으로 사용하고 있다.
	string n_protocolId;
	string n_fileName;
	string n_protocolName;	// *.prt 파일의 확장자와 경로를 제외한 파일이름을 저장한다.
	string n_OrigianlFileName;
	string n_listenerClassName;
	
	

	// Write Protocol Listener Code
	bool WriteFirstListenerHeader(sRmi *rmi);
	bool WriteListenerHeader(FILE *fp, sRmi *rmi);
	bool WriteFirstListenerCpp(sRmi *rmi, bool IsAddStdafxHeader);
	bool WriteListenerCpp(FILE *fp, sRmi *rmi);


	void WriteDeclProtocolList(FILE *fp, sProtocol *pProtocol, bool isVirtual, bool isImpl, bool isTarget);
	void WriteFirstArg(FILE *fp, sArg*p, bool isTarget);
	void WriteArg(FILE *fp, sArg *arg, bool comma);
	void WriteArgVar(FILE *fp, sArg *arg, bool comma);
	void WriteProtocolDispatchFunc(FILE *fp, sRmi *rmi);
	void WriteDispatchSwitchCase(FILE *fp, sProtocol *pProtocol, int packetId);
	void WriteDispatchImpleArg(FILE *fp, sArg*p);
	void WriteLastDispatchSwitchCase(FILE *fp, sProtocol *pProtocol);


	// Write Protocol Code
	bool WriteFirstProtocolClassHeader(sRmi *rmi);
	bool WriteProtocolClassHeader(FILE *fp, sRmi *rmi);
	bool WriteFirstProtocolCpp(sRmi *rmi, bool IsAddStdafxHeader);
	bool WriteProtocolCpp(FILE *fp, sRmi *rmi);

	void WriteImplProtocolList(FILE *fp, sProtocol *pProtocol, int packetId);
	void WriteFirstImpleProtocol(FILE *fp, sArg*p, int packetId);
	void WriteImpleArg(FILE *fp, sArg*p);
	void WriteLastImpleProtocol(FILE *fp);

	string GetFileNameExceptExt(const string &fileName);
	string GetProtocolName(const string &fileName);
	string GetProtocolClassName(const string &protocolName, const string &rmiName );
	string GetProtocolListenerClassName(const string &protocolName, const string &rmiName );
	string GetProtocolDispatcherClassName(const string &protocolName, const string &rmiName );



}

using namespace compiler;


//------------------------------------------------------------------------
// ProtocolListener 클래스 헤더파일을 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteProtocolCode(string protocolFileName, sRmi *rmi)
{
	if (!rmi)
		return true;

	n_OrigianlFileName = GetFileNameExceptExt(protocolFileName);
	n_protocolName = GetProtocolName(protocolFileName);

	WriteFirstProtocolClassHeader(rmi);
	WriteFirstProtocolCpp(rmi, false);

	WriteFirstListenerHeader(rmi);
	WriteFirstListenerCpp(rmi, false);

	return true;
}


//------------------------------------------------------------------------
// 인자로 넘어온 fileName의 확장자를 제외한 스트링을 리턴한다.
//------------------------------------------------------------------------
string compiler::GetFileNameExceptExt(const string &fileName)
{
	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
	char *tmp;
	char *name = strtok_s(srcFileName, ".", &tmp);
	return name;
}


//------------------------------------------------------------------------
// 파일이름의 경로와 확장자를 제외한 파일 이름
//------------------------------------------------------------------------
string compiler::GetProtocolName(const string &fileName)
{
	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
	char *name = PathFindFileNameA(srcFileName);
	PathRemoveExtensionA(name);
	return name;
}


//------------------------------------------------------------------------
// *.prt 파일이름(protocolName)과 프로토콜이름(rmiName) 의 조합으로 
// 프로토콜 클래스 이름을 리턴한다.
//------------------------------------------------------------------------
string compiler::GetProtocolClassName(const string &protocolName, const string &rmiName )
{
	return rmiName + "_Protocol";
}


//------------------------------------------------------------------------
// *.prt 파일이름(protocolName)과 프로토콜이름(rmiName) 의 조합으로 
// 프로토콜리스너 클래스 이름을 리턴한다.
//------------------------------------------------------------------------
string compiler::GetProtocolListenerClassName(const string &protocolName, const string &rmiName )
{
	return rmiName + "_ProtocolListener";
}


//------------------------------------------------------------------------
// 프로토콜 디스패쳐 클래스 이름을 리턴한다.
//------------------------------------------------------------------------
string compiler::GetProtocolDispatcherClassName(const string &protocolName, const string &rmiName )
{
	return rmiName + "_Dispatcher";
}


//------------------------------------------------------------------------
// Protocol Header 파일을 생성하는 처음부분의 코드를 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteFirstProtocolClassHeader(sRmi *rmi)
{
	n_fileName = n_OrigianlFileName + "_Protocol.h";

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
	fprintf( fp, "namespace %s {\n", n_protocolName.c_str() );

	WriteProtocolClassHeader(fp, rmi);

	fprintf( fp, "}\n" );
	fclose(fp);
	return true;
}


//------------------------------------------------------------------------
// Protocol 헤더 소스코드를 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteProtocolClassHeader(FILE *fp, sRmi *rmi)
{
	if (!rmi) return true;

	n_className = GetProtocolClassName(n_protocolName, rmi->name );
	n_protocolId = n_className + "_ID";

	fprintf( fp, "static const int %s = %d;\n", n_protocolId.c_str(), rmi->number );
	fprintf( fp, "\n" );
	fprintf( fp, "class %s : public network::IProtocol\n", n_className.c_str() );
	fprintf( fp, "{\n" );
	fprintf( fp, "public:\n" );
	fprintf( fp, "\t%s() : IProtocol(%s) {}\n", n_className.c_str(), n_protocolId.c_str() );
	// print protocol list
	WriteDeclProtocolList( fp, rmi->protocol, false, false, true);
	fprintf( fp, "};\n" );

	return WriteProtocolClassHeader(fp, rmi->next);
}


//------------------------------------------------------------------------
// ProtocolListener 헤더파일에서 처음 들어갈 주석 코드 추가
//------------------------------------------------------------------------
bool compiler::WriteFirstListenerHeader(sRmi *rmi)
{
	n_fileName = n_OrigianlFileName + "_ProtocolListener.h";

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
	fprintf( fp, "namespace %s {\n", n_protocolName.c_str() );

	WriteListenerHeader(fp, rmi);

	fprintf( fp, "}\n");
	fclose(fp);
	return true;
}


//------------------------------------------------------------------------
// ProtocolListener 헤더 클래스 소스파일을 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteListenerHeader(FILE *fp, sRmi *rmi)
{
	if (!rmi) return true;

	n_className = GetProtocolDispatcherClassName(n_protocolName, rmi->name);
	n_protocolId = n_className + "_ID";

	fprintf( fp, "static const int %s = %d;\n", n_protocolId.c_str(), rmi->number );
	fprintf( fp, "\n" );

	// Protocol Dispatcher
	fprintf( fp, "// Protocol Dispatcher\n" );
	fprintf( fp, "class %s : public network::IProtocolDispatcher\n", n_className.c_str() );
	fprintf( fp, "{\n" );
	fprintf( fp, "public:\n" );
	fprintf( fp, "\t%s();\n", n_className.c_str() );
	fprintf( fp, "protected:\n");
	fprintf( fp, "\tvirtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;\n" );
	fprintf( fp, "};\n" );
	fprintf( fp, "\n" );
	fprintf( fp, "\n" );

	// CProtocolListener class
	n_className = GetProtocolListenerClassName(n_protocolName, rmi->name);
	string dispatcherClassName = GetProtocolDispatcherClassName(n_protocolName, rmi->name);

	fprintf( fp, "// ProtocolListener\n" );
	fprintf( fp, "class %s : virtual public network::IProtocolListener\n", n_className.c_str() );
	fprintf( fp, "{\n" );
//	fprintf( fp, "public:\n" );
// 	fprintf( fp, "\t%s() : IProtocolListener(%s) {}\n", n_className.c_str(), n_protocolId.c_str() );
// 	fprintf( fp, "protected:\n");
// 	fprintf( fp, "\tvirtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;\n" );
	fprintf( fp, "\tfriend class %s;\n", dispatcherClassName.c_str() );
	WriteDeclProtocolList( fp, rmi->protocol, true, true, false);
	fprintf( fp, "};\n" );
	fprintf( fp, "\n" );
	fprintf( fp, "\n" );

	return WriteListenerHeader(fp, rmi->next);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool compiler::WriteFirstListenerCpp(sRmi *rmi, bool IsAddStdafxHeader)
{
	n_fileName = n_OrigianlFileName + "_ProtocolListener.cpp";
	string headerFileName = n_protocolName + "_ProtocolListener.h";

	FILE *fp;
	fopen_s(&fp, n_fileName.c_str(), "w");
	if (!fp) return false;

	if (IsAddStdafxHeader)
		fprintf( fp, "#include \"stdafx.h\"\n");
	fprintf( fp, "#include \"%s\"\n", headerFileName.c_str());
	fprintf( fp, "#include \"Network/Controller/NetController.h\"\n" );
	fprintf( fp, "#include \"network/PrtCompiler/ProtocolMacro.h\"\n");
	fprintf( fp, "\n");
	fprintf( fp, "using namespace network;\n");
	fprintf( fp, "using namespace %s;\n", n_protocolName.c_str());
	fprintf( fp, "\n");

	WriteListenerCpp(fp, rmi);

	fclose(fp);
	return true;
}


//------------------------------------------------------------------------
// ProtocolListener Cpp 파일을 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteListenerCpp(FILE *fp, sRmi *rmi)
{
	if (!rmi) return true;

	n_className = GetProtocolDispatcherClassName(n_protocolName, rmi->name);
	n_protocolId = n_className + "_ID";

	// Dispatcher 생성자 코드 생성
	fprintf( fp, "static %s::%s g_%s_%s;\n", 
		n_protocolName.c_str(), n_className.c_str(), n_protocolName.c_str(), n_className.c_str() ); // 전역변수 선언
	fprintf( fp, "\n" );
	fprintf( fp, "%s::%s::%s()\n", n_protocolName.c_str(), n_className.c_str(), n_className.c_str() );
	fprintf( fp, "\t: IProtocolDispatcher(%s::%s)\n", n_protocolName.c_str(), n_protocolId.c_str() );
	fprintf( fp, "{\n" );
	fprintf( fp, "\tCNetController::Get()->AddDispatcher(this);\n" );
	fprintf( fp, "}\n" );
	fprintf( fp, "\n" );
	//

	// Diaptcher 클래스의 Dispatch() 함수 코드 생성
	WriteProtocolDispatchFunc(fp, rmi);
	fprintf( fp, "\n" );
	fprintf( fp, "\n" );
	fprintf( fp, "\n" );

	return WriteListenerCpp(fp, rmi->next);
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
	fprintf( fp, "void %s::%s::%s(", n_protocolName.c_str(), n_className.c_str(), pProtocol->name.c_str() );
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
// Protocol Cpp 코드를 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteFirstProtocolCpp(sRmi *rmi, bool IsAddStdafxHeader)
{
	n_fileName = n_OrigianlFileName + "_Protocol.cpp";
	string headerFileName = n_protocolName + "_Protocol.h";

	FILE *fp;
	fopen_s(&fp, n_fileName.c_str(), "w");
	if (!fp) return false;

	if (IsAddStdafxHeader)
		fprintf( fp, "#include \"stdafx.h\"\n");
	fprintf( fp, "#include \"%s\"", headerFileName.c_str());
	fprintf( fp, "\n");
	fprintf( fp, "using namespace network;\n");
	fprintf( fp, "using namespace %s;\n", n_protocolName.c_str());
	fprintf( fp, "\n");

	WriteProtocolCpp(fp, rmi);

	fclose(fp);
	return true;
}


//------------------------------------------------------------------------
// ProtocolListener Cpp 파일 생성
//------------------------------------------------------------------------
bool compiler::WriteProtocolCpp(FILE *fp, sRmi *rmi)
{
	if (!rmi) return true;
	
	n_className = GetProtocolClassName(n_protocolName, rmi->name);
	WriteImplProtocolList( fp, rmi->protocol, rmi->number+1 );
	fprintf( fp, "\n" );
	fprintf( fp, "\n" );

	return WriteProtocolCpp(fp, rmi->next);
}


//------------------------------------------------------------------------
// Listener::Dispatch() 함수 코드 생성
//------------------------------------------------------------------------
void compiler::WriteProtocolDispatchFunc(FILE *fp, sRmi *rmi)
{
	n_listenerClassName = GetProtocolListenerClassName(n_protocolName, rmi->name);

	fprintf( fp, "//------------------------------------------------------------------------\n");
	fprintf( fp, "// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.\n" );
	fprintf( fp, "//------------------------------------------------------------------------\n");
	fprintf( fp, "void %s::%s::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)\n", 
		n_protocolName.c_str(), n_className.c_str() );
	fprintf( fp, "{\n" );
		fprintf( fp, "\tint protocolId, packetId;\n");
		fprintf( fp, "\tpacket >> protocolId >> packetId;\n");

	if (rmi->protocol)
	{
			fprintf( fp, "\tswitch (packetId)\n");
			fprintf( fp, "\t{\n");

			WriteDispatchSwitchCase(fp, rmi->protocol, rmi->number+1);

			fprintf( fp, "\tdefault:\n");
				fprintf( fp, "\t\tassert(0);\n");
				fprintf( fp, "\t\tbreak;\n");
			fprintf( fp, "\t}\n");
	}
	fprintf( fp, "}\n" );	
}


//------------------------------------------------------------------------
// Dispatch 함수에서case 문 코드 생성
//------------------------------------------------------------------------
void compiler::WriteDispatchSwitchCase(FILE *fp, sProtocol *pProtocol, int packetId)
{
	if (!pProtocol) return;

	fprintf( fp, "\tcase %d:\n", packetId );
	fprintf( fp, "\t\t{\n" );

	WriteDispatchImpleArg(fp, pProtocol->argList);
	WriteLastDispatchSwitchCase(fp, pProtocol);

	fprintf( fp, "\t\t}\n" );
	fprintf( fp, "\t\tbreak;\n" );
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
	fprintf( fp, "\t\t\t%s %s;\n", p->var->type.c_str(), p->var->var.c_str() );
	// 패킷에서 데이타 얻음
	fprintf( fp, "\t\t\tpacket >> %s;\n", p->var->var.c_str() );
	WriteDispatchImpleArg(fp, p->next);
}


//------------------------------------------------------------------------
// Dispatch 함수의 switch case 문의 마지막에 들어가는 코드로, 리스너의 
// 해당하는 프로토콜을 호출하는 코드를 생성한다.
//------------------------------------------------------------------------
void compiler::WriteLastDispatchSwitchCase(FILE *fp, sProtocol *pProtocol)
{
	fprintf( fp, "\t\t\tSEND_LISTENER(%s, listeners, %s(packet.GetSenderId()", 
		n_listenerClassName.c_str(), pProtocol->name.c_str() );
	WriteArgVar(fp, pProtocol->argList, true );
	fprintf( fp, ") );\n" );
}
