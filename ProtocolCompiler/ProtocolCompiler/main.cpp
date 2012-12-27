
#include <stdio.h>
#include "ProtocolParser.h"


int main(int argc, char* argv[])
{
	if (argc >= 2)
	{
		cProtocolParser parser;
		parser.Parse( argv[1] );
	}
	return 0;
}
