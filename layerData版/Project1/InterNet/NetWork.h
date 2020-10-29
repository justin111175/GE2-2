#pragma once
#include "Dxlib.h"
#include "NetWorkState.h"
#include <memory>
#include <map>
#include <functional>
#include <vector>
#include <chrono>

#define IpNetwork NetWork::GetInstance()


using RevBox = std::vector<int>;


enum class MesType:unsigned char
{
	STANDY,					// 初期化情報送信完了（ホスト用）
	GAME_START,				// ホストからの初期化情報での初期化完了
	TMX_SIZE,
	TMX_DATA,
	POS
};

struct MesData
{
	MesType type;
	short int id;
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




	void TEST(std::string);

	bool SendMes(MesData& data);
	void Send(std::string file);



	void SendStanby(void);
	void SendStart();
	bool GetRevStanby();

	bool SetNetWorkMode(NetWorkMode mode);

	NetWorkMode GetNetWorkMode(void);

	ActiveState ConnectHost(IPDATA hostIP);
	IPDATA GetIp(void);

	ActiveState GetActiv(void);

	void NetRev();

	bool revStanby_;

private:

	std::chrono::system_clock::time_point  start, end;

	RevBox revtmx_;


	std::map<ActiveState, std::function<bool(void)>> funcAct_;
	std::vector<int> layerData;

	std::unique_ptr<NetWorkState> state_;

	MesData mesData_;


	int retest = 0;

	NetWork();
	~NetWork();

};

