#include "LoginScene.h"
#include"Transition.h"
#include <ctype.h> 
#include "../common/_debug/_DebugConOut.h"
#include "../common/_debug/_DebugDispOut.h"
#include <fstream>
#include <sstream>
#include "SceneMng.h"

unique_Base LoginScene::Update(unique_Base own)
{
	Draw();

	GetCount_Down();



	if (!IpNetwork.countFlag_)
	{
		_dbgDrawFormatString(200, 600, 0xFFFFFF, "待機状態");

	}
	else
	{
		_dbgDrawFormatString(200, 600, 0xFFFFFF, "開始まで　：　%d秒", IpNetwork.time_);

	}
	


	func_[mode_]();


	if (mode_== UpdataMode::Play)
	{
		return std::make_unique<Transition>(std::move(own), SceneChange::Login_GO_Game);

	}



	return std::move(own);
}

void LoginScene::GetHostIP()
{
	std::ifstream ifp("ini/hostIP.txt");



	std::string getlineString_;




	if (ifp.is_open())
	{
		if (!ifp.eof())
		{
			while (std::getline(ifp, getlineString_))
			{
				VectorIP_.emplace_back(getlineString_);
				//stringIp_.emplace_back(stringInt(getlineString_));
				if (ifp.eof())
				{
					break;
				}
			}
		}
	}


}

LoginScene::LoginScene()
{
	IpImageMng.GetID("1", "image/1.jpg", { 800,600 }, { 1,1 });



	GetHostIP();
	hostIpCnt_ = 0;

	mean_.try_emplace(LoginMean_ID::HOST, "オンライン【ホスト】");
	mean_.try_emplace(LoginMean_ID::GEST, "オンライン【ゲスト】");
	mean_.try_emplace(LoginMean_ID::GEST_BEFORE, "オンライン【前回ゲスト】");
	mean_.try_emplace(LoginMean_ID::NON, "オフライン");

	func_.try_emplace(UpdataMode::SetNetWork, std::bind(&LoginScene::SetNetWork, this));
	func_.try_emplace(UpdataMode::GetHostIP, std::bind(&LoginScene::GetHostIp, this));
	func_.try_emplace(UpdataMode::StartInit, std::bind(&LoginScene::StartInit, this));
	func_.try_emplace(UpdataMode::Play, std::bind(&LoginScene::Play, this));
	
	GetDrawScreenSize(&screen_size_.x,&screen_size_.y);

	meanID_ = LoginMean_ID::HOST;
	mode_ = UpdataMode::SetNetWork;
	testFlag_ = false;
	ipData = IpNetwork.GetIp();

	intOut_ = IN_OUT::Out;




}

LoginScene::~LoginScene()
{
}

void LoginScene::GetCount_Down()
{
	MesPacket data_;
	data_ = IpNetwork.GetNewPacket(MesType::COUNT_DOWN_ROOM);

	if (data_.size())
	{	
		UnionTime time = { std::chrono::system_clock::now() };
		time.iData[0] = data_[0].iData;
		time.iData[1] = data_[1].iData;
		IpNetwork.start = time.start_+ std::chrono::milliseconds(15000);
		IpNetwork.countFlag_ = true;
	}



}

void LoginScene::GetTmx()
{

	MesPacket data_;
	data_ = IpNetwork.GetNewPacket(MesType::TMX_SIZE);
	if (data_.size())
	{
		for (int i = 0; i < data_.size(); i++)
		{
			IpNetwork.revtmx_.insert(IpNetwork.revtmx_.end(), { data_[i] });
		}
	}


		// パゲットがない場合新しい情報をとって、確保する
		//if (revPacket_.size() == 0)
		//{
		//	NetWorkRecv(handle, &revMesHeader_, sizeof(revMesHeader_));
		//	revPacket_.resize(revMesHeader_.length);
		//	NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);
		//}
		// 確保したデータを後ろに追加する



	//if (data_.size())
	//{
	//	time.iData[0] = data_[0].iData;
	//	time.iData[1] = data_[1].iData;

	//}
}





