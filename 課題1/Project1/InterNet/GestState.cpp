#include <Dxlib.h>
#include "GestState.h"
#include "../common/_debug/_DebugConOut.h"

GestState::GestState()
{
	TRACE("ƒlƒbƒgGEST.cpp\n");
	active_ = false;
}

GestState::~GestState()
{
}

bool GestState::ConnectHost(IPDATA hostIP)
{
	
	netHandle_ = ConnectNetWork(hostIP,portNum_);
	if (netHandle_ >= 0)
	{
		active_ = true;
	}
	
		
	return active_;

}

bool GestState::CheckNetWork()
{
	if (GetLostNetWork() != -1)
	{
		CloseNetWork(netHandle_);
	}
	return true;
}
