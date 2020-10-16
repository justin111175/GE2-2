#include "HostState.h"
#include"../common/_debug/_DebugConOut.h"
#include <Dxlib.h>
#include "../common/Vector2.h"

HostState::HostState()
{
	TRACE("ネットHOST.cpp\n");
	active_ = !PreparationListenNetWork(portNum_);
	//netHandle_=GetNewAcceptNetWork();


	//TRACE("netHandle_確認%d", netHandle_);
	
}

HostState::~HostState()
{
}

bool HostState::test(void)
{

	// GetNewAcceptNetWork:新たに確立した接続を示すネットワークハンドルを得る
	auto tmpID = GetNewAcceptNetWork();
	if (tmpID != -1)
	{
		//接続があった、新規接続止め
		netHandle_ = tmpID;
		StopListenNetWork();
	}

	if (GetLostNetWork() != -1)
	{
		// 切断があったら、再開する
		PreparationListenNetWork(portNum_);
		TRACE("LOST");

		return false;
	}
	return true;
}

bool HostState::CheckNetWork()
{
	Vector2 a;
	NetWorkRecv(netHandle_, &a, sizeof(a));

	return test();
}


