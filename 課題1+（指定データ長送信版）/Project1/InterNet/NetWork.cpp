#include "NetWork.h"
#include"HostState.h"
#include "GestState.h"
#include "../common/_debug/_DebugConOut.h"
#include "../common/_debug/_DebugConOut.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <bitset>
#include "OffLine.h"

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

bool NetWork::SendMes(MesType type, MesPacket& mesPacket, int handle)
{
    MesPacket data;

    Header header;
    UnionData uniondata_;
    header.mesheader.sendID = 0;

    int tmpCount = intSendCount_ / 4;
    auto sendSize = min(tmpCount, mesPacket.size());
    // もしTYPEはサイズなら
    if (type == MesType::TMX_SIZE)
    {
        start = std::chrono::system_clock::now();
        // パゲットヘッダー追加（length送信）
        MesPacket data;
        SetHeader(Header{ type,0,0,1 }, data);
        NetWorkSend(state_->GetHandle(), data.data(), data.size() * 4);
        
        // 初期化TMXの最後送信
        data.clear();
        
        uniondata_.cData[0] = 21;
        uniondata_.cData[1] = 17;
        uniondata_.cData[2] = 4;

        data.insert(data.end(), { /*static_cast<unsigned int>(mesPacket.size())*/uniondata_ });
        NetWorkSend(state_->GetHandle(), data.data(), data.size() * 4);

        TRACE("TMX_SIZE送信\n");
        TRACE("SIZE : %dです\n", mesPacket.size());

    }

    // もしTYPEはデータなら
    //if (type == MesType::TMX_DATA)
    //{

    //    // 1回送信する大きさが全部パゲットデータより大きい場合（1回送信）
    //    if (sendSize >= mesPacket.size())
    //    {
    //        SetHeader(Header{ type,0,1,static_cast<unsigned int>(mesPacket.size()) }, mesPacket);
    //        NetWorkSend(state_->GetHandle(), mesPacket.data(), mesPacket.size() * 4);
    //        return true;
    //    }
    //    else
    //    {
    //        do
    //        {
    //            data.clear();
    //            // 1回送信分を初期化する
    //            for (auto i = 0; i < sendSize; i++)
    //            {
    //                // 1回送信分が残るパゲットのサイズより大きいなら入らない（パゲットのデータがない）
    //                if (mesPacket.size()>i)
    //                {
    //                    data.insert(data.end(),mesPacket[i]);

    //                }
    //            }
    //            // 1回送信分が残るパゲットのサイズより大きいなら、nextがない
    //            if (mesPacket.size()<= sendSize)
    //            {
    //                header = { type,0, header.mesheader.sendID,static_cast<unsigned int>(data.size()) };
    //            }
    //            else
    //            {
    //                header = { type,1,header.mesheader.sendID,static_cast<unsigned int>(data.size()) };

    //            }
    //            TRACE("TMX_DATA送信\n");
    //            TRACE("sendID : %d\n", header.mesheader.sendID);
    //            TRACE("データ量 : %d\n", data.size());

    //            SetHeader(header, data);
    //            header.mesheader.sendID++;

    //            NetWorkSend(state_->GetHandle(), data.data(), data.size() * 4);

    //            mesPacket.erase(mesPacket.begin(), mesPacket.begin()+ (data.size()-2));

    //        } while (mesPacket.size() > 0);
    //        end = std::chrono::system_clock::now();

    //        TRACE("送信完了\n");
    //        return true;

    //    }

    //}

    if (type == MesType::POS)
    {

            SetHeader(Header{ type,0,0,4 }, mesPacket);

            NetWorkSend(state_->GetHandle(), mesPacket.data(), mesPacket.size() * 4);
            //TRACE("送信した\n");
            return true;
        


    }    
    
    if (type == MesType::SET_BOMB)
    {


        SetHeader(Header{ type,0,0,7 }, mesPacket);

        NetWorkSend(state_->GetHandle(), mesPacket.data(), mesPacket.size() * 4);
        return true;
        


    }

    if (type == MesType::DEATH)
    {

        SetHeader(Header{ type,0,0,1 }, mesPacket);

        NetWorkSend(state_->GetHandle(), mesPacket.data(), mesPacket.size() * 4);

        return true;
        


    }
    return false;
}

