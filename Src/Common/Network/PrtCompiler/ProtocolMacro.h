//------------------------------------------------------------------------
// Name:    ProtocolMacro.h
// Author:  jjuiddong
// Date:    1/4/2013
// 
// �������� �����Ϸ��� �����ϴ� Protocol, ProtocolListener�� ���� ��ũ��
// ���� ��Ƴ��Ҵ�.
//------------------------------------------------------------------------
#pragma once


// Dispatcher::Dispatch ���� ���Ǵ� ��ũ��
#define SEND_LISTENER(listenerType, listeners, func)											\
	BOOST_FOREACH(ProtocolListenerPtr p, listeners)												\
	{																							\
		listenerType *lstr = dynamic_cast<listenerType*>((IProtocolListener*)p);				\
		if (lstr)																				\
			lstr->func;																			\
	}																							\


