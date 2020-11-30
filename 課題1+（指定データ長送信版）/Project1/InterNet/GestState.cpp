#include <Dxlib.h>
#include "GestState.h"
#include "../common/_debug/_DebugConOut.h"

GestState::GestState()
{
	TRACE("ネットGEST.cpp\n");
	active_ = ActiveState::Wait;
}

GestState::~GestState()
{
}

ActiveState GestState::ConnectHost(IPDATA hostIP)
{
	auto handle = ConnectNetWork(hostIP, portNum_);

	// IPアドレスを入れて、接続できるかどうか
	netHandle_.emplace_back(handle,0);

	if (handle >= 0)
	{



		active_ = ActiveState::Init;
	}
	
	//TRACE("Active状態->0:Non　1:Wait  2:Init  3:Stanby  4:Play\n");
	//TRACE("active_の状態：%d\n", active_);
	//TRACE("接続失敗\n");
	return active_;

}

bool GestState::CheckNetWork()
{
	// 接続しているかどうか、確認
	if (GetLostNetWork() != -1)
	{
		//切断する
		CloseNetWork(netHandle_.front().first);
	}
	return true;
}
