/**
Name:   Factory.h
Author:  jjuiddong
Date:    4/7/2013

	CSession, Group 생성 factory 클래스 정의 
*/
#pragma once

#include "DataStructure/RemoteClient.h"
#include "Controller/Session.h"

namespace network
{

	/// CSession Factory class
	class ISessionFactory
	{
	public:
		virtual ~ISessionFactory() {}
		virtual CSession* New() = 0;
		virtual ISessionFactory* Clone() = 0;
	};

	class CSessionFactory : public ISessionFactory
	{
		virtual CSession* New() { return new CSession(); }
		virtual ISessionFactory* Clone() { return new CSessionFactory(); }
	};

	/// CRemoteSubServer class Factory
	class CRemoteServerFactory : public ISessionFactory
	{
		virtual CSession* New() { return new CRemoteServer(); }
		virtual ISessionFactory* Clone() { return new CRemoteServerFactory(); }
	};


	/// CGroup Factory class
	class CGroup;
	class IGroupFactory
	{
	public:
		virtual ~IGroupFactory() {}
		virtual CGroup* New() = 0;
		virtual IGroupFactory* Clone() = 0;
	};	

	class CGroupFactory : public IGroupFactory
	{
		virtual CGroup* New() { return new CGroup(); }
		virtual IGroupFactory* Clone() { return new CGroupFactory(); }
	};

}
