#include "NetWorkState.h"
#include"../common/_debug/_DebugConOut.h"

NetWorkState::NetWorkState()
{
	active_ = false;

}

NetWorkState::~NetWorkState()
{
}

bool NetWorkState::GetActive(void)
{
	return active_;
}

bool NetWorkState::ConnectHost(IPDATA hostIP)
{

	return false;
}



void NetWorkState::Send(Vector2 pos)
{
	NetWorkSend(netHandle_, &pos, sizeof(pos));
}

Vector2 NetWorkState::Recv()
{
	Vector2 a;
	NetWorkRecv(netHandle_, &a, sizeof(a));
	return a;
}

bool NetWorkState::Updata(void)
{
	if (CheckNetWork() != -1)
	{
		//-1ˆÈŠO‚È‚çÚ‘±‚µ‚Ä‚¢‚È‚¢
		//NetWorkSend(netHandle_, "Œq‚ª‚Á‚½‚¼`II", 17);

		return true;

	}

	//-1‚È‚çÚ‘±‚µ‚Ä‚¢‚é

	//NetWorkSend(netHandle_, &pos, sizeof(pos));

	////NetWorkRecv

	return false;
}
