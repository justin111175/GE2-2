#include "NetWork.h"
#include"HostState.h"
#include "GestState.h"
#include "../common/_debug/_DebugConOut.h"
#include "../common/_debug/_DebugConOut.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <bitset>

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


bool NetWork::CloseNetWork()
{

    //state_->Updata()
    return false;
}

bool NetWork::TEST(std::string file)
{
    std::ofstream ofp;
    std::ifstream ifp("map/tmx.dat");
    ofp.open(file);

    std::string stringData;

    int count = 0;

    if (!ifp.eof())
    {
        do
        {
            std::getline(ifp, stringData);
            ofp << stringData<<std::endl;

            if (ifp.eof())
            {

                break;
            }

        } while (stringData.find("data encoding") == std::string::npos);


        for (auto data : revtmx_)
        {
            UnionData test;
            test = data;
            for (int byteCnt = 0; byteCnt < 8; byteCnt++)
            {
                for (int bitCnt = 0; bitCnt <2; bitCnt++)
                {
                    std::ostringstream strNum;
                    strNum.clear();

                    strNum << ((test.cData[byteCnt] >> (4 * bitCnt)) & 0x0f);
                    ofp << strNum.str();

                    count++;

                    if (count % 21 != 0)
                    {
                        ofp << ",";
                    }
                    else
                    {
                        if ((count % (21 * 17)) == 0)
                        {
                            ofp << std::endl;
                            do
                            {
                                std::getline(ifp, stringData);
                                ofp << stringData << std::endl;

                                if (ifp.eof())
                                {
                                    return true;
                                }
       

                            } while (stringData.find("data encoding") == std::string::npos);
                            

                        }
                        else
                        {
                            ofp << ","<<std::endl;

                        }

                    }
                }
            }
        }

    }

    ofp.close();




    TRACE("回数　：%d\n", count);

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

                revtmx_.resize(data.data[0]);

                start = std::chrono::system_clock::now();

                
                TRACE("ホストからのTMXもらった%d\n", data.data[0]);

            }

            if (data.type == MesType::TMX_DATA)
            {
                revtmx_[data.sdata].iData[0] = data.data[0];
                revtmx_[data.sdata].iData[1] = data.data[1];

                TRACE(" ID : %d, DATA1 : %d, DATA2 : %d\n", data.sdata, data.data[0], data.data[1]);
                end = std::chrono::system_clock::now();

            }


            if (data.type == MesType::STANDY)
            {
                revStanby_ = true;

                std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>((end - start));

                TRACE("時間　：　%d秒\n", s.count());


                state_->SetActive(ActiveState::Stanby);

                TRACE("ホストからのスタンバイもらった\n");

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

                std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>((end - start));

                TRACE("時間　：　%d秒\n", s.count());

                state_->SetActive(ActiveState::Play);
            }

        }
    }



}

void NetWork::SetNetWork()
{

    auto ipData = IpNetwork.GetIp();

    TRACE("IPアドレス：%d.%d.%d.%d\n", ipData.d1, ipData.d2, ipData.d3, ipData.d4);
    TRACE("0:HOSTです\n1:GESTです\n/*2:前回の接続先です*/\n3:offline\n");


    IPDATA hostIP;					// ホストのIP

    int mode;

    do
    {
        std::cin >> mode;

        if (mode == 0)
        {
            TRACE("ゲスト接続待ち待機\n");

            IpNetwork.SetNetWorkMode(NetWorkMode::HOST);
            mode_ = UpdataMode::StartInit;


        }
        else if (mode == 1)
        {

            IpNetwork.SetNetWorkMode(NetWorkMode::GEST);
            TRACE("IPアドレスを入力してください\n");
            std::string ip, data;
            std::cin >> ip;
            //IPに入力された情報をホストIPに入れる
            //std::getline
            std::stringstream ipData;
            // stringstreamで文字列から数値を取り出す
            // coutのような使い方で代入する
            ipData << ip;

            auto GetIpNum = [&]() {
                std::getline(ipData, data, '.');
                return atoi(data.c_str());
            };
            hostIP.d1 = GetIpNum();
            hostIP.d2 = GetIpNum();
            hostIP.d3 = GetIpNum();
            hostIP.d4 = GetIpNum();

            if (IpNetwork.ConnectHost(hostIP) == ActiveState::Init)
            {
                TRACE("ホスト接続\n");
                mode_ = UpdataMode::StartInit;
                TRACE("ホストからの開始合図を待ち\n");



            }


            //if (IpNetwork.ConnectHost(hostIP)==ActiveState::Init)
            {
                //TRACE("ホスト接続\n");
                //mode_ = UpdataMode::StartInit;
                //TRACE("ホストからの開始合図を待ち\n");
                //
                //std::unique_ptr<FILE, decltype(&fclose)> fp(fopen("hostIP.txt", "wt"), fclose);
                //
                //fwrite(&ipData, sizeof(data), 1, &(*fp));
                //std::ofstream fp2("hostIP.txt", std::ios::trunc);
                //
                //fp2 << data;

            }
        }
        else if (mode == 2)
        {
            IpNetwork.SetNetWorkMode(NetWorkMode::OFFLINE);
        }
        else
        {

        }
    } while (mode < 0 || mode>2);



}

