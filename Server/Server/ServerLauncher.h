//------------------------------------------------------------------------
// Name:    ServerLauncher.h
// Author:  jjuiddong
// Date:    12/21/2012
// 
// 서버시작 스크립트를 읽어서 서버를 실행시킨다.
//------------------------------------------------------------------------

#pragma once


class CServerLauncher : public common::CSingleton<CServerLauncher>
{
public:
	bool Launcher( std::string configFileName );


};
