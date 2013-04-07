
#include "stdafx.h"
#include "GenerateProtocolCode.h"
#include <direct.h>

using namespace network;
using namespace std;

namespace compiler
{
	// Write Protocol Listener Code
	bool WriteFirstListenerHeader(sRmi *rmi);
	bool WriteListenerHeader(ofstream &fs, sRmi *rmi);
	bool WriteFirstListenerCpp(sRmi *rmi, bool IsAddStdafxHeader);
	bool WriteListenerCpp(ofstream &fs, sRmi *rmi);


	void WriteDeclProtocolList(ofstream &fs, sProtocol *pProtocol, bool isVirtual, bool isImpl, bool isTarget);
	void WriteFirstArg(ofstream &fs, sArg*p, bool isTarget);
	void WriteArg(ofstream &fs, sArg *arg, bool comma);
	void WriteArgVar(ofstream &fs, sArg *arg, bool comma);
	void WriteProtocolDispatchFunc(ofstream &fs, sRmi *rmi);
	void WriteDispatchSwitchCase(ofstream &fs, sProtocol *pProtocol, int packetId);
	void WriteDispatchImpleArg(ofstream &fs, sArg*p);
	void WriteLastDispatchSwitchCase(ofstream &fs, sProtocol *pProtocol);


	// Write Protocol Code
	bool WriteFirstProtocolClassHeader(sRmi *rmi);
	bool WriteProtocolClassHeader(ofstream &fs, sRmi *rmi);
	bool WriteFirstProtocolCpp(sRmi *rmi, bool IsAddStdafxHeader);
	bool WriteProtocolCpp(ofstream &fs, sRmi *rmi);

	void WriteImplProtocolList(ofstream &fs, sProtocol *pProtocol, int packetId);
	void WriteFirstImpleProtocol(ofstream &fs, sArg*p, int packetId);
	void WriteImpleArg(ofstream &fs, sArg*p);
	void WriteLastImpleProtocol(ofstream &fs);

	string GetFileNameExceptExt(const string &fileName);
	string GetProtocolName(const string &fileName);
	string GetProtocolClassName(const string &protocolName, const string &rmiName );
	string GetProtocolListenerClassName(const string &protocolName, const string &rmiName );
	string GetProtocolDispatcherClassName(const string &protocolName, const string &rmiName );


	string g_className; // Protocol, ProtocolListener 공동으로 사용하고 있다.
	string g_protocolId;
	string n_fileName;
	string g_protocolName;	// *.prt 파일의 확장자와 경로를 제외한 파일이름을 저장한다.
	string n_OrigianlFileName;
	string g_listenerClassName;
	string n_SrcFolderName = "Src";
}

using namespace compiler;


//------------------------------------------------------------------------
// ProtocolListener 클래스 헤더파일을 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteProtocolCode(string protocolFileName, sRmi *rmi)
{
	if (!rmi)
		return true;

	const std::string fileName = common::GetFileNameExceptExt(protocolFileName);
	const std::string path = common::GetFilePathExceptFileName(protocolFileName);
	const std::string folder = (path.empty())? n_SrcFolderName : path + "\\" + n_SrcFolderName;
	_mkdir(folder.c_str());
	n_OrigianlFileName = folder+ "\\" + fileName;

	g_protocolName = GetProtocolName(protocolFileName);

	WriteFirstProtocolClassHeader(rmi);
	WriteFirstProtocolCpp(rmi, false);

	WriteFirstListenerHeader(rmi);
	WriteFirstListenerCpp(rmi, false);

	return true;
}


////------------------------------------------------------------------------
//// 인자로 넘어온 fileName의 확장자를 제외한 스트링을 리턴한다.
////------------------------------------------------------------------------
//string compiler::GetFileNameExceptExt(const string &fileName)
//{
//	char srcFileName[ MAX_PATH];
//	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
//	char *tmp;
//	char *name = strtok_s(srcFileName, ".", &tmp);
//	return name;
//}


