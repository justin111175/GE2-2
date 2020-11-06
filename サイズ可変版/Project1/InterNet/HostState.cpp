#include "HostState.h"
#include"../common/_debug/_DebugConOut.h"
#include <Dxlib.h>
#include "../common/Vector2.h"

HostState::HostState()
{
	TRACE("ホストです\n");

	//!PreparationListenNetWork(portNum_)==1 接続開始　0接続失敗

	if(!PreparationListenNetWork(portNum_) == 1)
	{

		active_ = ActiveState::Wait;
		TRACE("状態は%dです\n", active_);

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
		TRACE("ゲストの接続\n");
		netHandle_ = tmpID;
		StopListenNetWork();
		active_ = ActiveState::Init;
	}

	if (GetLostNetWork() != -1)
	{
		// 切断があったら、再開する
		PreparationListenNetWork(portNum_);
		TRACE("ゲストの切断\n");
		active_ = ActiveState::Wait;

		return false;
	}
	return true;
}


