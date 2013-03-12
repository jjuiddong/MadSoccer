
#include "stdafx.h"
#include "ConfigParser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace config
{
	using boost::property_tree::ptree;

	ptree g_Props;
	std::string g_ProtocolDir;
	std::string g_ErrorMsg;
}

std::string config::GetErrorMsg()
{
	return g_ErrorMsg;
}


//------------------------------------------------------------------------
// config 파일을 읽는다.
//------------------------------------------------------------------------
bool	 config::OpenConfigFile()
{
	std::string configFileName("virtualclient2_config.json");

	try
	{
		boost::property_tree::read_json(configFileName.c_str(), g_Props);
		g_ProtocolDir = g_Props.get<std::string>("protocol directory");
	}
	catch (std::exception &e)
	{
		g_ErrorMsg = common::format("\"%s\" json script Err!! [%s]",  
			configFileName.c_str(), e.what());
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// scope 키값을 가진 데이타를 리턴한다.
//------------------------------------------------------------------------
std::string config::FindReservedString( const std::string &scope )
{
	try
	{
		return g_Props.get<std::string>(scope);
	}
	catch (std::exception &e)
	{
		std::string msg = e.what(); // debug용
	}
	return "";
}


//------------------------------------------------------------------------
// return *.ptr directory name
//------------------------------------------------------------------------
std::string config::GetProtocolDirectory()
{
	return g_ProtocolDir;
}
