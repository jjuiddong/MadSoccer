
#include "stdafx.h"
#include "error.h"


using namespace network;
using namespace error;


//------------------------------------------------------------------------
// 에러 메세지창 출력
//------------------------------------------------------------------------
void error::ErrorMsg(const std::string &str)
{
	::MessageBoxA(NULL, str.c_str(), "Error", MB_OK );
}


//------------------------------------------------------------------------
// ErrorLog.txt 파일에 로그를 남긴다.
// 날짜/시간 + 에러메세지
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
// Log.txt 파일에 로그를 남긴다.
// 날짜/시간 + 메세지
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
// 메세지창을 출력한다.
//------------------------------------------------------------------------
void error::Msg(const std::string &str)
{
	::MessageBoxA(NULL, str.c_str(), "Msg", MB_OK );
}

