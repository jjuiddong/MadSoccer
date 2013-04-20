/**
Name:   SubServerGroup.h
Author:  jjuiddong
Date:    4/7/2013

서브서버 그룹
*/
#pragma once

class CSubServerGroup : public network::CGroup
{
public:
	void SetSvrType(const std::string &svrType);
	void SetOuterPortBase(int port);
	void SetInnerPortBase(int port);
	//void SetNextOuterPort(int port);
	//void SetNextInnerPort(int port);
	const std::string& GetSvrType() const;
	int GetOuterPortBase() const;
	int GetInnerPortBase() const;
	//int GetNextOuterPort() const;
	//int GetNextInnerPort() const;

	bool GetServerInfoCorrespondClient( const std::string &clientSvrType, const std::string &serverSvrType, 
		network::IUserAccess &userAccess, OUT std::vector<network::SHostInfo> &v);

	bool GetCorrespondClientInfo( network::IUserAccess &userAccess, OUT std::vector<std::string> &svrTypes );

	int GetToBindOuterPort(network::IUserAccess &userAccess);
	int GetToBindInnerPort(network::IUserAccess &userAccess);

private:
	std::string m_SvrType;
	int	m_OuterPortBase;
	int	m_InnerPortBase;
	//int	m_NextOuterPort;
	//int	m_NextInnerPort;
};


inline void CSubServerGroup::SetSvrType(const std::string &svrType) { m_SvrType = svrType; }
inline void CSubServerGroup::SetOuterPortBase(int port) { m_OuterPortBase = port; }
inline void CSubServerGroup::SetInnerPortBase(int port) { m_InnerPortBase = port; }
inline const std::string& CSubServerGroup::GetSvrType()  const { return m_SvrType; }
inline int CSubServerGroup::GetOuterPortBase() const { return m_OuterPortBase; }
inline int CSubServerGroup::GetInnerPortBase() const { return m_InnerPortBase; }
//inline void CSubServerGroup::SetNextOuterPort(int port) { m_NextOuterPort = port; }
//inline void CSubServerGroup::SetNextInnerPort(int port) { m_NextInnerPort = port; }
//inline int CSubServerGroup::GetNextOuterPort() const { return m_NextOuterPort; }
//inline int CSubServerGroup::GetNextInnerPort() const { return m_NextInnerPort; }


class CSubServerGroupFactory : public network::IGroupFactory
{
	virtual network::CGroup* New() { return new CSubServerGroup(); }
	virtual network::IGroupFactory* Clone() { return new CSubServerGroupFactory(); }
};

