//------------------------------------------------------------------------
// Name:    Error.h
// Author:  jjuiddong
// Date:    12/23/2012
// 
// 네트워크에서 발생하는 에러를 출력하거나, 로그로 남긴다.
//------------------------------------------------------------------------
#pragma once

namespace network { namespace error 
{
	void ErrorMsg(const std::string &str);
 	void ErrorLog(const std::string &str);
 	void Msg(const std::string &str);

}}