void LoginScene::Ctl(conType input)
{

	(*controller[input])();
	for (auto data : controller[input]->GetCntData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			switch (data.first)
			{
			case InputID::Left:
				break;
			case InputID::Right:
				break;
			case InputID::Up:
				if (meanID_ > LoginMean_ID::HOST)
				{
					meanID_ = (LoginMean_ID)(static_cast<int>(meanID_) - 1);

				}
				else
				{
					meanID_ = LoginMean_ID::NON;
				}

				break;
			case InputID::Down:
				if (meanID_ < LoginMean_ID::NON)
				{
					meanID_ = (LoginMean_ID)(static_cast<int>(meanID_) + 1);

				}
				else
				{
					meanID_ = LoginMean_ID::HOST;
				}
				break;

			case InputID::Btn1:
				switch (meanID_)
				{
				case LoginMean_ID::HOST:
					HOST();
					break;
				case LoginMean_ID::GEST:

					GEST();


					break;
				case LoginMean_ID::GEST_BEFORE:
					intOut_ = IN_OUT::In;

					break;
				case LoginMean_ID::NON:
					IpNetwork.SetNetWorkMode(NetWorkMode::OFFLINE);
					mode_ = UpdataMode::Play;
					break;
				default:
					break;
				}
			default:
				break;
			}
		}
	}

}

void LoginScene::InCtl(conType input)
{
	(*controller[input])();
	for (auto data : controller[input]->GetCntData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			switch (data.first)
			{
			case InputID::Left:
				break;
			case InputID::Right:
				break;
			case InputID::Up:
				if (hostIpCnt_ > 0)
				{
					hostIpCnt_--;
				}
				else
				{
					hostIpCnt_ = VectorIP_.size()-1;
				}

				break;
			case InputID::Down:
				if (hostIpCnt_ >= VectorIP_.size()-1)
				{
					hostIpCnt_ = 0;
				}
				else
				{
					hostIpCnt_++;
				}

				break;

			case InputID::Btn1:
				intOut_ = IN_OUT::Out;

				B_GEST();

				break;
			case InputID::ESC:
				intOut_ = IN_OUT::Out;
				hostIpCnt_ = 0;
				break;
			default:
				break;
			}
		}
	}


}

void LoginScene::HOST()
{
	TRACE("ゲスト接続待ち待機\n");
	IpNetwork.SetNetWorkMode(NetWorkMode::HOST);
	mode_ = UpdataMode::StartInit;

}

void LoginScene::GEST()
{
	std::ifstream ifp("ini/hostIP.txt");


	IPDATA hostIP;					// ホストのIP

	TRACE("IPアドレスを入力してください\n");
	std::string ip, data;
	KeyInputSingleCharString(200, 200, 30, Name, false);

	ip = Name;
	//IPに入力された情報をホストIPに入れる
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
	
	IpNetwork.SetNetWorkMode(NetWorkMode::GEST);

	if (IpNetwork.ConnectHost(hostIP) == ActiveState::Init)
	{	
		int cnt_ = 0;
		std::ofstream ofp("ini/hostIP.txt");

		ofp << ip<<std::endl;
		cnt_++;

		if (VectorIP_.size() > 0)
		{
			while (cnt_ < 5)
			{
				if (VectorIP_.size() > cnt_-1)
				{
					ofp << VectorIP_[cnt_-1]<<std::endl;

				}
				cnt_++;

			}
		}

		ofp.close();

		TRACE("ホスト接続\n");

		mode_ = UpdataMode::StartInit;
		TRACE("ホストからの開始合図を待ち\n");

	}


}

void LoginScene::B_GEST()
{
	IpNetwork.SetNetWorkMode(NetWorkMode::GEST);

	IPDATA hostIP;
	std::string ip, data;

	ip =VectorIP_[hostIpCnt_];
	std::stringstream ipData;


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
}

void LoginScene::SetNetWork()
{
	std::ifstream ifp("ini/hostIP.txt");

	if (intOut_ == IN_OUT::Out)
	{
		Ctl(conType::Key);

	}
	else
	{
		InCtl(conType::Key);
	}




}

void LoginScene::GetHostIp()
{
	if (IpNetwork.Updata())
	{
		if (IpNetwork.GetRevStanby())
		{

			mode_ = UpdataMode::StartInit;
		}
	}
}

