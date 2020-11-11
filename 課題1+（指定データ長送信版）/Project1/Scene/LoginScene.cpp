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

	

	mean_.try_emplace(LoginMean_ID::HOST, "�I�����C���y�z�X�g�z");
	mean_.try_emplace(LoginMean_ID::GEST, "�I�����C���y�Q�X�g�z");
	mean_.try_emplace(LoginMean_ID::GEST_BEFORE, "�I�����C���y�O��Q�X�g�z");
	mean_.try_emplace(LoginMean_ID::NON, "�I�t���C��");

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
	TRACE("�Q�X�g�ڑ��҂��ҋ@\n");
	IpNetwork.SetNetWorkMode(NetWorkMode::HOST);
	mode_ = UpdataMode::StartInit;

}

void LoginScene::GEST()
{
	std::ifstream ifp("ini/hostIP.txt");


	IPDATA hostIP;					// �z�X�g��IP

	IpNetwork.SetNetWorkMode(NetWorkMode::GEST);
	TRACE("IP�A�h���X����͂��Ă�������\n");
	std::string ip, data;
	KeyInputSingleCharString(200, 200, 30, Name, false);

	ip = Name;
	//IP�ɓ��͂��ꂽ�����z�X�gIP�ɓ����
	std::stringstream ipData;
	// stringstream�ŕ����񂩂琔�l�����o��
	// cout�̂悤�Ȏg�����ő������
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

		TRACE("�z�X�g�ڑ�\n");
		mode_ = UpdataMode::StartInit;
		TRACE("�z�X�g����̊J�n���}��҂�\n");

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
	//			TRACE("�z�X�g�ڑ�\n");
	//			mode_ = UpdataMode::StartInit;
	//			TRACE("�z�X�g����̊J�n���}��҂�\n");
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

			TRACE("�v���C���[�h�ɍs��\n");
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

			TRACE("�v���C���[�h�ɍs��\n");

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

	DrawFormatString(300,250,0xffffff,"IP�A�h���X�F%d.%d.%d.%d\n", ipData.d1, ipData.d2, ipData.d3, ipData.d4);
	SetFontSize(30);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	DrawBox(300, 320 + static_cast<int>(meanID_) * 50, 660, 360 + static_cast<int>(meanID_) * 50, 0xFFFFFF, true);


	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);



	for (auto i : mean_)
	{
		DrawString(300, 320 + static_cast<int>(i.first) * 50, i.second, 0xffffff, true);

	}




}

