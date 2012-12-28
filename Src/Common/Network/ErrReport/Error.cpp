
#include "stdafx.h"
#include "error.h"


using namespace network;
using namespace error;


//------------------------------------------------------------------------
// ���� �޼���â ���
//------------------------------------------------------------------------
void error::ErrorMsg(const std::string &str)
{
	::MessageBoxA(NULL, str.c_str(), "Error", MB_OK );
}


//------------------------------------------------------------------------
// ErrorLog.log ���Ͽ� �α׸� �����.
// ��¥/�ð� + �����޼���
//------------------------------------------------------------------------
void error::ErrorLog(const std::string &str)
{
	FILE *fp;
	fopen_s(&fp, "ErrorLog.log", "a+");
	if (!fp) return;

	std::string msg = "[";
	msg += common::GetTimeString();
	msg += "]	";
	msg += str;
	fprintf( fp, "%s\n", msg.c_str() );
	fclose(fp);	
}


//------------------------------------------------------------------------
// Log.log ���Ͽ� �α׸� �����.
// ��¥/�ð� + �޼���
//------------------------------------------------------------------------
void error::Log(const std::string &str)
{
	FILE *fp;
	fopen_s(&fp, "Log.log", "a+");
	if (!fp) return;

	std::string msg = "[";
	msg += common::GetTimeString();
	msg += "]	";
	msg += str;
	fprintf( fp, "%s\n", msg.c_str() );
	fclose(fp);	
}


//------------------------------------------------------------------------
// �޼���â�� ����Ѵ�.
//------------------------------------------------------------------------
void error::Msg(const std::string &str)
{
	::MessageBoxA(NULL, str.c_str(), "Msg", MB_OK );
}

