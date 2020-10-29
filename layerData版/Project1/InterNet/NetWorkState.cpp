#include "NetWorkState.h"
#include"../common/_debug/_DebugConOut.h"

NetWorkState::NetWorkState()
{


	//active_ = ActiveState::Init;


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

void NetWorkState::SendStanby(bool stanby)
{

	NetWorkSend(netHandle_, &stanby, sizeof(stanby));
	TRACE("初期化情報送信完了、開始待ち\n");

}



Vector2 NetWorkState::Recv()
{
	Vector2 pos;
	NetWorkRecv(netHandle_, &pos, sizeof(pos));
	return pos;
}

bool NetWorkState::RecvStanby()
{
	bool ready;
	NetWorkRecv(netHandle_, &ready, sizeof(ready));
	return ready;

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
	return netHandle_;
}


