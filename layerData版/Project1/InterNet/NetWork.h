#pragma once
#include "Dxlib.h"
#include "NetWorkState.h"
#include <map>
#include <functional>
#include <vector>
#include <chrono>
#include <thread>
#include "../tmx/TmxObj.h"
#include <memory>

#define IpNetwork NetWork::GetInstance()


enum class UpdataMode
{
	SetNetWork,
	GetHostIP,
	StartInit,
	Play


};

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
	unsigned short int sdata;			//ID
	unsigned char cdata;		
	int data[2];
};

union UnionData
{
	char cData[8];
	int iData[2];
	long long IData;
};

using RevBox = std::vector<UnionData>;


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




	bool TEST(std::string);

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
	
	std::unique_ptr<TmxObj> tmx_;

	std::thread updata_;
	
	void SetNetWork();
	void GetHostIp();
	void StartInit();
	void Play();

	UpdataMode GetMode();
private:

	UpdataMode mode_;
	std::map<UpdataMode, std::function<void(void)>> func_;






	void ThreadUpdata(void);
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

