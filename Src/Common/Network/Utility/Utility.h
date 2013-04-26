//------------------------------------------------------------------------
// Name:    NetworkUtility.h
// Author:  jjuiddong
// Date:    2/28/2013
// 
// network ���̺귯������ ���Ǵ� ���� ��ƿ��Ƽ��
//------------------------------------------------------------------------
#pragma once

namespace network
{

	struct SFd_Set : fd_set
	{
		netid netid_array[ FD_SETSIZE];		// fd_array[] ���Ͽ� �ش��ϴ� netid ��
	};


	struct STimer
	{
		int id;
		int interval;
		int beginT;
		bool repeat;

		STimer() {}
		STimer(int _id):id(_id) {}
		STimer(int _id, int _interval, int _beginT, bool _repeat) : id(_id), interval(_interval), beginT(_beginT), repeat(_repeat) {}
		bool operator==(const STimer&rhs) { return id == rhs.id; }
	};


	//struct S

}
