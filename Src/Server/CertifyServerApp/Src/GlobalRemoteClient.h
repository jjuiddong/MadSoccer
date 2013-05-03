/**
Name:   GlobalRemoteClient.h
Author:  jjuiddong
Date:    5/2/2013

	전체 서버에서 동일하게 관리되는 클라이언트 객체
*/
#pragma once

class CGlobalRemoteClient : public network::CSession
{
public:
	CGlobalRemoteClient() {}
	virtual ~CGlobalRemoteClient() {}
	void		SetLocateSvrType(const std::string &svrType);
	const std::string& GetLocateSvrType() const;

protected:

private:
	std::string		m_LocateSvrType;

};


inline void	CGlobalRemoteClient::SetLocateSvrType(const std::string &svrType) { m_LocateSvrType = svrType; }
inline const std::string& CGlobalRemoteClient::GetLocateSvrType() const { return m_LocateSvrType; }

