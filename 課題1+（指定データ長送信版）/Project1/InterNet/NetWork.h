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
#include <list>
#include <mutex>
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
	NON=100,
	COUNT_DOWN_ROOM,						// 接続受付カントダウン
	ID,								// 自分のIDとプレイヤーの数
	STANDY_HOST,							// 初期化情報送信完了（ホスト用）
	STANDY_GEST,						// ホストからの初期化情報での初期化完了
	COUNT_DOWN_GAME,						// 全員の初期化完了後ゲーム開始時間
	TMX_SIZE,
	TMX_DATA,						// 
	POS,							// ゲーム中データ
	SET_BOMB,						// ボムの設置
	DEATH,							// 死亡-キャラクターID
	LOST,							// 切断時に生成（ホストは自分のネットワークキャラにもセットする）
	MAX
};

struct MesHeader
{

	MesType type;
	unsigned char next;				// データ送信が分割の場合に、次がある場合：1　、ない場合：0
	unsigned short sendID;			// 分割送信時のナンバリング
	unsigned int length;			// 分割かどうかに関わらず、単一バケットのデータ長（intの数）

};

union UnionData
{
	unsigned int uiData;
	int iData;
	char cData[4];
};

union Header
{
	MesHeader mesheader;
	unsigned int header[2];
};

union UnionTime
{
	std::chrono::system_clock::time_point start_;
	int iData[2];

};


using MesPacket = std::vector<UnionData>;

using RevPacket = std::pair<MesType, MesPacket>;



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


	bool SendMesAll(MesType type);
	bool SendMesAll(MesType type, MesPacket& mesPacket);

	bool SendMesAll(MesType type, MesPacket& mesPacket,int);

	ListInt GetHandleAll();

	void GetSetting(const char* setting);




	bool GetRevStanby();

	bool SetNetWorkMode(NetWorkMode mode);

	NetWorkMode GetNetWorkMode(void);

	ActiveState ConnectHost(IPDATA hostIP);
	IPDATA GetIp(void);

	ActiveState GetActiv(void);
	bool SetActive(ActiveState active);

	bool revStanby_;
	


	UpdataMode GetMode();
	
	std::chrono::system_clock::time_point  start, end,sendTime_;
	int time_;
	std::unique_ptr<TmxObj> tmx_;

	unsigned int intSendCount_;
	int dir_;
	Vector2 pos_;
	std::list<MesPacket> VecID_;
	std::list<RevPacket> TypePacket_;

	MesPacket revtmx_;

	int GetHandle();


	MesPacket GetPacket(int id);
	MesPacket GetNewPacket(MesType type);
	void GetSavePacket(MesType type);


	std::map<MesType, std::list<MesPacket>> saveData_;

	void EraserPac();
	bool countFlag_;	// ゲームシーン開始するかどうか（true:開始前、false:開始）
	
	std::pair<int, int> GetPlayerPac();

private:
	
	std::pair<int, int> playerPac_;//プレイヤーのID、プレイヤーの数


	std::map<MesType, std::function<void(int, MesHeader&, MesPacket&)>> revFunc_;
	
	std::unique_ptr<NetWorkState> state_;
	
	std::thread updata_;


	void RevInit();


	UpdataMode mode_;

	std::mutex mtx;


	void ThreadUpdata(void);

	std::vector<int> layerData;

	int retest = 0;



	NetWork();
	~NetWork();

};

