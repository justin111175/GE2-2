#include "NetWork.h"
#include"HostState.h"
#include "GestState.h"
#include "../common/_debug/_DebugConOut.h"
#include "../common/_debug/_DebugConOut.h"
#include <sstream>
#include <fstream>
#include <iostream>
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


bool NetWork::CloseNetWork()
{

    //state_->Updata()
    return false;
}

void NetWork::TEST(std::string file)
{
    std::ofstream test;
    
    test.open(file, std::ios::out | std::ios::binary);


    for (auto i : revtmx_)
    {
        //if (i != -1)
        {

            if (i <= 0)
            {
                test.close();
                break;

            }

            if (i <= 127)
            {
                break;

            }
            else
            {
                test.write(reinterpret_cast<char*>(&i), sizeof(int));

            }

        }

    }



}

bool NetWork::SendMes(MesData& data)
{

    NetWorkSend(state_->GetHandle(), &data, sizeof(data));


    return true;
}

void NetWork::Send(std::string file)
{
    NetWorkSend(state_->GetHandle(), &file, sizeof(file));



}

void NetWork::SendStanby(void)//host
{
    revStanby_ = true;
   // TRACE("準備終わって送信する\n");
    SendMes(mesData_);
    state_->SendStanby(revStanby_);
    
    start = std::chrono::system_clock::now();

    state_->SetActive(ActiveState::Stanby);


    
}

void NetWork::SendStart()   //gset
{

    mesData_.type = MesType::GAME_START;
    SendMes(mesData_);
    TRACE("ゲーム開始合図を送る\n");
    
    state_->SetActive(ActiveState::Play);
    





}

bool NetWork::GetRevStanby()
{
    
    
    //if (mesData_.type == MesType::TMX_DATA)
    //{
    //    revtmx_.resize();
    //}

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

void NetWork::NetRev()
{

    if (NetWorkMode::GEST == GetNetWorkMode())
    {
        int handle=state_->GetHandle();
        MesData data;
        if (sizeof(data) <= GetNetWorkDataLength(handle))
        {
            NetWorkRecv(handle, &data, sizeof(data));
            
            if (data.type == MesType::TMX_SIZE)
            {
                //if (revtmx_.size() > data.data[0])
                {
                    revtmx_.resize(data.data[0]);

                    start = std::chrono::system_clock::now();

                }
                TRACE("ホストからのTMXもらった%d\n", data.data[0]);

            }

            if (data.type == MesType::TMX_DATA)
            {

               //if (revtmx_.size() >= data.data[0])
                {
                    //revtmx_.at(data.data[0]) = data.data[1];

                    revtmx_.at(retest) = data.data[0];
                    retest++;
                    TRACE(" ID : %d,DATA1 : %d\n", retest,data.data[0]);

                    revtmx_.at(retest) = data.data[1];
                    retest++;

                    TRACE(" ID : %d,DATA2 : %d\n", retest, data.data[1]);


                }
            }


            if (data.type == MesType::STANDY)
            {
                revStanby_ = true;

                end = std::chrono::system_clock::now();
                std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>((end - start));

                TRACE("時間　：　%d秒\n", s.count());


                state_->SetActive(ActiveState::Stanby);

                TRACE("ホストからのスタンバイもらった\n")

                    //if (revStanby_)
                    //{
                    //    TEST("map/test.tmx");
                    //    SendStart();
                    //}

            }

        }
    }
    else
    {
        int handle = state_->GetHandle();
        MesData data;
        if (sizeof(data) <= GetNetWorkDataLength(handle))
        {
            NetWorkRecv(handle, &data, sizeof(data));


            if (data.type == MesType::GAME_START)
            {

                end = std::chrono::system_clock::now();
                std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>((end - start));

                TRACE("時間　：　%d秒\n", s.count());

                state_->SetActive(ActiveState::Play);
            }

        }
    }



}

NetWork::NetWork()
{
    revStanby_ = false;
}

NetWork::~NetWork()
{
}
