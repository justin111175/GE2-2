#pragma once
#include<Dxlib.h>
#include "../common/Vector2.h"
#include <iostream>
#include <utility>
#include <list>
#include <string>

enum class NetWorkMode
{
    OFFLINE,
    HOST,
    GEST,
    MAX

};



enum class ActiveState
{
    Non,                        // ���ݒ�
    Wait,                       // �ڑ��ҋ@��ԁi�z�X�g�p�j
    Init,                       // ���������i�Q�[���J�n�������A�z�X�g/�Q�X�g�p�j
    Stanby,                     // ��������񑗐M�ς݂̊J�n�҂��i�z�X�g�p�j
    Play,                       // �Q�[�����i�z�X�g/�Q�X�g�p�j
    Offline

};


using ListInt = std::list<std::pair<int, unsigned int>>;

class NetWorkState
{
public:
    NetWorkState();
    virtual ~NetWorkState();
    virtual NetWorkMode GetMode(void) { return NetWorkMode::OFFLINE;};
   
    
    virtual ActiveState ConnectHost(IPDATA hostIP);
    
    ActiveState SetActive(ActiveState active);
    ActiveState GetActive(void);


    // GEST���[�h��HOST���[�h�ǂ�����g���܂�
    void SendStanby(bool stanby);

    Vector2 Recv();                         // �f�[�^���炤����
    bool RecvStanby();
    bool Updata(void);


    int GetHandle();
    int playerID;
    unsigned int playerMax;
private:
    virtual bool CheckNetWork() { return false; };

protected:
   ActiveState  active_;


   const int portNum_=8086;

   ListInt netHandle_;
};