void NetWork::GetSetting(const char* setting)
{

    std::string string;
    std::ifstream ifp(setting);
    // 全部回す
    if (!ifp.eof())
    {
        // 1行ずつ確認、'"'をとって
        while (getline(ifp, string,'"'))
        {
            // byteじゃない場合はいる
            if (string.find("byte") == std::string::npos)
            {
                // 値とる
                intSendCount_ = atoi(string.c_str());
                // 
                if (getline(ifp, string, '"'))
                {
                    break;

                }

            }
        }
    }
}



void NetWork::SendStanby(void)//host
{
    revStanby_ = true;

    if (SendMes(MesType::STANDY))
    {
        TRACE("STANDYを送る\n");
        state_->SetActive(ActiveState::Stanby);
        TRACE("STANDY送信完了\n");

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
        state_ = std::make_unique<OffLine>();
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
   // std::lock_guard<std::mutex> lock(mtx);

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

            tmx_->SendTmx();

            SendStanby();

        }
        else if (GetActiv() == ActiveState::Stanby)
        {

        }
        else if (GetActiv() == ActiveState::Play)
        {
            tmx_->LoadTmx("map/ObjTest.tmx");
            
            TRACE("プレイモードに行く\n");
            mode_ = UpdataMode::Play;
        }
    }
    else
    {

        if (GetActiv() == ActiveState::Init)
        {

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

int NetWork::GetHandle()
{
    return state_->GetHandle();
}

MesPacket NetWork::GetPacket(int id)
{       
    std::lock_guard<std::mutex> lock(mtx);

    MesPacket data_;

    if (VecID_.size())
    {
        for (auto& data : VecID_)
        {
            if (data.data())
            {
                if (data[0].iData == id)
                {
                    data_ = data;
                    data[0].iData = -1;
                }
            }
            else
            {
                data = MesPacket{ UnionData{0} };
                data[0].iData = -1;
            }

        }
    }

    return data_;
}

MesPacket NetWork::GetPacket(MesType type)
{
    std::lock_guard<std::mutex> lock(mtx);

    MesPacket data_;


    for (auto& data : TypePacket_)
    {
        if (data.first == type)
        {
            data_ = data.second;
            data.second[0].iData = -1;
        }

    }



    return data_;

}



void NetWork::EraserPac()
{
    std::lock_guard<std::mutex> lock(mtx);

    auto itr = std::remove_if(VecID_.begin(), VecID_.end(), [](MesPacket& data) {return data[0].iData == -1; });

    if (itr!= VecID_.end())
    {
        VecID_.erase(itr, VecID_.end());
    }


    auto test = std::remove_if(TypePacket_.begin(), TypePacket_.end(), [](RevPacket& data) {return data.second[0].iData == -1; });

    if (test != TypePacket_.end())
    {
        TypePacket_.erase(test, TypePacket_.end());
    }


}

void NetWork::RevInit()
{
    revFunc_.try_emplace(MesType::COUNT_DOWN, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        //TypePacket_[0].
        TRACE("COUNT_DOWNゲット\n");
        });

    revFunc_.try_emplace(MesType::ID, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        //TypePacket_[0].
        TRACE("ID : %d , 数 : %d\n", revPacket_[0].iData, revPacket_[1].iData);
        });

    revFunc_.try_emplace(MesType::START_TIME, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        TRACE("開始時間\n");
        });


    revFunc_.try_emplace(MesType::TMX_SIZE, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        start = std::chrono::system_clock::now();
        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        //do
        //{
        //    // パゲットがない場合新しい情報をとって、確保する
        //    if (revPacket_.size() == 0)
        //    {
        //        NetWorkRecv(handle, &revMesHeader_, sizeof(revMesHeader_));
        //        revPacket_.resize(revMesHeader_.length);
        //        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);
        //    }
        //    // 確保したデータを後ろに追加する
        //    for (int i = 0; i < revPacket_.size(); i++)
        //    {
        //        revtmx_.insert(revtmx_.end(), { revPacket_[i] });
        //    }
        //    // もし次がある場合、パゲットを消す
        //    if (revMesHeader_.next != 0)
        //    {
        //        revPacket_.clear();
        //    }
        //    // パゲットがない場合、ループする
        //} while (revPacket_.size() == 0);

        //TypePacket_.emplace_back(revMesHeader_.type, revPacket_);
        TRACE("TMX ゲット\n");
        
        });
    revFunc_.try_emplace(MesType::TMX_DATA, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // 最初のパゲットを確保して、とる
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        do
        {
            // パゲットがない場合新しい情報をとって、確保する
            if (revPacket_.size() == 0)
            {
                NetWorkRecv(handle, &revMesHeader_, sizeof(revMesHeader_));
                revPacket_.resize(revMesHeader_.length);
                NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);
            }
            // 確保したデータを後ろに追加する
            for (int i = 0; i < revPacket_.size(); i++)
            {
                revtmx_.insert(revtmx_.end(), { revPacket_[i] });
            }
            // もし次がある場合、パゲットを消す
            if (revMesHeader_.next != 0)
            {
                revPacket_.clear();
            }
            // パゲットがない場合、ループする
        } while (revPacket_.size() == 0);

        end = std::chrono::system_clock::now();



    });

    revFunc_.try_emplace(MesType::STANDY, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        std::lock_guard<std::mutex> lock(mtx);

        revStanby_ = true;
        //std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>((end - start));
        //TRACE("時間　：　%d秒\n", s.count());
        state_->SetActive(ActiveState::Stanby);
        TRACE("ホストからのスタンバイもらった\n");



    });

    revFunc_.try_emplace(MesType::GAME_START, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        end = std::chrono::system_clock::now();
        //std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>((end - start));
        //TRACE("時間　：　%d秒\n", s.count());
        state_->SetActive(ActiveState::Play);



    });

    revFunc_.try_emplace(MesType::POS, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

       VecID_.emplace_back(revPacket_);



    });

    revFunc_.try_emplace(MesType::SET_BOMB, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        TRACE("TEST");
        });

    revFunc_.try_emplace(MesType::DEATH, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        //TypePacket_[0].
        TRACE("TEST");
        });

    revFunc_.try_emplace(MesType::LOST, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        TRACE("開始時間\n");
        });

}

void NetWork::ThreadUpdata(void)
{

    while (ProcessMessage() == 0)
    {
        if (!state_)
        {
            continue;
        }
        if (state_->Updata())
        {
            RevInit();
            break;

        }
    }

    while (ProcessMessage() == 0&&GetLostNetWork()==-1)
    {

        int handle = state_->GetHandle();

        MesHeader revMesHeader_;
        MesPacket revPacket_;

        if (sizeof(revMesHeader_) <= GetNetWorkDataLength(handle))
        {
            // 受信
            NetWorkRecv(handle, &revMesHeader_, sizeof(revMesHeader_));

            if (revMesHeader_.type < MesType::MAX)
            {
                revFunc_[revMesHeader_.type](handle, revMesHeader_, revPacket_);
            }
        }
        
    }



}

NetWork::NetWork()
{
    revStanby_ = false;


    tmx_ = std::make_unique<TmxObj>();


    // 1回送信する大きさをとる
    GetSetting("ini/setting.txt");

    updata_ = std::thread(&NetWork::ThreadUpdata, this);

}

NetWork::~NetWork()
{
    updata_.detach();
}
