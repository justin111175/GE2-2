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

    return false;
}

void NetWork::SetHeader(Header header, MesPacket& mesP_)
{
    mesP_.insert(mesP_.begin(), { header.header[1] });
    mesP_.insert(mesP_.begin(), { header.header[0] });
}

bool NetWork::RevTmx(std::string file)//gest
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
            for (int byteCnt = 0; byteCnt < 4; byteCnt++)
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


    return false;

    TRACE("回数　：%d\n", count);

}


bool NetWork::SendMes(MesType type)
{
	
    MesPacket data;

    SetHeader(Header{ type,0,0,0 }, data);

    if (NetWorkSend(state_->GetHandle(), data.data(), data.size() * 4) == 0)
    {
        return true;

    }
    return false;



}

bool NetWork::SendMes(MesType type, MesPacket& mesPacket)
{
    MesPacket data;

    Header header;
    header.mesheader.sendID = 0;

    int tmpCount = intSendCount_ / 4;
    auto sendSize = min(tmpCount, mesPacket.size());

    if (type == MesType::TMX_SIZE)
    {
        MesPacket data;

        SetHeader(Header{ type,0,0,1 }, data);

        NetWorkSend(state_->GetHandle(), data.data(), data.size() * 4);
        
        data.clear();
        data.insert(data.end(), { static_cast<unsigned int>(mesPacket.size()) });
        NetWorkSend(state_->GetHandle(), data.data(), data.size() * 4);


    }

    if (type == MesType::TMX_DATA)
    {
        if (sendSize >= mesPacket.size())
        {
            SetHeader(Header{ type,0,1,static_cast<unsigned int>(mesPacket.size()) }, mesPacket);
            NetWorkSend(state_->GetHandle(), mesPacket.data(), mesPacket.size() * 4);
            return true;
        }
        else
        {
            do
            {
                data.clear();

                for (auto i = 0; i < sendSize; i++)
                {
                    if (mesPacket.size()>i)
                    {
                        data.insert(data.end(),mesPacket[i]);

                    }
                }

                if (mesPacket.size()<= sendSize)
                {
                    header = { type,0, header.mesheader.sendID,static_cast<unsigned int>(data.size()) };
                }
                else
                {
                    header = { type,1,header.mesheader.sendID,static_cast<unsigned int>(data.size()) };

                }
                SetHeader(header, data);
                header.mesheader.sendID++;

                NetWorkSend(state_->GetHandle(), data.data(), data.size() * 4);
                

                mesPacket.erase(mesPacket.begin(), mesPacket.begin()+ (data.size()-2));



            } while (mesPacket.size() > 0);

            return true;




        }




    }








    return false;
}

void NetWork::GetSetting(const char* setting)
{

    std::string string;
    std::ifstream ifp(setting);
    if (!ifp.eof())
    {
        while (getline(ifp, string,'"'))
        {
            if (string.find("byte") == std::string::npos)
            {
                intSendCount_ = atoi(string.c_str());
                if (getline(ifp, string, '"'))
                {
                    break;

                }

            }
        }
    }
}

void NetWork::Send(std::string file)
{
    NetWorkSend(state_->GetHandle(), &file, sizeof(file));



}

void NetWork::SendStanby(void)//host
{
    revStanby_ = true;

    

    if (SendMes(MesType::STANDY))
    {
        TRACE("ゲーム開始合図を送る\n");
        state_->SetActive(ActiveState::Stanby);

    }



    
}

