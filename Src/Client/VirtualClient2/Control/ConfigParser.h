/**
Name:   ConfigParser.h
Author:  jjuiddong
Date:    2/14/2013

 Config.json ��ũ��Ʈ�� �о �ʿ��� ������ �������ش�.

 ReserveString ���� ����
"1_connect::c2s::Connect::ip" : "127.0.0.1",
"1_connect::c2s::Connect::port" : "2334"

*/
#pragma once

namespace config
{
	bool				OpenConfigFile();
	std::string	GetProtocolDirectory();
	std::string	FindReservedString( const std::string &scope );
	bool				IsCollapseNode( const std::string &scope );
	std::string	GetErrorMsg();

	void				RepositionWindow();
	void				SaveWindowPosition();
};
