//------------------------------------------------------------------------
// Name:    Src\basic_ProtocolData.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

namespace basic {

using namespace network;
using namespace marshalling;


	struct func1_Packet {
		IProtocolDispatcher *pdispatcher;
		netid senderId;
	};

	struct func2_Packet {
		IProtocolDispatcher *pdispatcher;
		netid senderId;
		std::string str;
	};

	struct func3_Packet {
		IProtocolDispatcher *pdispatcher;
		netid senderId;
		float value;
	};

	struct func4_Packet {
		IProtocolDispatcher *pdispatcher;
		netid senderId;
	};





	struct func2_Packet {
		IProtocolDispatcher *pdispatcher;
		netid senderId;
		std::string str;
	};

	struct func3_Packet {
		IProtocolDispatcher *pdispatcher;
		netid senderId;
		float value;
	};



}
