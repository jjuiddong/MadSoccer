//------------------------------------------------------------------------
// Name:    ConfigParser.h
// Author:  jjuiddong
// Date:    2/14/2013
// 
// Config.json ��ũ��Ʈ�� �о �ʿ��� ������ �������ش�.
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
