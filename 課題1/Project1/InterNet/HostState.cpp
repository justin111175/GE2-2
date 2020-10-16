#include "HostState.h"
#include"../common/_debug/_DebugConOut.h"
#include <Dxlib.h>
#include "../common/Vector2.h"

HostState::HostState()
{
	TRACE("ネットHOST.cpp\n");

	//!PreparationListenNetWork(portNum_)==1 接続開始　0接続失敗
	active_ = ActiveState::Wait;

	if(!PreparationListenNetWork(portNum_) == 1)
	{
		active_ = ActiveState::Stanby;
	}

	
}

HostState::~HostState()
{
}


bool HostState::CheckNetWork()
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