void NetWork::SendStart()   //gset
{

    
    if (SendMes(MesType::GAME_START))
    {
        TRACE("ゲーム開始合図を送る\n");

    }
    
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
        MesHeader data;
        //Header data1;
        MesPacket mesPacket_;

        if (sizeof(data) <= GetNetWorkDataLength(handle))
        {

            NetWorkRecv(handle, &data, sizeof(data));

            if (data.type == MesType::TMX_SIZE)
            {

                mesPacket_.resize(data.length);

                NetWorkRecv(handle, mesPacket_.data(), data.length * 4);
                //revtmx_.resize(mesPacket_[0].iData);

                TRACE("TMX_SIZE : %d\n", data.length * 4);

            }

           // NetWorkRecv(handle, &data1, sizeof(data1));

            if (data.type == MesType::TMX_DATA)
            {
                mesPacket_.resize(data.length);

                NetWorkRecv(handle, mesPacket_.data(), data.length * 4);     

                do
                {
                    if (mesPacket_.size() == 0)
                    {
                        NetWorkRecv(handle, &data, sizeof(data));
                        mesPacket_.resize(data.length);

                        NetWorkRecv(handle, mesPacket_.data(), data.length * 4);
                    }


       
                    for (int i = 0; i < mesPacket_.size(); i++)
                    {
                        revtmx_.insert(revtmx_.end(), { mesPacket_[i] });
                    }
                    

                    if (data.next !=0)
                    {
                        mesPacket_.clear();

                    }


                } while (mesPacket_.size()==0);


                //for (int i = 0; i < mesPacket_.size(); i++)
                //{
                //    revtmx_[i].iData = mesPacket_[i].iData;
                //}
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
        MesHeader data;
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
    std::ifstream ifp("ini/hostIP.txt");

    auto ipData = GetIp();


    TRACE("IPアドレス：%d.%d.%d.%d\n", ipData.d1, ipData.d2, ipData.d3, ipData.d4);
    std::vector<int> stringIp_;
    
    std::string getlineString_;

    auto stringInt = [](std::string string) {
    
        return atoi(string.c_str());
    
    };


    if (ifp.is_open())
    {
        if (!ifp.eof())
        {
            while (std::getline(ifp, getlineString_, '.'))
            {
                stringIp_.emplace_back(stringInt(getlineString_));
                if (ifp.eof())
                {
                    break;
                }
            }
        }

        TRACE("0:オンライン【ホスト】\n1:オンライン【ゲスト】\n2:オンライン【ゲスト:%d.%d.%d.%d】\n3:offline\n", 
            stringIp_[0], stringIp_[1], stringIp_[2], stringIp_[3]);

    }
    else
    {
        TRACE("0:HOSTです\n1:GESTです\n\n3:offline\n");

    }


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

            if (ConnectHost(hostIP) == ActiveState::Init)
            {
                std::ofstream ofp("ini/hostIP.txt");
                ofp << ip;
                ofp.close();



                TRACE("ホスト接続\n");
                mode_ = UpdataMode::StartInit;
                TRACE("ホストからの開始合図を待ち\n");

            }

        }
        else if (mode == 2&& ifp.is_open())
        {
           SetNetWorkMode(NetWorkMode::GEST);

            hostIP.d1 = stringIp_[0];
            hostIP.d2 = stringIp_[1];
            hostIP.d3 = stringIp_[2];
            hostIP.d4 = stringIp_[3];

            if (ConnectHost(hostIP) == ActiveState::Init)
            {
                TRACE("ホスト接続\n");
                mode_ = UpdataMode::StartInit;
                TRACE("ホストからの開始合図を待ち\n");
            }

        }
        else if(mode==3)
        {
            SetNetWorkMode(NetWorkMode::OFFLINE);

        }
    } while (mode < 0 || mode>2);



}

void NetWork::GetHostIp()
{
    if (Updata())
    {
        if (GetRevStanby())
        {

            mode_ = UpdataMode::StartInit;
        }
    }

}

void NetWork::StartInit()
{
   Updata();

    if (GetNetWorkMode() == NetWorkMode::HOST)
    {
        if (GetActiv() == ActiveState::Init)
        {
            start = std::chrono::system_clock::now();

            //tmx_->SendTmxSize("map/testMap.tmx");
            //TRACE("サイズ送信完了\n");
            tmx_->SendTmx();
            TRACE("データ送信完了\n");


            SendStanby();
            TRACE("ステンバーイ送信完了\n");


        }
        else if (GetActiv() == ActiveState::Stanby)
        {
            NetRev();
            TRACE("ステンバーイチェックしている\n");

        }
        else if (GetActiv() == ActiveState::Play)
        {
            tmx_->LoadTmx("map/testMap.tmx");

            TRACE("プレイモードに行く\n");
            mode_ = UpdataMode::Play;
        }
    }
    else
    {

        if (GetActiv() == ActiveState::Init)
        {


            NetRev();


        }
        if (GetActiv() == ActiveState::Stanby)
        {
            if (revStanby_)
            {

               RevTmx("map/test.tmx");
               SendStart();
            }


        }

        if (GetActiv() == ActiveState::Play)
        {

            tmx_->LoadTmx("map/test.tmx");


            TRACE("プレイモードに行く\n");

            mode_ = UpdataMode::Play;

        }


    }


}

void NetWork::Play()
{
    Updata();

    if (GetActiv() == ActiveState::Wait)
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

    //tmx_->SendTmxSize("map/testMap.tmx");
    updata_ = std::thread(&NetWork::ThreadUpdata, this);

}

NetWork::~NetWork()
{
    updata_.detach();
}
