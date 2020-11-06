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

struct MesHeader//8
{
	//MesType type;
	//unsigned short int sdata;			//ID
	//unsigned char cdata;		
	//int data[2];

	MesType type;//1
	unsigned char cdata;//1
	unsigned short id;//2
	unsigned int length;//4

};
union UnionData
{
	unsigned int uiData;//4
	int iData;//4
	char cData[4];//4
};

union Header
{
	MesHeader mesheader;//8
	unsigned int header[2];
};



using MesPacket = std::vector<UnionData>;

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

	void SetHeader(Header header, MesPacket& mesP_);


	bool RevTmx(std::string);

	bool SendMes(MesPacket& data);
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
	
	std::chrono::system_clock::time_point  start, end;

private:

	UpdataMode mode_;
	std::map<UpdataMode, std::function<void(void)>> func_;






	void ThreadUpdata(void);

	RevBox revtmx_;


	std::map<ActiveState, std::function<bool(void)>> funcAct_;
	std::vector<int> layerData;

	std::unique_ptr<NetWorkState> state_;

	MesHeader mesData_;

	int retest = 0;

	NetWork();
	~NetWork();

};

