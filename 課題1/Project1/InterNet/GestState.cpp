#include <Dxlib.h>
#include "GestState.h"
#include "../common/_debug/_DebugConOut.h"

GestState::GestState()
{
	TRACE("ネットGEST.cpp\n");
	active_ = ActiveState::Init;
}

GestState::~GestState()
{
}

ActiveState GestState::ConnectHost(IPDATA hostIP)
{
	// IPアドレスを入れて、接続できるかどうか
	netHandle_ = ConnectNetWork(hostIP,portNum_);
	if (netHandle_ >= 0)
	{
		active_ = ActiveState::Stanby;
	}
	
		
	return active_;

}

bool GestState::CheckNetWork()
{
	// 接続しているかどうか、確認
	if (GetLostNetWork() != -1)
	{
		//切断する
		CloseNetWork(netHandle_);
	}
	return true;
}
