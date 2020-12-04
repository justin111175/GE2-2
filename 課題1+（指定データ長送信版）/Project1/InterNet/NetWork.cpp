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






bool NetWork::SendMesAll(MesType type)
{
    MesPacket data;
    UnionData uniondata_;

    if (type == MesType::ID)
    {           

        for (auto handle : state_->GetHandleAll())
        {
            data.clear();
            uniondata_.iData = handle.second * 5;
            data.insert(data.end(), uniondata_);

            uniondata_.iData = state_->GetHandleAll().size() + 1;
            data.insert(data.end(), uniondata_);

            SetHeader(Header{ type,0,0,2 }, data);

            NetWorkSend(handle.first, data.data(), data.size() * 4);
        }
        
        return true;


    }
    if (type == MesType::TMX_SIZE)
    {

        MesPacket data;
        UnionData uniondata_;


        uniondata_.cData[0] = 21;
        uniondata_.cData[1] = 17;
        uniondata_.cData[2] = 4;

        data.insert(data.end(), { uniondata_ });
        SetHeader(Header{ type,0,0,1 }, data);
        for (auto handle : GetHandleAll())
        {
            NetWorkSend(handle.first, data.data(), data.size() * 4);
        }
        return true;
    }

    if (type == MesType::STANDY_HOST)
    {
        for (auto handle : state_->GetHandleAll())
        {


            SetHeader(Header{ type,0,0,0 }, data);

            NetWorkSend(handle.first, data.data(), data.size() * 4);
        }
        state_->SetActive(ActiveState::Stanby);

        return true;


    }
    if (type == MesType::STANDY_GEST)
    {
        for (auto handle : state_->GetHandleAll())
        {


            SetHeader(Header{ type,0,0,0 }, data);

            NetWorkSend(handle.first, data.data(), data.size() * 4);
        }
        state_->SetActive(ActiveState::Play);

        return true;


    }

    return false;

}

bool NetWork::SendMesAll(MesType type, MesPacket& mesPacket)
{
    MesPacket data;
    Header header;

    if (type == MesType::TMX_DATA)
    {

        int tmpCount = intSendCount_ / 4;
        auto sendSize = min(tmpCount, mesPacket.size());
        header.mesheader.sendID = 0;


        // 1回送信する大きさが全部パゲットデータより大きい場合（1回送信）
        if (sendSize >= mesPacket.size())
        {
            SetHeader(Header{ type,0,1,static_cast<unsigned int>(mesPacket.size()) }, mesPacket);
            for (auto handle : GetHandleAll())
            {
                NetWorkSend(handle.first, mesPacket.data(), mesPacket.size() * 4);

            }
            return true;
        }
        else
        {
            do
            {
                data.clear();
                // 1回送信分を初期化する
                for (auto i = 0; i < sendSize; i++)
                {
                    // 1回送信分が残るパゲットのサイズより大きいなら入らない（パゲットのデータがない）
                    if (mesPacket.size() > i)
                    {
                        data.insert(data.end(), mesPacket[i]);

                    }
                }
                // 1回送信分が残るパゲットのサイズより大きいなら、nextがない
                if (mesPacket.size() <= sendSize)
                {
                    header = { type,0, header.mesheader.sendID,static_cast<unsigned int>(data.size()) };
                }
                else
                {
                    header = { type,1,header.mesheader.sendID,static_cast<unsigned int>(data.size()) };

                }
                TRACE("TMX_DATA送信\n");
                TRACE("sendID : %d\n", header.mesheader.sendID);
                TRACE("データ量 : %d\n", data.size());

                SetHeader(header, data);
                header.mesheader.sendID++;

                for (auto handle : GetHandleAll())
                {
                    NetWorkSend(handle.first, data.data(), data.size() * 4);

                }

                mesPacket.erase(mesPacket.begin(), mesPacket.begin() + (data.size() - 2));

            } while (mesPacket.size() > 0);

            TRACE("送信完了\n");
            return true;
        }
    }

    if (type == MesType::POS)
    {

        SetHeader(Header{ type,0,0,4 }, mesPacket);


        for (auto handle : GetHandleAll())
        {
            NetWorkSend(handle.first, mesPacket.data(), mesPacket.size() * 4);
        }
        return true;

    }

    if (type == MesType::SET_BOMB)
    {


        SetHeader(Header{ type,0,0,7 }, mesPacket);
        for (auto handle : GetHandleAll())
        {
            NetWorkSend(handle.first, mesPacket.data(), mesPacket.size() * 4);
        }
        return true;



    }
    if (type == MesType::DEATH)
    {

        SetHeader(Header{ type,0,0,1 }, mesPacket);
        for (auto handle : GetHandleAll())
        {
            NetWorkSend(handle.first, mesPacket.data(), mesPacket.size() * 4);
        }
        return true;



    }
    return false;
}

bool NetWork::SendMesAll(MesType type, MesPacket& mesPacket,int handle)
{
    MesPacket data;
    Header header;

    if (type == MesType::COUNT_DOWN_ROOM)
    {
        SetHeader(Header{ type,0,0,2 }, mesPacket);

        NetWorkSend(handle, mesPacket.data(), mesPacket.size() * 4);
        return true;

    }

    if (type == MesType::ID)
    {

        SetHeader(Header{ type,0,0,2 }, mesPacket);

        NetWorkSend(handle, mesPacket.data(), mesPacket.size() * 4);
        return true;
    }



 

    if (type == MesType::COUNT_DOWN_GAME)
    {

        SetHeader(Header{ type,0,0,2 }, mesPacket);

        NetWorkSend(handle, mesPacket.data(), mesPacket.size() * 4);
        

        return true;

    }







    return false;
}



