//------------------------------------------------------------------------
// Name:    Error.h
// Author:  jjuiddong
// Date:    12/23/2012
// 
// ��Ʈ��ũ���� �߻��ϴ� ������ ����ϰų�, �α׷� �����.
//------------------------------------------------------------------------
#pragma once

namespace network { namespace error 
{
	void ErrorMsg(const std::string &str);
 	void ErrorLog(const std::string &str);
 	void Msg(const std::string &str);

}}
