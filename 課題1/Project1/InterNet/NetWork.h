#pragma once
#include "Dxlib.h"
#include "NetWorkState.h"
#include <memory>
#include <map>
#include <functional>

#define IpNetwork NetWork::GetInstance()

enum class MesType
{
	STANDY,					// ��������񑗐M�����i�z�X�g�p�j
	GAME_START,				// �z�X�g����̏��������ł̏���������
	POS
};

struct MesData
{
	MesType type;
	int data[2];
};

class NetWork
{
public:

	static NetWork& GetInstance(void)
	{
		static NetWork s_Instance;
		return s_Instance;
	}
	
	bool Updata(void);

	bool CloseNetWork();
	bool SendMes(MesData& data);
	void SendStanby(void);
	void SendStart();
	bool GetRevStanby();

	bool SetNetWorkMode(NetWorkMode mode);

	NetWorkMode GetNetWorkMode(void);

	ActiveState ConnectHost(IPDATA hostIP);
	IPDATA GetIp(void);

	ActiveState GetActiv(void);




	void Send(Vector2 pos);
	Vector2 Recv();
private:

	bool Wait();

	std::map<ActiveState, std::function<bool(void)>> funcAct_;

	bool revStanby_;

	std::unique_ptr<NetWorkState> state_;

	MesData mesData_;

	NetWork();
	~NetWork();

};

