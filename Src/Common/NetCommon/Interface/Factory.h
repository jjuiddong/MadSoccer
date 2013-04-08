/**
Name:   Factory.h
Author:  jjuiddong
Date:    4/7/2013

	RemoteClient, Group 생성 factory 클래스 정의 
*/
#pragma once

#include "DataStructure/RemoteClient.h"

namespace network
{

	/// CRemoteClient Factory class
	class IRemoteClientFactory
	{
	public:
		virtual ~IRemoteClientFactory() {}
		virtual CRemoteClient* New() = 0;
		virtual IRemoteClientFactory* Clone() = 0;
	};

	class CRemoteClientFactory : public IRemoteClientFactory
	{
		virtual CRemoteClient* New() { return new CRemoteClient(); }
		virtual IRemoteClientFactory* Clone() { return new CRemoteClientFactory(); }
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
