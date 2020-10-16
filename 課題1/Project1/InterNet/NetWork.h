#pragma once
#include "Dxlib.h"
#include "NetWorkState.h"
#include <memory>
#define IpNetwork NetWork::GetInstance()

class NetWork
{
public:

	static NetWork& GetInstance(void)
	{
		static NetWork s_Instance;
		return s_Instance;
	}

	bool SetNetWorkMode(NetWorkMode mode);
	NetWorkMode GetNetWorkMode(void);


	bool ConnectHost(IPDATA hostIP);
	bool GetActiv(void);
	IPDATA GetIp(void);

	bool Updata(void);
	void Send(Vector2 pos);
	Vector2 Recv();
private:
	
	std::unique_ptr<NetWorkState> state_;
	NetWork();
	~NetWork();

};

