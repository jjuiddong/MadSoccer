//------------------------------------------------------------------------
// Name:    ConfigParser.h
// Author:  jjuiddong
// Date:    2/14/2013
// 
// Config.json 스크립트를 읽어서 필요한 정보를 리턴해준다.
//
// ReserveString 설정 샘플
//	"1_connect::c2s::Connect::ip" : "127.0.0.1",
//	"1_connect::c2s::Connect::port" : "2334"
//
//------------------------------------------------------------------------
#ifndef __CONFIGPARSER_H__
#define  __CONFIGPARSER_H__

namespace config
{
	bool				OpenConfigFile();
	std::string	FindReservedString( const std::string &scope );
	std::string	GetErrorMsg();

};

#endif // __CONFIGPARSER_H__
