//------------------------------------------------------------------------
// Name:    ProtocolMacro.h
// Author:  jjuiddong
// Date:    1/4/2013
// 
// 프로토콜 컴파일러가 생성하는 Protocol, ProtocolListener에 들어가는 매크로
// 들을 모아놓았다.
//------------------------------------------------------------------------
#pragma once


// Dispatcher::Dispatch 에서 사용되는 매크로
#define SEND_LISTENER(listenerType, listeners, func)											\
	BOOST_FOREACH(ProtocolListenerPtr p, listeners)												\
	{																							\
		listenerType *lstr = dynamic_cast<listenerType*>((IProtocolListener*)p);				\
		if (lstr)																				\
			lstr->func;																			\
	}																							\