ListInt NetWork::GetHandleAll()
{
    return state_->GetHandleAll();
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

bool NetWork::SetActive(ActiveState active)
{
    state_->SetActive(active);
    return true;
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

MesPacket NetWork::GetNewPacket(MesType type)
{
    std::lock_guard<std::mutex> lock(mtx);

    MesPacket data_;


    for (auto& data : TypePacket_)
    {
        if (data.first == type)
        {
            data_ = data.second;
            data.first = MesType::NON;
        }
    }
    return data_;
    




}

void NetWork::GetSavePacket(MesType type)
{
    std::lock_guard<std::mutex> lock(mtx);
    MesPacket data_;

    for (auto& data : TypePacket_)
    {
        if (data.first == type)
        {
            data_ = data.second;
            saveData_.try_emplace(type);
            saveData_[type].emplace_back(data_);
            data.first = MesType::NON;
        }
    }


}




void NetWork::EraserPac()
{
    std::lock_guard<std::mutex> lock(mtx);

    auto itr = std::remove_if(VecID_.begin(), VecID_.end(), [](MesPacket& data) {return data[0].iData == -1; });

    if (itr!= VecID_.end())
    {
        VecID_.erase(itr, VecID_.end());
    }


    auto test = std::remove_if(TypePacket_.begin(), TypePacket_.end(), [](RevPacket& data) {return data.first == MesType::NON; });

    if (test != TypePacket_.end())
    {
        TypePacket_.erase(test, TypePacket_.end());
    }


}


void NetWork::RevInit()
{
    revFunc_.try_emplace(MesType::COUNT_DOWN_ROOM, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        //TypePacket_[0].
        TRACE("COUNT_DOWN_ROOMゲット\n");
        });

    revFunc_.try_emplace(MesType::ID, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        TRACE("ID : %d , 数 : %d\n", revPacket_[0].iData, revPacket_[1].iData);
        });

    revFunc_.try_emplace(MesType::COUNT_DOWN_GAME, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        TRACE("COUNT_DOWN_GAMEゲット\n");
        });


    revFunc_.try_emplace(MesType::TMX_SIZE, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);
        TRACE("TMX_SIZE ゲット\n");
        
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

        TRACE("TMX_SIZE ゲット　：　%d\n", revtmx_.size());


    });

    revFunc_.try_emplace(MesType::STANDY_HOST, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        std::lock_guard<std::mutex> lock(mtx);

        revStanby_ = true;
        //std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>((end - start));
        //TRACE("時間　：　%d秒\n", s.count());
        state_->SetActive(ActiveState::Stanby);
        TRACE("STANDY_HOSTゲット\n");



    });

    revFunc_.try_emplace(MesType::STANDY_GEST, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        end = std::chrono::system_clock::now();
        //std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>((end - start));
        //TRACE("時間　：　%d秒\n", s.count());
        state_->SetActive(ActiveState::Play);
        TRACE("STANDY_HOSTゲット\n");



    });

    revFunc_.try_emplace(MesType::POS, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

       VecID_.emplace_back(revPacket_);
       //TypePacket_.emplace_back(revMesHeader_.type, revPacket_);



    });

    revFunc_.try_emplace(MesType::SET_BOMB, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        TRACE("SET_BOMBゲット\n");
        });

    revFunc_.try_emplace(MesType::DEATH, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        //TypePacket_[0].
        TRACE("DEATH＿IDゲット\n");
        });

    revFunc_.try_emplace(MesType::LOST, [&](int handle, MesHeader& revMesHeader_, MesPacket& revPacket_) {

        // ヘッダーの部分確保する
        revPacket_.resize(revMesHeader_.length);
        NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);

        TypePacket_.emplace_back(revMesHeader_.type, revPacket_);

        TRACE("LOST＿IDゲット\n");
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

        if (GetNetWorkMode() == NetWorkMode::HOST)
        {
            state_->Updata();

            if (GetHandle() != -1)
            {
                countFlag_ = true;
                IpNetwork.end = std::chrono::system_clock::now();
                IpNetwork.time_ = std::chrono::duration_cast<std::chrono::milliseconds>(IpNetwork.start - IpNetwork.end).count() / 1000;

                if (time_ <= 0)
                {
                    state_->SetActive(ActiveState::Init);
                    RevInit();
                    break;
                }
            }
        }
        else if (GetNetWorkMode() == NetWorkMode::GEST)
        {

            if (state_->Updata())
            {
                RevInit();
                break;

            }
        }

    }

    while (ProcessMessage() == 0&&GetLostNetWork()==-1)
    {
       EraserPac();

        if (GetNetWorkMode() == NetWorkMode::HOST)
        {

            MesHeader revMesHeader_;
            MesPacket revPacket_;


            for (auto handle : GetHandleAll())
            {
                if (sizeof(revMesHeader_) <= GetNetWorkDataLength(handle.first))
                {
                    // 受信
                    NetWorkRecv(handle.first, &revMesHeader_, sizeof(revMesHeader_));

                    if (revMesHeader_.type < MesType::MAX)
                    {
                        revFunc_[revMesHeader_.type](handle.first, revMesHeader_, revPacket_);
                    }
                }
            }


        }
        else
        {
            int handle = state_->GetHandle();

            MesHeader revMesHeader_;
            MesPacket revPacket_;

            state_->Updata();

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



}

NetWork::NetWork()
{
    revStanby_ = false;

    countFlag_ = false;

    tmx_ = std::make_unique<TmxObj>();


    // 1回送信する大きさをとる
    GetSetting("ini/setting.txt");

    updata_ = std::thread(&NetWork::ThreadUpdata, this);
}

NetWork::~NetWork()
{
    updata_.detach();
}
