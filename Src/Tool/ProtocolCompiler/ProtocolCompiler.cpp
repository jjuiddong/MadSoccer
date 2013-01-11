// ProtocolCompiler.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Network/PrtCompiler/ProtocolParser.h"
#include "Generator/GenerateProtocolCode.h"

using namespace network;

int main(int argc, char* argv[])
{
	if (argc >= 2)
	{
		network::cProtocolParser parser;
		sRmi *rmiList = parser.Parse( argv[1] );
		if (rmiList)
		{
			 compiler::WriteProtocolCode(argv[1], rmiList);
		}
	}
	return 0;
}
