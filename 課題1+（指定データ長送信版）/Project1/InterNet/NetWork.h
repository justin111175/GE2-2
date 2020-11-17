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
	STANDY,							// ��������񑗐M�����i�z�X�g�p�j
	GAME_START,						// �z�X�g����̏��������ł̏���������
	TMX_SIZE,
	TMX_DATA,
	POS
};

struct MesHeader
{

	MesType type;
	unsigned char next;				// �f�[�^���M�������̏ꍇ�ɁA��������ꍇ�F1�@�A�Ȃ��ꍇ�F0
	unsigned short sendID;			// �������M���̃i���o�����O
	unsigned int length;			// �������ǂ����Ɋւ�炸�A�P��o�P�b�g�̃f�[�^���iint�̐��j

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


using MesPacket = std::vector<UnionData>;
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


	bool SendMes(MesType type);
	bool SendMes(MesType type, MesPacket& mesPacket);

	void GetSetting(const char* setting);

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
	
	unsigned int intSendCount_;


	int dir_;
	Vector2 pos_;
	std::map<int,std::list<MesPacket>> mapID_;
	std::list<MesPacket> palyerList_;
	MesPacket posPacket_;
	
	MesPacket GetList(int id);
private:
	std::map<MesType, std::function<void(void)>> revFunc_;

	void RevInit();


	UpdataMode mode_;

	std::mutex mtx;


	void ThreadUpdata(void);

	MesPacket revtmx_;

	std::vector<int> layerData;

	std::unique_ptr<NetWorkState> state_;

	MesHeader mesData_;

	int retest = 0;

	NetWork();
	~NetWork();

};