void LoginScene::StartInit()
{
	IpNetwork.Updata();

	if (IpNetwork.GetNetWorkMode() == NetWorkMode::HOST)
	{

		if (IpNetwork.GetActiv() == ActiveState::Init)
		{
			MesPacket data;
			UnionData uniondata_;

			for (auto handle : IpNetwork.GetHandleAll())
			{
				data.clear();
				uniondata_.iData = handle.second * 5;
				data.insert(data.end(), uniondata_);

				uniondata_.iData = IpNetwork.GetHandleAll().size() + 1;
				data.insert(data.end(), uniondata_);
				
				IpNetwork.SendMesAll(MesType::ID,data,handle.first);

			}

			IpNetwork.tmx_->SendTmxSize();

			IpNetwork.tmx_->SendTmxData();

			IpNetwork.SendMesAll(MesType::STANDY_HOST);

		}
		else if (IpNetwork.GetActiv() == ActiveState::Stanby)
		{
			IpNetwork.Updata();
		}
		else if (IpNetwork.GetActiv() == ActiveState::Play)
		{

			IpNetwork.tmx_->LoadTmx("map/testMap.tmx");

			TRACE("プレイモードに行く\n");
			mode_ = UpdataMode::Play;


			UnionTime time_ = { std::chrono::system_clock::now() };
			MesPacket data_;

			UnionData unionD;

			unionD.iData = time_.iData[0];
			data_.insert(data_.end(), unionD);
			unionD.iData = time_.iData[1];
			data_.insert(data_.end(), unionD);

			IpNetwork.TypePacket_.emplace_back(MesType::COUNT_DOWN_GAME, data_);

			for (auto handle : IpNetwork.GetHandleAll())
			{
				IpNetwork.SendMesAll(MesType::COUNT_DOWN_GAME, data_, handle.first);

			}

		}
	}
	else
	{

		if (IpNetwork.GetActiv() == ActiveState::Init)
		{
			if (IpNetwork.countFlag_)
			{
				IpNetwork.end = std::chrono::system_clock::now();

				IpNetwork.time_ = std::chrono::duration_cast<std::chrono::milliseconds>(IpNetwork.start - IpNetwork.end).count() / 1000;

			}

		}
		if (IpNetwork.GetActiv() == ActiveState::Stanby)
		{
			if (IpNetwork.revStanby_)
			{
				IpNetwork.RevTmx("map/test.tmx");
				IpNetwork.SendMesAll(MesType::STANDY_GEST);
			}


		}

		if (IpNetwork.GetActiv() == ActiveState::Play)
		{
			IpNetwork.tmx_->LoadTmx("map/test.tmx");

			TRACE("プレイモードに行く\n");

			mode_ = UpdataMode::Play;

		}
	}

}

void LoginScene::Play()
{
	testFlag_ = true;
	if (IpNetwork.GetActiv() == ActiveState::Wait)
	{
		//mode_ = UpdataMode::SetNetWork;
	}
}

void LoginScene::Draw()
{	

	SetDrawScreen(screenID_);
	ClsDrawScreen();

	DrawOwn();

}

void LoginScene::DrawOwn()
{

	DrawBox(0, 0, IpSceneMng.screenSize.x, IpSceneMng.screenSize.y, 0xcc81ff, true);

	SetFontSize(40);

	DrawFormatString(300,250,0xffffff,"IPアドレス：%d.%d.%d.%d\n", ipData.d1, ipData.d2, ipData.d3, ipData.d4);
	SetFontSize(30);


	if (intOut_ == IN_OUT::Out)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);

		DrawBox(300, 320 + static_cast<int>(meanID_) * 50, 660, 360 + static_cast<int>(meanID_) * 50, 0xFFFFFF, true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		for (auto i : mean_)
		{
			DrawString(300, 320 + static_cast<int>(i.first) * 50, i.second, 0xffffff, true);

		}
	}
	else
	{


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
		DrawBox(300, 320+ hostIpCnt_*40, 660, 350+ hostIpCnt_*40, 0xFFFFFF, true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		for (int i = 0; i < VectorIP_.size(); i++)
		{
			DrawFormatString(300, 320+i*40, 0x000000, "%d. %s",i+1,VectorIP_[i].c_str());

		}
	}








}

