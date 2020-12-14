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
	COUNT_DOWN_ROOM,						// �ڑ���t�J���g�_�E��
	ID,								// ������ID�ƃv���C���[�̐�
	STANDY_HOST,							// ��������񑗐M�����i�z�X�g�p�j
	STANDY_GEST,						// �z�X�g����̏��������ł̏���������
	COUNT_DOWN_GAME,						// �S���̏�����������Q�[���J�n����
	TMX_SIZE,
	TMX_DATA,						// 
	POS,							// �Q�[�����f�[�^
	SET_BOMB,						// �{���̐ݒu
	DEATH,							// ���S-�L�����N�^�[ID
	LOST,							// �ؒf���ɐ����i�z�X�g�͎����̃l�b�g���[�N�L�����ɂ��Z�b�g����j
	MAX
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
	bool countFlag_;	// �Q�[���V�[���J�n���邩�ǂ����itrue:�J�n�O�Afalse:�J�n�j
	
	std::pair<int, int> GetPlayerPac();

private:
	
	std::pair<int, int> playerPac_;//�v���C���[��ID�A�v���C���[�̐�


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

