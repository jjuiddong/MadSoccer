// ProtocolCompiler.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
