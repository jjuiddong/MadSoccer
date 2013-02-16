//------------------------------------------------------------------------
// Name:    ConfigParser.h
// Author:  jjuiddong
// Date:    2/14/2013
// 
// Config.json ��ũ��Ʈ�� �о �ʿ��� ������ �������ش�.
//
// ReserveString ���� ����
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
