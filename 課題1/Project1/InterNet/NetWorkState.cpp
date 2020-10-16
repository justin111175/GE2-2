#include "NetWorkState.h"
#include"../common/_debug/_DebugConOut.h"

NetWorkState::NetWorkState()
{
	active_ = ActiveState::Wait;

}

NetWorkState::~NetWorkState()
{
}
ActiveState NetWorkState::ConnectHost(IPDATA hostIP)
{

	return active_;
}

ActiveState NetWorkState::GetActive(void)
{
	return active_;
}



ActiveState NetWorkState::SetActive(ActiveState active)
{
	
	return active_=active;
}



void NetWorkState::Send(Vector2 pos)
{
	NetWorkSend(netHandle_, &pos, sizeof(pos));
}

Vector2 NetWorkState::Recv()
{
	Vector2 pos;
	NetWorkRecv(netHandle_, &pos, sizeof(pos));
	return pos;
}

bool NetWorkState::Updata(void)
{
	if (CheckNetWork() != -1)
	{
		//-1�ȊO�Ȃ�ڑ����Ă��Ȃ�
		return true;
	}
	//-1�Ȃ�ڑ����Ă���


	return false;
}
