//------------------------------------------------------------------------
// Name:    ProtocolPropertyCtrl.h
// Author:  jjuiddong
// Date:    1/5/2013
// 
// 패킷 정보 입력용 PropertyGridCtrl
//------------------------------------------------------------------------
#pragma once


class CProtocolPropertyCtrl : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CProtocolPropertyCtrl)

public:
	CProtocolPropertyCtrl();
	virtual ~CProtocolPropertyCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};
