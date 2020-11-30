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
    Non,                        // 未設定
    Wait,                       // 接続待機状態（ホスト用）
    Init,                       // 初期化中（ゲーム開始準備中、ホスト/ゲスト用）
    Stanby,                     // 初期化情報送信済みの開始待ち（ホスト用）
    Play,                       // ゲーム中（ホスト/ゲスト用）
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


    // GESTモードもHOSTモードどちらも使えます
    void SendStanby(bool stanby);

    Vector2 Recv();                         // データもらう部分
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

