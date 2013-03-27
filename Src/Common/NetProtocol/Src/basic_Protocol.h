//------------------------------------------------------------------------
// Name:    C:\GitHub\MadSoccer\Src\Common\NetProtocol\Src\basic_Protocol.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

namespace basic {
static const int s2c_Protocol_ID= 500;

class s2c_Protocol : public network::IProtocol
{
public:
	s2c_Protocol() : IProtocol(s2c_Protocol_ID) {}
	void AckGroupList(netid targetId, const network::SEND_FLAG flag, const network::error::ERROR_CODE &errorCode, const GroupVector &groups);
	void AckGroupJoin(netid targetId, const network::SEND_FLAG flag, const network::error::ERROR_CODE &errorCode, const netid &reqId, const netid &joinGroupId);
	void AckGroupCreate(netid targetId, const network::SEND_FLAG flag, const network::error::ERROR_CODE &errorCode, const netid &reqId, const netid &crGroupId, const netid &crParentGroupId, const std::string &groupName);
	void AckGroupCreateBlank(netid targetId, const network::SEND_FLAG flag, const network::error::ERROR_CODE &errorCode, const netid &reqId, const netid &crGroupId, const netid &crParentGroupId, const std::string &groupName);
	void JoinMember(netid targetId, const network::SEND_FLAG flag, const netid &toGroupId, const netid &fromGroupId, const netid &userId);
	void AckP2PConnect(netid targetId, const network::SEND_FLAG flag, const network::error::ERROR_CODE &errorCode, const network::P2P_STATE &state, const std::string &ip, const int &port);
	void func1(netid targetId, const network::SEND_FLAG flag);
	void func2(netid targetId, const network::SEND_FLAG flag, const std::string &str);
	void func3(netid targetId, const network::SEND_FLAG flag, const float &value);
	void func4(netid targetId, const network::SEND_FLAG flag);
	void func5(netid targetId, const network::SEND_FLAG flag, const std::string &ok, const float &a, const int &b);
};
static const int c2s_Protocol_ID= 600;

class c2s_Protocol : public network::IProtocol
{
public:
	c2s_Protocol() : IProtocol(c2s_Protocol_ID) {}
	void ReqGroupList(netid targetId, const network::SEND_FLAG flag, const netid &groupid);
	void ReqGroupJoin(netid targetId, const network::SEND_FLAG flag, const netid &groupid);
	void ReqGroupCreate(netid targetId, const network::SEND_FLAG flag, const netid &parentGroupId, const std::string &groupName);
	void ReqGroupCreateBlank(netid targetId, const network::SEND_FLAG flag, const netid &parentGroupId, const std::string &groupName);
	void ReqP2PConnect(netid targetId, const network::SEND_FLAG flag);
	void ReqP2PConnectTryResult(netid targetId, const network::SEND_FLAG flag, const bool &isSuccess);
	void func2(netid targetId, const network::SEND_FLAG flag, const std::string &str);
	void func3(netid targetId, const network::SEND_FLAG flag, const float &value);
};
}