void NetWork::GetHostIp()
{
    if (IpNetwork.Updata())
    {
        if (IpNetwork.GetRevStanby())
        {

            mode_ = UpdataMode::StartInit;
        }
        //if (IpNetwork.GetActiv() == ActiveState::Stanby)
        //{
        //	mode_ = UpdataMode::Play;
        //}
    }

}

void NetWork::StartInit()
{
    IpNetwork.Updata();

    if (IpNetwork.GetNetWorkMode() == NetWorkMode::HOST)
    {
        if (IpNetwork.GetActiv() == ActiveState::Init)
        {

            tmx_->SendSize("map/testMap.tmx");
            tmx_->SendData();

            IpNetwork.SendStanby();

        }
        else if (IpNetwork.GetActiv() == ActiveState::Stanby)
        {
            IpNetwork.NetRev();


        }
        else if (IpNetwork.GetActiv() == ActiveState::Play)
        {

            tmx_->LoadTmx("map/testMap.tmx");

            TRACE("プレイモードに行く\n");
            mode_ = UpdataMode::Play;
        }



    }
    else
    {

        if (IpNetwork.GetActiv() == ActiveState::Init)
        {


            IpNetwork.NetRev();

            //IpNetwork.SendStart();
            //TRACE("プレイモードに行く\n");
            //mode_ = UpdataMode::Play;

        }
        if (IpNetwork.GetActiv() == ActiveState::Stanby)
        {
            if (IpNetwork.revStanby_)
            {

                IpNetwork.TEST("map/test.tmx");
                IpNetwork.SendStart();
            }


        }

        if (IpNetwork.GetActiv() == ActiveState::Play)
        {

            tmx_->LoadTmx("map/test.tmx");


            TRACE("プレイモードに行く\n");

            mode_ = UpdataMode::Play;

        }


    }


}

void NetWork::Play()
{
    IpNetwork.Updata();

    if (IpNetwork.GetActiv() == ActiveState::Wait)
    {
        mode_ = UpdataMode::SetNetWork;
    }

}

UpdataMode NetWork::GetMode()
{
    return mode_;
}

void NetWork::ThreadUpdata(void)
{

    while (ProcessMessage() == 0)
    {
        func_[mode_]();

    }



}

NetWork::NetWork()
{
    revStanby_ = false;


    tmx_ = std::make_unique<TmxObj>();


    mode_ = UpdataMode::SetNetWork;

    func_.try_emplace(UpdataMode::SetNetWork, std::bind(&NetWork::SetNetWork, this));
    func_.try_emplace(UpdataMode::GetHostIP, std::bind(&NetWork::GetHostIp, this));
    func_.try_emplace(UpdataMode::StartInit, std::bind(&NetWork::StartInit, this));
    func_.try_emplace(UpdataMode::Play, std::bind(&NetWork::Play, this));

    updata_ = std::thread(&NetWork::ThreadUpdata, this);

}

NetWork::~NetWork()
{
    updata_.detach();
}
