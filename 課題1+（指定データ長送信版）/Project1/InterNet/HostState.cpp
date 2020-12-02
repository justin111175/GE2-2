#include "HostState.h"
#include"../common/_debug/_DebugConOut.h"
#include <Dxlib.h>
#include "../common/Vector2.h"
#include "NetWork.h"
HostState::HostState()
{
	TRACE("ホストです\n");
	cnt_ = 1;
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
		
		if (!netHandle_.size())
		{
			IpNetwork.sendTime_ = std::chrono::system_clock::now();
			IpNetwork.start = IpNetwork.sendTime_ + std::chrono::milliseconds(15000);
		}

		//接続があった、新規接続止め
		TRACE("ゲストの接続 : %d番->%d\n",cnt_, tmpID);
		netHandle_.emplace_back(tmpID,cnt_);
		
		cnt_++;

		UnionTime time_ = { IpNetwork.sendTime_ };
		MesPacket data_;

		UnionData unionD;

		unionD.iData = time_.iData[0];
		data_.insert(data_.end(), unionD);
		unionD.iData = time_.iData[1];
		data_.insert(data_.end(), unionD);

		IpNetwork.SendMesAll(MesType::COUNT_DOWN_ROOM,data_,tmpID);




		//active_ = ActiveState::Init;
		//StopListenNetWork();

		
		return true;
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


