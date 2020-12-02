#include "HostState.h"
#include"../common/_debug/_DebugConOut.h"
#include <Dxlib.h>
#include "../common/Vector2.h"
#include "NetWork.h"
HostState::HostState()
{
	TRACE("�z�X�g�ł�\n");
	cnt_ = 1;
	//!PreparationListenNetWork(portNum_)==1 �ڑ��J�n�@0�ڑ����s

	if(!PreparationListenNetWork(portNum_) == 1)
	{

		active_ = ActiveState::Wait;
		TRACE("��Ԃ�%d�ł�\n", active_);

	}

	
}

HostState::~HostState()
{
}


bool HostState::CheckNetWork()
{

	// GetNewAcceptNetWork:�V���Ɋm�������ڑ��������l�b�g���[�N�n���h���𓾂�
	auto tmpID = GetNewAcceptNetWork();
	if (tmpID != -1)
	{
		
		if (!netHandle_.size())
		{
			IpNetwork.sendTime_ = std::chrono::system_clock::now();
			IpNetwork.start = IpNetwork.sendTime_ + std::chrono::milliseconds(15000);
		}

		//�ڑ����������A�V�K�ڑ��~��
		TRACE("�Q�X�g�̐ڑ� : %d��->%d\n",cnt_, tmpID);
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
		// �ؒf����������A�ĊJ����
		PreparationListenNetWork(portNum_);
		TRACE("�Q�X�g�̐ؒf\n");
		active_ = ActiveState::Wait;

		return false;
	}
	return true;
}


