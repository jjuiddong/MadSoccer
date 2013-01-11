
#include "stdafx.h"
#include <fstream>
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
// ErrorLog.log 파일에 로그를 남긴다.
// 날짜/시간 + 에러메세지
//------------------------------------------------------------------------
void error::ErrorLog(const std::string &str)
{
	std::ofstream fs("ErrorLog.log", std::ios_base::out);
	if (!fs.is_open()) return;

	fs << "[" << common::GetTimeString() << "]	" << str << std::endl;
}


//------------------------------------------------------------------------
// 메세지창을 출력한다.
//------------------------------------------------------------------------
void error::Msg(const std::string &str)
{
	::MessageBoxA(NULL, str.c_str(), "Msg", MB_OK );
}
