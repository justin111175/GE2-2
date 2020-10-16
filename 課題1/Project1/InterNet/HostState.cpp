#include "HostState.h"
#include"../common/_debug/_DebugConOut.h"
#include <Dxlib.h>
#include "../common/Vector2.h"

HostState::HostState()
{
	TRACE("�l�b�gHOST.cpp\n");
	active_ = !PreparationListenNetWork(portNum_);
	//netHandle_=GetNewAcceptNetWork();


	//TRACE("netHandle_�m�F%d", netHandle_);
	
}

HostState::~HostState()
{
}

bool HostState::test(void)
{

	// GetNewAcceptNetWork:�V���Ɋm�������ڑ��������l�b�g���[�N�n���h���𓾂�
	auto tmpID = GetNewAcceptNetWork();
	if (tmpID != -1)
	{
		//�ڑ����������A�V�K�ڑ��~��
		netHandle_ = tmpID;
		StopListenNetWork();
	}

	if (GetLostNetWork() != -1)
	{
		// �ؒf����������A�ĊJ����
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


