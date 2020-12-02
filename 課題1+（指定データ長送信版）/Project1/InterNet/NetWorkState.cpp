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
	TRACE("初期化情報送信完了、開始待ち\n");

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
	int data;
	if (netHandle_.size())
	{
		data = netHandle_.front().first;
		return data;

	}
	return -1;
}

ListInt NetWorkState::GetHandleAll()
{
	return netHandle_;
}


