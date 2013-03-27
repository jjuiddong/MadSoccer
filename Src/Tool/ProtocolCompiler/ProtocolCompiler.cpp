// ProtocolCompiler.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Generator/GenerateProtocolCode.h"

using namespace network;

int main(int argc, char* argv[])
{
// 	common::ReferencePtr<int> ptr(new int(10));
// 	delete ptr;
	if (argc >= 2)
	{
		network::CProtocolParser parser;
		sRmi *rmiList = parser.Parse( argv[1] );
		if (rmiList)
		{
			 compiler::WriteProtocolCode(argv[1], rmiList);
		}
	}
	
	return 0;
}
