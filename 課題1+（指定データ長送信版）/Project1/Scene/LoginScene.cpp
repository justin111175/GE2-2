#include "LoginScene.h"
#include"Transition.h"
#include <ctype.h> 
#include "../common/_debug/_DebugConOut.h"

#include <fstream>
#include <sstream>
#include "SceneMng.h"

unique_Base LoginScene::Update(unique_Base own)
{
	func_[mode_]();


	if (mode_== UpdataMode::Play)
	{
		return std::make_unique<Transition>(std::move(own), SceneChange::Login_GO_Game);

	}

	Draw();


	return std::move(own);
}

LoginScene::LoginScene()
{
	IpImageMng.GetID("1", "image/1.jpg", { 800,600 }, { 1,1 });

	

	mean_.try_emplace(LoginMean_ID::HOST, "オンライン【ホスト】");
	mean_.try_emplace(LoginMean_ID::GEST, "オンライン【ゲスト】");
	mean_.try_emplace(LoginMean_ID::GEST_BEFORE, "オンライン【前回ゲスト】");
	mean_.try_emplace(LoginMean_ID::NON, "オフライン");

	func_.try_emplace(UpdataMode::SetNetWork, std::bind(&LoginScene::SetNetWork, this));
	func_.try_emplace(UpdataMode::GetHostIP, std::bind(&LoginScene::GetHostIp, this));
	func_.try_emplace(UpdataMode::StartInit, std::bind(&LoginScene::StartInit, this));
	func_.try_emplace(UpdataMode::Play, std::bind(&LoginScene::Play, this));
	
	GetDrawScreenSize(&screen_size_.x,&screen_size_.y);

	ipData = IpNetwork.GetIp();
	meanID_ = LoginMean_ID::HOST;
	mode_ = UpdataMode::SetNetWork;
	testFlag_ = false;
	//Draw();
	//IpNetwork.updata_;

}

LoginScene::~LoginScene()
{
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
					break;
				case LoginMean_ID::NON:
					IpNetwork.SetNetWorkMode(NetWorkMode::OFFLINE);
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

	IpNetwork.SetNetWorkMode(NetWorkMode::GEST);
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

	if (IpNetwork.ConnectHost(hostIP) == ActiveState::Init)
	{
		std::ofstream ofp("ini/hostIP.txt");
		ofp << ip;
		ofp.close();

		TRACE("ホスト接続\n");
		mode_ = UpdataMode::StartInit;
		TRACE("ホストからの開始合図を待ち\n");

	}


}

void LoginScene::SetNetWork()
{
	std::ifstream ifp("ini/hostIP.txt");

	ipData = IpNetwork.GetIp();
	
	Ctl(conType::Key);



	//	else if (mode == 2 && ifp.is_open())
	//	{
	//		IpNetwork.SetNetWorkMode(NetWorkMode::GEST);

	//		hostIP.d1 = stringIp_[0];
	//		hostIP.d2 = stringIp_[1];
	//		hostIP.d3 = stringIp_[2];
	//		hostIP.d4 = stringIp_[3];

	//		if (IpNetwork.ConnectHost(hostIP) == ActiveState::Init)
	//		{
	//			TRACE("ホスト接続\n");
	//			mode_ = UpdataMode::StartInit;
	//			TRACE("ホストからの開始合図を待ち\n");
	//		}

	//	}
	//	else if (mode == 3)
	//	{
	//		IpNetwork.SetNetWorkMode(NetWorkMode::OFFLINE);

	//	}
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
			IpNetwork.start = std::chrono::system_clock::now();

			IpNetwork.tmx_->SendTmx();

			IpNetwork.SendStanby();

		}
		else if (IpNetwork.GetActiv() == ActiveState::Stanby)
		{
			IpNetwork.NetRev();

		}
		else if (IpNetwork.GetActiv() == ActiveState::Play)
		{
			IpNetwork.tmx_->LoadTmx("map/testMap.tmx");

			TRACE("プレイモードに行く\n");
			mode_ = UpdataMode::Play;
		}
	}
	else
	{

		if (IpNetwork.GetActiv() == ActiveState::Init)
		{
			IpNetwork.NetRev();

		}
		if (IpNetwork.GetActiv() == ActiveState::Stanby)
		{
			if (IpNetwork.revStanby_)
			{

				IpNetwork.RevTmx("map/test.tmx");
				IpNetwork.SendStart();
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
	IpNetwork.Updata();
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

	ScreenFlip();
}

void LoginScene::DrawOwn()
{



	//DrawGraph(0, 0, IpImageMng.GetID("1")[0], true);

	SetFontSize(50);
	DrawString(0, 0, "Login  Scene", 0xFFFFFF,0x000000 );
	


	SetFontSize(40);

	DrawFormatString(300,250,0xffffff,"IPアドレス：%d.%d.%d.%d\n", ipData.d1, ipData.d2, ipData.d3, ipData.d4);
	SetFontSize(30);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	DrawBox(300, 320 + static_cast<int>(meanID_) * 50, 660, 360 + static_cast<int>(meanID_) * 50, 0xFFFFFF, true);


	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);



	for (auto i : mean_)
	{
		DrawString(300, 320 + static_cast<int>(i.first) * 50, i.second, 0xffffff, true);

	}




}

