#include "HostState.h"
#include"../common/_debug/_DebugConOut.h"
#include <Dxlib.h>
#include "../common/Vector2.h"

HostState::HostState()
{
	TRACE("�z�X�g�ł�\n");

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
		//�ڑ����������A�V�K�ڑ��~��
		TRACE("�Q�X�g�̐ڑ�\n");
		netHandle_ = tmpID;
		StopListenNetWork();
		active_ = ActiveState::Init;
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


