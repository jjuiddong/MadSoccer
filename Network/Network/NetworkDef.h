
#pragma once


#include <Windows.h>
#include <winsock.h>
#include <string>


// class CServer;
// class CClient;

enum NETWORK_TYPE
{
	SERVER,
	CLIENT,		
};

typedef struct _SSeverData
{
	int port;

} SSeverData;

typedef struct _SClientData
{
	std::string ip;
	int port;

} SClientData;

