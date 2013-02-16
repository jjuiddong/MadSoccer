
#include "stdafx.h"
#include "ConfigParser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace config
{
	using boost::property_tree::ptree;

	ptree n_Props;
	std::string n_ErrorMsg;
}

std::string config::GetErrorMsg()
{
	return n_ErrorMsg;
}


//------------------------------------------------------------------------
// config ������ �д´�.
//------------------------------------------------------------------------
bool	 config::OpenConfigFile()
{
	std::string configFileName("virtualclient2_config.json");

	try
	{
		boost::property_tree::read_json(configFileName.c_str(), n_Props);
	}
	catch (std::exception &e)
	{
		n_ErrorMsg = common::format("\"%s\" json script Err!! [%s]",  
			configFileName.c_str(), e.what());
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// scope Ű���� ���� ����Ÿ�� �����Ѵ�.
//------------------------------------------------------------------------
std::string config::FindReservedString( const std::string &scope )
{
	try
	{
		return n_Props.get<std::string>(scope);
	}
	catch (std::exception &e)
	{
		std::string msg = e.what(); // debug��
	}
	return "";
}
