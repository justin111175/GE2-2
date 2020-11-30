#include <Dxlib.h>
#include "GestState.h"
#include "../common/_debug/_DebugConOut.h"

GestState::GestState()
{
	TRACE("�l�b�gGEST.cpp\n");
	active_ = ActiveState::Wait;
}

GestState::~GestState()
{
}

ActiveState GestState::ConnectHost(IPDATA hostIP)
{
	auto handle = ConnectNetWork(hostIP, portNum_);

	// IP�A�h���X�����āA�ڑ��ł��邩�ǂ���
	netHandle_.emplace_back(handle,0);

	if (handle >= 0)
	{



		active_ = ActiveState::Init;
	}
	
	//TRACE("Active���->0:Non�@1:Wait  2:Init  3:Stanby  4:Play\n");
	//TRACE("active_�̏�ԁF%d\n", active_);
	//TRACE("�ڑ����s\n");
	return active_;

}

bool GestState::CheckNetWork()
{
	// �ڑ����Ă��邩�ǂ����A�m�F
	if (GetLostNetWork() != -1)
	{
		//�ؒf����
		CloseNetWork(netHandle_.front().first);
	}
	return true;
}
