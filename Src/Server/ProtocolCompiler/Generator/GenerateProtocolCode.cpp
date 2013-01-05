
#include "stdafx.h"
#include "GenerateProtocolCode.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi")

using namespace network;
using namespace std;

namespace compiler
{
	string n_className; // Protocol, ProtocolListener �������� ����ϰ� �ִ�.
	string n_protocolId;
	string n_fileName;
	string n_protocolName;	// *.prt ������ Ȯ���ڿ� ��θ� ������ �����̸��� �����Ѵ�.
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
// ProtocolListener Ŭ���� ��������� �����Ѵ�.
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
// ���ڷ� �Ѿ�� fileName�� Ȯ���ڸ� ������ ��Ʈ���� �����Ѵ�.
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
// �����̸��� ��ο� Ȯ���ڸ� ������ ���� �̸�
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
// *.prt �����̸�(protocolName)�� ���������̸�(rmiName) �� �������� 
// �������� Ŭ���� �̸��� �����Ѵ�.
//------------------------------------------------------------------------
string compiler::GetProtocolClassName(const string &protocolName, const string &rmiName )
{
	return rmiName + "_Protocol";
}


//------------------------------------------------------------------------
// *.prt �����̸�(protocolName)�� ���������̸�(rmiName) �� �������� 
// �������ݸ����� Ŭ���� �̸��� �����Ѵ�.
//------------------------------------------------------------------------
string compiler::GetProtocolListenerClassName(const string &protocolName, const string &rmiName )
{
	return rmiName + "_ProtocolListener";
}


//------------------------------------------------------------------------
// �������� ������ Ŭ���� �̸��� �����Ѵ�.
//------------------------------------------------------------------------
string compiler::GetProtocolDispatcherClassName(const string &protocolName, const string &rmiName )
{
	return rmiName + "_Dispatcher";
}


//------------------------------------------------------------------------
// Protocol Header ������ �����ϴ� ó���κ��� �ڵ带 �����Ѵ�.
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
// Protocol ��� �ҽ��ڵ带 �����Ѵ�.
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
// ProtocolListener ������Ͽ��� ó�� �� �ּ� �ڵ� �߰�
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
// ProtocolListener ��� Ŭ���� �ҽ������� �����Ѵ�.
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
// ProtocolListener Cpp ������ �����Ѵ�.
//------------------------------------------------------------------------
bool compiler::WriteListenerCpp(FILE *fp, sRmi *rmi)
{
	if (!rmi) return true;

	n_className = GetProtocolDispatcherClassName(n_protocolName, rmi->name);
	n_protocolId = n_className + "_ID";

	// Dispatcher ������ �ڵ� ����
	fprintf( fp, "static %s::%s g_%s_%s;\n", 
		n_protocolName.c_str(), n_className.c_str(), n_protocolName.c_str(), n_className.c_str() ); // �������� ����
	fprintf( fp, "\n" );
	fprintf( fp, "%s::%s::%s()\n", n_protocolName.c_str(), n_className.c_str(), n_className.c_str() );
	fprintf( fp, "\t: IProtocolDispatcher(%s::%s)\n", n_protocolName.c_str(), n_protocolId.c_str() );
	fprintf( fp, "{\n" );
	fprintf( fp, "\tCNetController::Get()->AddDispatcher(this);\n" );
	fprintf( fp, "}\n" );
	fprintf( fp, "\n" );
	//

	// Diaptcher Ŭ������ Dispatch() �Լ� �ڵ� ����
	WriteProtocolDispatchFunc(fp, rmi);
	fprintf( fp, "\n" );
	fprintf( fp, "\n" );
	fprintf( fp, "\n" );

	return WriteListenerCpp(fp, rmi->next);
}


//------------------------------------------------------------------------
// ��� Ŭ�������Ͽ� ���� �������� ����Ʈ�� ����Ѵ�.
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
// Cpp ���Ͽ� ���� �������� ����Ʈ�� ����Ѵ�.
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
// �������� ���ڸ���Ʈ ���
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
// �������� ���ڰ��� ���� �̸��� ����Ѵ�. (�Լ� ȣ��� ���)
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
// Cpp ���Ͽ� �� �������� �Լ��� ó���� ���� �ڵ�
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
// Cpp ���Ͽ� �� �������� �ڵ忡�� ��Ŷ�� ���ڰ��� �ִ� �ڵ�
//------------------------------------------------------------------------
void compiler::WriteImpleArg(FILE *fp, sArg*p)
{
	if (!p) return;
	fprintf( fp, "\tpacket << %s;\n", p->var->var.c_str());
	WriteImpleArg(fp, p->next);
}


//------------------------------------------------------------------------
// Cpp ���Ͽ� �� �������� �Լ��� �������� ���� �ڵ�
//------------------------------------------------------------------------
void compiler::WriteLastImpleProtocol(FILE *fp)
{
	fprintf( fp, "\tGetNetConnector()->Send(targetId, packet);\n" );
}


//------------------------------------------------------------------------
// Protocol Cpp �ڵ带 �����Ѵ�.
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
// ProtocolListener Cpp ���� ����
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
// Listener::Dispatch() �Լ� �ڵ� ����
//------------------------------------------------------------------------
void compiler::WriteProtocolDispatchFunc(FILE *fp, sRmi *rmi)
{
	n_listenerClassName = GetProtocolListenerClassName(n_protocolName, rmi->name);

	fprintf( fp, "//------------------------------------------------------------------------\n");
	fprintf( fp, "// ��Ŷ�� �������ݿ� ���� �ش��ϴ� �������� �Լ��� ȣ���Ѵ�.\n" );
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
// Dispatch �Լ�����case �� �ڵ� ����
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
// Dispatch �Լ��� switch case ���� ���� �� ��Ŷ ���ڸ� ������ �ڵ带
// �����Ѵ�.
//------------------------------------------------------------------------
void compiler::WriteDispatchImpleArg(FILE *fp, sArg*p)
{
	if (!p) return;
	// ���� ����
	fprintf( fp, "\t\t\t%s %s;\n", p->var->type.c_str(), p->var->var.c_str() );
	// ��Ŷ���� ����Ÿ ����
	fprintf( fp, "\t\t\tpacket >> %s;\n", p->var->var.c_str() );
	WriteDispatchImpleArg(fp, p->next);
}


//------------------------------------------------------------------------
// Dispatch �Լ��� switch case ���� �������� ���� �ڵ��, �������� 
// �ش��ϴ� ���������� ȣ���ϴ� �ڵ带 �����Ѵ�.
//------------------------------------------------------------------------
void compiler::WriteLastDispatchSwitchCase(FILE *fp, sProtocol *pProtocol)
{
	fprintf( fp, "\t\t\tSEND_LISTENER(%s, listeners, %s(packet.GetSenderId()", 
		n_listenerClassName.c_str(), pProtocol->name.c_str() );
	WriteArgVar(fp, pProtocol->argList, true );
	fprintf( fp, ") );\n" );
}
