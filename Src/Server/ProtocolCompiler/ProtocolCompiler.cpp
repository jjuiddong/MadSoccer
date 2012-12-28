// ProtocolCompiler.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Compiler/ProtocolParser.h"


int main(int argc, char* argv[])
{
	if (argc >= 2)
	{
		cProtocolParser parser;
		parser.Parse( argv[1] );
	}
	return 0;
}
