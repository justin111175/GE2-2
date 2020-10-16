#pragma once
#include<Dxlib.h>
#include "../common/Vector2.h"
enum class NetWorkMode
{
    OFFLINE,
    HOST,
    GEST,
    MAX

};

class NetWorkState
{
public:
    NetWorkState();
    virtual ~NetWorkState();
    virtual NetWorkMode GetMode(void) { return NetWorkMode::OFFLINE;};
    bool GetActive(void);
    virtual bool ConnectHost(IPDATA hostIP);
   
    // GESTモードもHOSTモードどちらも使えます
    void Send(Vector2 pos);                 //データ送る部分
    Vector2 Recv();                         // データもらう部分

    bool Updata(void);

private:
    virtual bool CheckNetWork() { return false; };

protected:
   const int portNum_=8086;

   bool  active_;
   int netHandle_ = 0;
};

