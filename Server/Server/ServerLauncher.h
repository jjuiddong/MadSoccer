//------------------------------------------------------------------------
// Name:    ServerLauncher.h
// Author:  jjuiddong
// Date:    12/21/2012
// 
// �������� ��ũ��Ʈ�� �о ������ �����Ų��.
//------------------------------------------------------------------------

#pragma once


class CServerLauncher : public common::CSingleton<CServerLauncher>
{
public:
	bool Launcher( std::string configFileName );


};
