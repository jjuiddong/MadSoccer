/**
Name:   ClientUserAccess.h
Author:  jjuiddong
Date:    3/17/2013

adapter class for access the user object in CClient class object
*/
namespace network
{
	class CClientUserAccess : public IUserAccess
	{
	public:
		CClientUserAccess( ClientBasicPtr svr );
		virtual ~CClientUserAccess();
		virtual RemoteClientPtr GetUser(netid id) override;
	protected:
		ClientBasicPtr m_pSvr;
	};

	// not implements

}
