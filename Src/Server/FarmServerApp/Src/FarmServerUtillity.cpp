
#include "stdafx.h"
#include "FarmServerUtillity.h"
#include "FarmServer.h"
#include "RemoteSubServer.h"
#include "SubServerGroup.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace farmsvr;

/**
 @brief 
 */
void ReadSubServerConfig( boost::property_tree::ptree &props, CFarmServer &farmSvr )
{
	using std::string;

	string svrType = props.get<string>("type");
	if (svrType.empty())
		return;

	SubServerGroupPtr ptr = farmSvr.FindGroup( svrType );
	if (ptr) // already exist
		return;

	/// farm server config ���Ͽ� ��ϵ� �׷��� �����Ѵ�.
	// config �� ��ϵ��� ���� �׷��� link �� �� ���ܵȴ�.
	SubServerGroupPtr pNewGroup = dynamic_cast<CSubServerGroup*>(
		farmSvr.GetServer()->GetRootGroup().AddChild( farmSvr.GetServer()->GetGroupFactory() ));
	if (!pNewGroup)
		return;

	pNewGroup->SetName( svrType );

	string port = props.get<string>("inner port");
	pNewGroup->SetInnerPortBase( atoi(port.c_str()) );

	port = props.get<string>("outer port");
	pNewGroup->SetOuterPortBase( atoi(port.c_str()) );

	//port = props.get<string>("next inner port");
	//pNewGroup->SetNextInnerPort( atoi(port.c_str()) );

	//port = props.get<string>("next outer port");
	//pNewGroup->SetNextOuterPort( atoi(port.c_str()) );
}


/**
 @brief 
 */
bool	farmsvr::ReadServerGrouprConfig( const std::string &fileName, CFarmServer &farmSvr )
{
	if (!farmSvr.GetServer())
		return false;

	bool result = true;
	try
	{
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		boost::property_tree::read_json(fileName.c_str(), props);

		ptree child = props.get_child("servers");
		auto &it = child.begin();
		while (child.end() != it)
		{
			ReadSubServerConfig(it->second, farmSvr);
			++it;
		}
	}
	catch (std::exception &)
	{
		result  = false;
	}

	return result;
}