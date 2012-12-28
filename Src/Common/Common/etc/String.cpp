
#include "stdafx.h"
#include "String.h"


using namespace common;



//------------------------------------------------------------------------
// 유니코드를 멀티바이트 문자로 변환
//------------------------------------------------------------------------
std::string common::wstring2string(const std::wstring &wstr)
{
	std::locale const& loc = std::locale();
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
	std::mbstate_t state = 0;
	std::vector<char> buf((wstr.size() + 1) * codecvt.max_length());
	wchar_t const* in_next = wstr.c_str();
	char* out_next = &buf[0];
	codecvt_t::result r = codecvt.out(state, 
		wstr.c_str(), wstr.c_str() + wstr.size(), in_next, 
		&buf[0], &buf[0] + buf.size(), out_next);
	return std::string(&buf[0]);
}


//------------------------------------------------------------------------
// 멀티바이트 문자를 유니코드로 변환
//------------------------------------------------------------------------
std::wstring common::string2wstring(const std::string &str)
{
	std::locale const& loc = std::locale();
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
	std::mbstate_t state = 0;
	std::vector<wchar_t> buf(str.size() + 1);
	char const* in_next = str.c_str();
	wchar_t* out_next = &buf[0];
	codecvt_t::result r = codecvt.in(state, 
		str.c_str(), str.c_str() + str.size(), in_next, 
		&buf[0], &buf[0] + buf.size(), out_next);
	return std::wstring(&buf[0]);
}


//------------------------------------------------------------------------
// 스트링포맷
//------------------------------------------------------------------------
std::string common::format(const char* fmt, ...)
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );
	return textString;
}
