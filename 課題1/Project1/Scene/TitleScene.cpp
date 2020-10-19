#include "TitleScene.h"
#include "Dxlib.h"
#include "SceneMng.h"
#include "../InterNet/NetWork.h"
#include "../common/_debug/_DebugConOut.h"
#include "../common/_debug/_DebugDispOut.h"
#include <iostream>

#include <sstream>
#include"../common/Input/PadInput.h"
#include "../common/Input/KeyInput.h"


TitleScene::TitleScene()
{
	screen_size_ = { 0,0 };

	mode_=UpdataMode::SetNetWork;

	func_.try_emplace(UpdataMode::SetNetWork, std::bind(&TitleScene::SetNetWork, this));



	Init();
}

TitleScene::~TitleScene()
{
}

unique_Base TitleScene::Update(unique_Base own)
{
	Draw();


	do
	{
		func_[mode_]();

	} while (IpNetwork.GetActiv() == ActiveState::Init);





	return std::move(own);
}


void TitleScene::Init(void)
{
	GetDrawScreenSize(&screen_size_.x,&screen_size_.y);
	controller.try_emplace(conType::Key, std::make_unique<KeyInput>());
	controller[conType::Key]->SetUp();

	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());
	controller[conType::Pad]->SetUp();
}

void TitleScene::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	DrawString(pos_.x, pos_.y, "TEST", 0xFFFFFF);


	ScreenFlip();
}

void TitleScene::SetNetWork(void)
{


	auto ipData = IpNetwork.GetIp();

	TRACE("IP�A�h���X�F%d,%d,%d,%d\n", ipData.d1, ipData.d2, ipData.d3, ipData.d4);
	TRACE("0:HOST�ł�\n1:GEST�ł�\n2:OFFLINE�ł�\n");


	IPDATA hostIP;					// �z�X�g��IP
	
	int mode;

	do
	{
		std::cin >> mode;

		if (mode == 0)
		{
			IpNetwork.SetNetWorkMode(NetWorkMode::HOST);
		}
		else if (mode == 1)
		{

			IpNetwork.SetNetWorkMode(NetWorkMode::GEST);
			TRACE("IP�A�h���X����͂��Ă�������\n");
			std::string ip, data;
			std::cin >> ip;
			//IP�ɓ��͂��ꂽ�����z�X�gIP�ɓ����
			//std::getline
			std::stringstream ipData;
			// stringstream�ŕ����񂩂琔�l�����o��
			// cout�̂悤�Ȏg�����ő������
			ipData << ip;

			auto GetIpNum = [&]() {
				std::getline(ipData, data, ',');
				return atoi(data.c_str());
			};
			hostIP.d1 = GetIpNum();
			hostIP.d2 = GetIpNum();
			hostIP.d3 = GetIpNum();
			hostIP.d4 = GetIpNum();


			IpNetwork.ConnectHost(hostIP);
		}
		else if (mode == 2)
		{
			IpNetwork.SetNetWorkMode(NetWorkMode::OFFLINE);
		}
		else
		{

		}
	} while (mode < 0 || mode>2);


	//switch (IpNetwork.GetNetWorkMode())
	//{
	//case NetWorkMode::OFFLINE:
	//	TRACE("OFFLINE�ł�\n");
	//	break;
	//case NetWorkMode::HOST:
	//	TRACE("HOST�ł�\n");

	//	break;
	//case NetWorkMode::GEST:
	//	TRACE("GEST�ł�\n");

	//	break;
	//default:
	//	break;
	//}
	
	TRACE("Active���->0:Non�@1:Wait  2:Init  3:Stanby  4:Play\n");
	TRACE("��Ԃ�%d�ł�\n", IpNetwork.GetActiv());
	if (IpNetwork.GetActiv() ==ActiveState::Init)
	{
		TRACE("�ڑ����s\n");
	}

}

void TitleScene::GetHostIp()
{

}

void TitleScene::StartInit()
{


}

void TitleScene::Play()
{
	IpNetwork.Updata();

	for (auto&& conType_ : conType())
	{
		Ctl(conType_);
	}
	// ��������f�[�^�i�ړ��ʁj�𑫂�
	pos_ += IpNetwork.Recv();

	// �ړ��ʂ��Ƃ��āA�l�𑗂�
	if (pos_ != posOld_)
	{
		Vector2 tmp = pos_ - posOld_;
		IpNetwork.Send(tmp);
	}

}

void TitleScene::Ctl(conType input)
{
	posOld_ = pos_;
	(*controller[input])();
	for (auto data : controller[input]->GetCntData())
	{
		if (data.second[static_cast<int>(Trg::Now)])
		{
			switch (data.first)
			{
			case InputID::Left:
				pos_.x--;
				break;
			case InputID::Right:
				pos_.x++;
				break;
			case InputID::Up:
				pos_.y--;
				break;
			case InputID::Down:
				pos_.y++;
				break;
			default:
				break;
			}
		}
	}


}