//------------------------------------------------------------------------
// 파일이름의 경로와 확장자를 제외한 파일 이름
//------------------------------------------------------------------------
string compiler::GetProtocolName(const string &fileName)
{
	return common::GetFileNameExceptExt(fileName);
// 	char srcFileName[ MAX_PATH];
// 	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
// 	char *name = PathFindFileNameA(srcFileName);
// 	PathRemoveExtensionA(name);
// 	return name;
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

	std::ofstream fs;
	fs.open(n_fileName.c_str());
	if (!fs.is_open()) return false;

	fs << "//------------------------------------------------------------------------\n";
	fs << "// Name:    " << n_fileName << endl;
	fs << "// Author:  ProtocolCompiler (by jjuiddong)\n";
	fs << "// Date:    \n";
	fs << "//------------------------------------------------------------------------\n";

	fs << "#pragma once\n";
	fs << endl;
	fs << "namespace " << g_protocolName << " {\n";
	fs << endl;
	fs << "using namespace network;\n";
	fs << "using namespace marshalling;\n";

	WriteProtocolClassHeader(fs, rmi);

	fs << "}\n";
	return true;
}


//------------------------------------------------------------------------
// Protocol 헤더 소스코드를 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteProtocolClassHeader(ofstream &fs, sRmi *rmi)
{
	if (!rmi) return true;

	g_className = GetProtocolClassName(g_protocolName, rmi->name );
	g_protocolId = g_className + "_ID";

	fs << "static const int " << g_protocolId << "= " << rmi->number << ";\n";
	fs << endl;
	fs << "class " << g_className <<  " : public network::IProtocol\n";
	fs << "{" << endl;
	fs << "public:" << endl;
	fs << "\t" << g_className << "() : IProtocol(" << g_protocolId << ") {}\n";
	// print protocol list
	WriteDeclProtocolList( fs, rmi->protocol, false, false, true);
	fs << "};\n";

	return WriteProtocolClassHeader(fs, rmi->next);
}


//------------------------------------------------------------------------
// ProtocolListener 헤더파일에서 처음 들어갈 주석 코드 추가
//------------------------------------------------------------------------
bool compiler::WriteFirstListenerHeader(sRmi *rmi)
{
	n_fileName = n_OrigianlFileName + "_ProtocolListener.h";

	ofstream fs;
	fs.open( n_fileName.c_str());
	if (!fs.is_open()) return false;

	fs << "//------------------------------------------------------------------------\n";
	fs << "// Name:    " << n_fileName << endl;
	fs << "// Author:  ProtocolCompiler (by jjuiddong)\n";
	fs << "// Date:    \n";
	fs << "//------------------------------------------------------------------------\n";

	fs << "#pragma once\n";
	fs << endl;
	fs << "namespace " << g_protocolName << " {\n";
	fs << endl;
	fs << "using namespace network;\n";
	fs << "using namespace marshalling;\n";

	WriteListenerHeader(fs, rmi);

	fs << "}\n";
	return true;
}


