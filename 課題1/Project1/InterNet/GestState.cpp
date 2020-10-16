#include <Dxlib.h>
#include "GestState.h"
#include "../common/_debug/_DebugConOut.h"

GestState::GestState()
{
	TRACE("�l�b�gGEST.cpp\n");
	active_ = ActiveState::Init;
}

GestState::~GestState()
{
}

ActiveState GestState::ConnectHost(IPDATA hostIP)
{
	// IP�A�h���X�����āA�ڑ��ł��邩�ǂ���
	netHandle_ = ConnectNetWork(hostIP,portNum_);
	if (netHandle_ >= 0)
	{
		active_ = ActiveState::Stanby;
	}
	
		
	return active_;

}

bool GestState::CheckNetWork()
{
	// �ڑ����Ă��邩�ǂ����A�m�F
	if (GetLostNetWork() != -1)
	{
		//�ؒf����
		CloseNetWork(netHandle_);
	}
	return true;
}
