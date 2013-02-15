
#include "stdafx.h"
#include "ConfigParser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace config
{
	using namespace boost;
	using namespace boost::property_tree;
	using boost::property_tree::ptree;

	ptree n_Props;
	ptree *n_pProtocolTree = NULL;
}



//------------------------------------------------------------------------
// config 파일을 읽는다.
//------------------------------------------------------------------------
bool	 config::Init( const std::string &configFileName )
{
	try
	{
		boost::property_tree::read_json(configFileName.c_str(), n_Props);
		n_pProtocolTree = &n_Props.get_child("protocol");
	}
	catch (std::exception &e)
	{
		CString msg = common::formatw("\"%s\" json script Err!! [%s]",  
			configFileName.c_str(), e.what()).c_str();
//		::AfxMessageBox( msg );
		return false;
	}

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void config::Release()
{
	
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
config::AttrType config::FindProtocol( const std::string &protocolName )
{
	RETV(!n_pProtocolTree, NULL);
	try
	{
		using std::string;

		ptree &childs = n_pProtocolTree->get_child( protocolName );
		BOOST_FOREACH(ptree::value_type &vt, childs)
		{
			const string name = vt.second.get<string>("name");
			if (name == protocolName )
			{
				ptree &packets = vt.second.get_child("packet");
				return &packets;
			}
		}
	}
	catch (...)
	{

	}
	return NULL;
}
