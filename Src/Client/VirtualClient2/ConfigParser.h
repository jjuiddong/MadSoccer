//------------------------------------------------------------------------
// Name:    ConfigParser.h
// Author:  jjuiddong
// Date:    2/14/2013
// 
// Config.json 스크립트를 읽어서 필요한 정보를 리턴해준다.
//------------------------------------------------------------------------
#ifndef __CONFIGPARSER_H__
#define  __CONFIGPARSER_H__


namespace config
{
	typedef void* AttrType;

	bool		Init( const std::string &configFileName );
	void		Release();

	AttrType FindProtocol( const std::string &protocolName );
	


};

#endif // __CONFIGPARSER_H__
