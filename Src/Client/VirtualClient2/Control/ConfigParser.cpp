
#include "stdafx.h"
#include "ConfigParser.h"
#include "../VirtualClient2.h"
#include "../UI/VirtualClient2Dlg.h"
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
		g_ErrorMsg = common::format("\"%s\" json script Err!! [ %s ]",  
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
		return g_Props.get<std::string>(scope, "");
	}
	catch (std::exception &e)
	{
		std::string msg = e.what(); // debug용
	}
	return "";
}


/**
 @brief find collapse node in config json script 
 */
bool	config::IsCollapseNode( const std::string &scope )
{
	try
	{
		ptree &collapse = g_Props.get_child("collapse");
		if (collapse.size() <= 0)
			return false;

		BOOST_FOREACH(auto &child, collapse)
		{
			if (scope == child.second.data())
				return true;
		}
	}
	catch (...)
	{
	}
	return false;
}


//------------------------------------------------------------------------
// return *.ptr directory name
//------------------------------------------------------------------------
std::string config::GetProtocolDirectory()
{
	return g_ProtocolDir;
}


/**
 @brief Change Main Dialog size from config script window size field
 */
void	config::RepositionWindow()
{
	try
	{
		ptree props;
		boost::property_tree::read_json("virtualclient2_windowsize.json", props);
		const std::string rectStr = props.get<std::string>("window size", "");
		if (!rectStr.empty())
		{
			CRect wr;
			std::stringstream ss(rectStr);
			ss >> wr.left >> wr.top >> wr.right >> wr.bottom;

			int width = GetSystemMetrics(SM_CXSCREEN);
			int height= GetSystemMetrics(SM_CYSCREEN);
			if (wr.right > width || wr.bottom > height)
				wr = CRect(0,0,300,400);
			GetMainDlg()->MoveWindow( wr );
		}
	}
	catch (std::exception &e)
	{
		std::string msg = e.what(); // debug용
	}
}


/**
 @brief save main dialog size
 */
void	config::SaveWindowPosition()
{
	try
	{
		CRect wr;
		GetMainDlg()->GetWindowRect(wr);
		std::stringstream ss;
		ss << wr.left << " " << wr.top << " " << wr.right << " " << wr.bottom;

		ptree props;
		props.add( "window size", ss.str() );
		boost::property_tree::write_json("virtualclient2_windowsize.json", props);
	}
	catch (std::exception &e)
	{
		std::string msg = e.what(); // debug용
	}
}
