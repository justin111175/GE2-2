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





    return state_->Updata();



}


void NetWork::Send(Vector2 pos)
{
    // state�̑���Pos�̈ړ���
    state_->Send(pos);
}

Vector2 NetWork::Recv()
{
    return state_->Recv();
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
            //OFFLINE�ُ̈�G���[
            TRACE("�l�b�gOFFLINE�̎w��G���[");
        }
        if (mode != NetWorkMode::HOST)
        {
            //HOST�ُ̈�G���[
            TRACE("�l�b�gHOST�̎w��G���[");

        }
        if (mode != NetWorkMode::GEST)
        {
            //GEST�ُ̈�G���[
            TRACE("�l�b�gGEST�̎w��G���[");

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
}

NetWork::~NetWork()
{
}