//------------------------------------------------------------------------
// ProtocolListener 헤더 클래스 소스파일을 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteListenerHeader(ofstream &fs, sRmi *rmi)
{
	if (!rmi) return true;

	g_className = GetProtocolDispatcherClassName(g_protocolName, rmi->name);
	g_protocolId = g_className + "_ID";

	fs << "static const int " << g_protocolId << " = " << rmi->number << ";\n";
	fs << endl;

	// Protocol Dispatcher
	fs << "// Protocol Dispatcher\n";
	fs << "class " << g_className << ": public network::IProtocolDispatcher\n";
	fs << "{\n";
	fs << "public:\n";
	fs << "\t" << g_className << "();\n";
	fs << "protected:\n";
	fs << "\tvirtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;\n";
	fs << "};\n";
	fs << endl;
	fs << endl;

	// CProtocolListener class
	g_className = GetProtocolListenerClassName(g_protocolName, rmi->name);
	string dispatcherClassName = GetProtocolDispatcherClassName(g_protocolName, rmi->name);

	fs << "// ProtocolListener\n";
	fs << "class " << g_className << " : virtual public network::IProtocolListener\n";
	fs << "{\n";
//	fs << "public:\n" );
// 	pProtocol->name"\t%s() : IProtocolListener(%s) {}\n", n_className.c_str(), n_protocolId.c_str() );
// 	pProtocol->name"protected:\n");
// 	pProtocol->name"\tvirtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;\n" );
	fs << "\tfriend class " << dispatcherClassName << ";\n";
	WriteDeclProtocolList( fs, rmi->protocol, true, true, false);
	fs << "};\n";
	fs << endl;
	fs << endl;

	return WriteListenerHeader(fs, rmi->next);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool compiler::WriteFirstListenerCpp(sRmi *rmi, bool IsAddStdafxHeader)
{
	n_fileName = n_OrigianlFileName + "_ProtocolListener.cpp";
	string headerFileName = g_protocolName + "_ProtocolListener.h";

	ofstream fs(n_fileName.c_str());
	if (!fs.is_open())
		return false;

	if (IsAddStdafxHeader)
		fs << "#include \"stdafx.h\"\n";
	fs << "#include \"" << headerFileName << "\"\n";
	fs << "#include \"Network/Controller/NetController.h\"\n";
	fs << endl;
	//fs << "using namespace network;\n";
	//fs << "using namespace marshalling;\n";
	fs << "using namespace " << g_protocolName << ";\n";
	fs << endl;

	WriteListenerCpp(fs, rmi);
	return true;
}


//------------------------------------------------------------------------
// ProtocolListener Cpp 파일을 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteListenerCpp(ofstream &fs, sRmi *rmi)
{
	if (!rmi) return true;

	g_className = GetProtocolDispatcherClassName(g_protocolName, rmi->name);
	g_protocolId = g_className + "_ID";

	// Dispatcher 생성자 코드 생성
	fs << "static " << g_protocolName << "::" << g_className << " g_" << g_protocolName << "_" << g_className << ";\n";// 전역변수 선언
	fs << "\n";
	fs << g_protocolName << "::" << g_className << "::" << g_className << "()\n";
	fs << "\t: IProtocolDispatcher(" << g_protocolName << "::" << g_protocolId << ")\n";
	fs << "{\n";
	fs << "\tCNetController::Get()->AddDispatcher(this);\n";
	fs << "}\n";
	fs << endl;
	//

	// Diaptcher 클래스의 Dispatch() 함수 코드 생성
	WriteProtocolDispatchFunc(fs, rmi);
	fs << endl;
	fs << endl;
	fs << endl;

	return WriteListenerCpp(fs, rmi->next);
}


//------------------------------------------------------------------------
// 헤더 클래스파일에 있을 프로토콜 리스트를 출력한다.
//------------------------------------------------------------------------
void compiler::WriteDeclProtocolList(ofstream &fs, sProtocol *pProtocol, bool isVirtual, bool isImpl, bool isTarget)
{
	if (!pProtocol) return;
	fs << "\t";
	if (isVirtual)
		fs << "virtual ";
	fs << "void " << pProtocol->name << "(";
	WriteFirstArg(fs, pProtocol->argList, isTarget);
	fs << ")";
	if (isImpl)
		fs << "{}";
	else
		fs << ";";
	fs << endl;
	WriteDeclProtocolList(fs, pProtocol->next, isVirtual, isImpl, isTarget);
}


//------------------------------------------------------------------------
// Cpp 파일에 있을 프로토콜 리스트를 출력한다.
//------------------------------------------------------------------------
void compiler::WriteImplProtocolList(ofstream &fs, sProtocol *pProtocol, int packetId)
{
	if (!pProtocol) return;
	fs << "//------------------------------------------------------------------------\n";
	fs << "// Protocol: " << pProtocol->name << endl;
	fs << "//------------------------------------------------------------------------\n";
	fs << "void " << g_protocolName << "::" << g_className << "::" << pProtocol->name << "(";
	WriteFirstArg(fs, pProtocol->argList, true);
	fs << ")\n";
	fs << "{\n";
	WriteFirstImpleProtocol(fs, pProtocol->argList, packetId);
	fs << "}\n";
	fs << "\n";
	WriteImplProtocolList(fs, pProtocol->next, packetId+1);
}


//------------------------------------------------------------------------
// 프로토콜 인자리스트 출력
//------------------------------------------------------------------------
void compiler::WriteFirstArg(ofstream &fs, sArg*p, bool isTarget)
{
	if (isTarget)
	{
		fs <<"netid targetId";
		fs << ", const SEND_FLAG flag";
	}
	else
	{
		fs << "netid senderId";
	}

	WriteArg(fs, p, true);
}
void compiler::WriteArg(ofstream &fs, sArg *arg, bool comma)
{
	if (!arg) return;
	if (comma)
		fs << ", ";
	fs << "const " << arg->var->type << " &" << arg->var->var;
	WriteArg(fs, arg->next, true);
}

//------------------------------------------------------------------------
// 프로토콜 인자값의 변수 이름만 출력한다. (함수 호출시 사용)
//------------------------------------------------------------------------
void compiler::WriteArgVar(ofstream &fs, sArg *arg, bool comma)
{
	if (!arg) return;
	if (comma)
		fs << ", ";
	fs << arg->var->var;
	WriteArgVar(fs, arg->next, true);
}


//------------------------------------------------------------------------
// Cpp 파일에 들어갈 프로토콜 함수의 처음에 나올 코드
//------------------------------------------------------------------------
void compiler::WriteFirstImpleProtocol(ofstream &fs, sArg*p, int packetId)
{
	fs << "\tCPacket packet;\n";
	fs << "\tpacket.SetProtocolId( GetId() );\n";
	fs << "\tpacket.SetPacketId( " << packetId << " );\n";
	WriteImpleArg(fs, p);
	fs << "\tpacket.EndPack();\n";
	WriteLastImpleProtocol(fs);
}


//------------------------------------------------------------------------
// Cpp 파일에 들어갈 프로토콜 코드에서 패킷에 인자값을 넣는 코드
//------------------------------------------------------------------------
void compiler::WriteImpleArg(ofstream &fs, sArg*p)
{
	if (!p) return;
	fs << "\tpacket << " << p->var->var << ";\n";
	WriteImpleArg(fs, p->next);
}


//------------------------------------------------------------------------
// Cpp 파일에 들어갈 프로토콜 함수의 마지막에 나올 코드
//------------------------------------------------------------------------
void compiler::WriteLastImpleProtocol(ofstream &fs)
{
	fs << "\tGetNetConnector()->Send(targetId, flag, packet);\n";
}


//------------------------------------------------------------------------
// Protocol Cpp 코드를 생성한다.
//------------------------------------------------------------------------
bool compiler::WriteFirstProtocolCpp(sRmi *rmi, bool IsAddStdafxHeader)
{
	n_fileName = n_OrigianlFileName + "_Protocol.cpp";
	string headerFileName = g_protocolName + "_Protocol.h";

	ofstream fs(n_fileName.c_str());
	if (!fs.is_open()) return false;

	if (IsAddStdafxHeader)
		fs << "#include \"stdafx.h\"\n";
	fs << "#include \"" << headerFileName << "\"\n";
	//fs << "using namespace network;\n";
	//fs << "using namespace marshalling;\n";
	fs << "using namespace " << g_protocolName << ";\n";
	fs << endl;

	WriteProtocolCpp(fs, rmi);

	return true;
}


//------------------------------------------------------------------------
// ProtocolListener Cpp 파일 생성
//------------------------------------------------------------------------
bool compiler::WriteProtocolCpp(ofstream &fs, sRmi *rmi)
{
	if (!rmi) return true;
	
	g_className = GetProtocolClassName(g_protocolName, rmi->name);
	WriteImplProtocolList( fs, rmi->protocol, rmi->number+1 );
	fs << endl;
	fs << endl;

	return WriteProtocolCpp(fs, rmi->next);
}


//------------------------------------------------------------------------
// Listener::Dispatch() 함수 코드 생성
//------------------------------------------------------------------------
void compiler::WriteProtocolDispatchFunc(ofstream &fs, sRmi *rmi)
{
	g_listenerClassName = GetProtocolListenerClassName(g_protocolName, rmi->name);

	fs << "//------------------------------------------------------------------------\n";
	fs << "// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.\n";
	fs << "//------------------------------------------------------------------------\n";
	fs << "void " << g_protocolName << "::" << g_className << "::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)\n", 
	fs << "{\n";
		fs << "\tconst int protocolId = packet.GetProtocolId();\n";
		fs << "\tconst int packetId = packet.GetPacketId();\n";

	if (rmi->protocol)
	{
			fs << "\tswitch (packetId)\n";
			fs << "\t{\n";

			WriteDispatchSwitchCase(fs, rmi->protocol, rmi->number+1);

			fs << "\tdefault:\n";
				fs << "\t\tassert(0);\n";
				fs << "\t\tbreak;\n";
			fs << "\t}\n";
	}
	fs << "}\n";	
}


//------------------------------------------------------------------------
// Dispatch 함수에서case 문 코드 생성
//------------------------------------------------------------------------
void compiler::WriteDispatchSwitchCase(ofstream &fs, sProtocol *pProtocol, int packetId)
{
	if (!pProtocol) return;

	fs << "\tcase " << packetId << ":\n";
	fs << "\t\t{\n";

	// call ListenerMatching<T> function 
	fs << "\t\t\tProtocolListenerList recvListener;\n";
	fs << "\t\t\tif (!ListenerMatching<" << g_listenerClassName << ">(listeners, recvListener))\n";
	fs << "\t\t\t\tbreak;\n";
	fs << endl;

	WriteDispatchImpleArg(fs, pProtocol->argList);
	WriteLastDispatchSwitchCase(fs, pProtocol);

	fs << "\t\t}\n";
	fs << "\t\tbreak;\n";
	fs << "\n";

	WriteDispatchSwitchCase(fs, pProtocol->next, packetId+1);
}


//------------------------------------------------------------------------
// Dispatch 함수의 switch case 문에 들어가는 각 패킷 인자를 얻어오는 코드를
// 생성한다.
//------------------------------------------------------------------------
void compiler::WriteDispatchImpleArg(ofstream &fs, sArg*p)
{
	if (!p) return;
	// 변수 선언
	fs << "\t\t\t" << p->var->type << " " << p->var->var << ";\n";
	// 패킷에서 데이타 얻음
	fs << "\t\t\tpacket >> " << p->var->var << ";\n";
	WriteDispatchImpleArg(fs, p->next);
}


//------------------------------------------------------------------------
// Dispatch 함수의 switch case 문의 마지막에 들어가는 코드로, 리스너의 
// 해당하는 프로토콜을 호출하는 코드를 생성한다.
//------------------------------------------------------------------------
void compiler::WriteLastDispatchSwitchCase(ofstream &fs, sProtocol *pProtocol)
{
	fs << "\t\t\tSEND_LISTENER(" << g_listenerClassName << ", recvListener, " << pProtocol->name << "(packet.GetSenderId()";
	WriteArgVar(fs, pProtocol->argList, true );
	fs << ") );\n";
}
