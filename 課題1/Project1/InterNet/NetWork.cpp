#include "NetWork.h"
#include"HostState.h"
#include "GestState.h"
#include "../common/_debug/_DebugConOut.h"

IPDATA NetWork::GetIp(void)
{

    IPDATA ip;
    GetMyIPAddress(&ip);

    return ip;
}

bool NetWork::Updata()
{

    //state_->GetActive();
   //return funcAct_[state_->GetActive()]();


    return state_->Updata();
   //TRACE("Active状態->0:Non　1:Wait  2:Init  3:Stanby  4:Play\n");

   //TRACE("%d", state_->GetActive());



}


void NetWork::Send(Vector2 pos)
{
    // stateの送るPosの移動量
    state_->Send(pos);
}

Vector2 NetWork::Recv()
{
    return state_->Recv();
}

bool NetWork::Wait()
{

    TRACE("ホスト側：ゲスト待ち\n");
    return true;
}






bool NetWork::CloseNetWork()
{

    //state_->Updata()
    return false;
}

bool NetWork::SendMes(MesData& data)
{

    NetWorkSend(state_->GetHandle(), &data, sizeof(data));


    return true;
}

void NetWork::SendStanby(void)
{
    revStanby_ = true;
   // TRACE("準備終わって送信する\n");
    SendMes(mesData_);
    state_->SendStanby(revStanby_);

    
}

void NetWork::SendStart()
{
    revStanby_ = true;

    mesData_.type = MesType::GAME_START;
    SendMes(mesData_);
    state_->SendStanby(revStanby_);

}

bool NetWork::GetRevStanby()
{


    return state_->RecvStanby();
}

bool NetWork::SetNetWorkMode(NetWorkMode mode)
{
    switch (mode)
    {
    case NetWorkMode::OFFLINE:
        break;
    case NetWorkMode::HOST:
        state_ = std::make_unique<HostState>();
        break;
    case NetWorkMode::GEST:
        state_ = std::make_unique<GestState>();
        break;
    default:
#ifdef _DEBUG
        if (mode != NetWorkMode::OFFLINE)
        {
            //OFFLINEの異常エラー
            TRACE("ネットOFFLINEの指定エラー");
        }
        if (mode != NetWorkMode::HOST)
        {
            //HOSTの異常エラー
            TRACE("ネットHOSTの指定エラー");

        }
        if (mode != NetWorkMode::GEST)
        {
            //GESTの異常エラー
            TRACE("ネットGESTの指定エラー");

        }
        state_ = std::make_unique<NetWorkState>();

#endif
        break;
    }
    return true;
}

NetWorkMode NetWork::GetNetWorkMode(void)
{
    return state_->GetMode();
}

ActiveState NetWork::ConnectHost(IPDATA hostIP)
{
    return state_->ConnectHost(hostIP);
}

ActiveState NetWork::GetActiv(void)
{
    return state_->GetActive();
}

NetWork::NetWork()
{
    revStanby_ = false;
}

NetWork::~NetWork()
{
}
