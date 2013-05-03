/**
Name:   GlobalRemoteClient.h
Author:  jjuiddong
Date:    5/2/2013

	��ü �������� �����ϰ� �����Ǵ� Ŭ���̾�Ʈ ��ü
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

