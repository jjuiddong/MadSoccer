
#include "stdafx.h"
#include <fstream>
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
	std::ofstream fs("ErrorLog.log", std::ios_base::out);
	if (!fs.is_open()) return;

	fs << "[" << common::GetTimeString() << "]	" << str << std::endl;
}


//------------------------------------------------------------------------
// �޼���â�� ����Ѵ�.
//------------------------------------------------------------------------
void error::Msg(const std::string &str)
{
	::MessageBoxA(NULL, str.c_str(), "Msg", MB_OK );
}
