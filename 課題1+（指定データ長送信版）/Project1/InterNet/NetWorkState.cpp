#include "NetWorkState.h"
#include"../common/_debug/_DebugConOut.h"

NetWorkState::NetWorkState()
{


	active_ = ActiveState::Init;


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


void NetWorkState::SendStanby(bool stanby)
{

	//NetWorkSend(netHandle_.front().first, &stanby, sizeof(stanby));
	TRACE("��������񑗐M�����A�J�n�҂�\n");

}



Vector2 NetWorkState::Recv()
{
	Vector2 pos;
	//NetWorkRecv(netHandle_.front().first, &pos, sizeof(pos));
	return pos;
}

bool NetWorkState::RecvStanby()
{
	bool ready;
	//NetWorkRecv(netHandle_.front().first, &ready, sizeof(ready));
	return false;
	//return ready;

}

bool NetWorkState::Updata(void)
{

	if (CheckNetWork())
	{
		
		return true;
	}

	return false;
}

int NetWorkState::GetHandle()
{
	auto data = netHandle_.front().first;
	return data;
}


