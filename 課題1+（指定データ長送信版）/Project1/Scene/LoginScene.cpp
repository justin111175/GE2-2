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
		_dbgDrawFormatString(200, 600, 0xFFFFFF, "�ҋ@���");

	}
	else
	{
		_dbgDrawFormatString(200, 600, 0xFFFFFF, "�J�n�܂Ł@�F�@%d�b", IpNetwork.time_);

	}
	


	func_[mode_]();


	if (mode_== UpdataMode::Play)
	{
		return std::make_unique<Transition>(std::move(own), SceneChange::Login_GO_Game);

	}



	return std::move(own);
}

LoginScene::LoginScene()
{
	IpImageMng.GetID("1", "image/1.jpg", { 800,600 }, { 1,1 });

	std::ifstream ifp("ini/hostIP.txt");



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
	}

		mean_.try_emplace(LoginMean_ID::HOST, "�I�����C���y�z�X�g�z");
		mean_.try_emplace(LoginMean_ID::GEST, "�I�����C���y�Q�X�g�z");
		mean_.try_emplace(LoginMean_ID::GEST_BEFORE, "�I�����C���y�O��Q�X�g�z");
		mean_.try_emplace(LoginMean_ID::NON, "�I�t���C��");

	func_.try_emplace(UpdataMode::SetNetWork, std::bind(&LoginScene::SetNetWork, this));
	func_.try_emplace(UpdataMode::GetHostIP, std::bind(&LoginScene::GetHostIp, this));
	func_.try_emplace(UpdataMode::StartInit, std::bind(&LoginScene::StartInit, this));
	func_.try_emplace(UpdataMode::Play, std::bind(&LoginScene::Play, this));
	
	GetDrawScreenSize(&screen_size_.x,&screen_size_.y);

	meanID_ = LoginMean_ID::HOST;
	mode_ = UpdataMode::SetNetWork;
	testFlag_ = false;
	ipData = IpNetwork.GetIp();






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


		// �p�Q�b�g���Ȃ��ꍇ�V���������Ƃ��āA�m�ۂ���
		//if (revPacket_.size() == 0)
		//{
		//	NetWorkRecv(handle, &revMesHeader_, sizeof(revMesHeader_));
		//	revPacket_.resize(revMesHeader_.length);
		//	NetWorkRecv(handle, revPacket_.data(), revMesHeader_.length * 4);
		//}
		// �m�ۂ����f�[�^�����ɒǉ�����



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
					B_GEST();

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
	
	IpNetwork.SetNetWorkMode(NetWorkMode::GEST);

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

void LoginScene::B_GEST()
{
	IpNetwork.SetNetWorkMode(NetWorkMode::GEST);

	IPDATA hostIP;

	hostIP.d1 = stringIp_[0];
	hostIP.d2 = stringIp_[1];
	hostIP.d3 = stringIp_[2];
	hostIP.d4 = stringIp_[3];

	if (IpNetwork.ConnectHost(hostIP) == ActiveState::Init)
	{
		TRACE("�z�X�g�ڑ�\n");
		mode_ = UpdataMode::StartInit;
		TRACE("�z�X�g����̊J�n���}��҂�\n");
	}
}

void LoginScene::SetNetWork()
{
	std::ifstream ifp("ini/hostIP.txt");

	Ctl(conType::Key);

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

			IpNetwork.tmx_->LoadTmx("map/ObjTest.tmx");

			TRACE("�v���C���[�h�ɍs��\n");
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
			//IpNetwork.NetRev();

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

			TRACE("�v���C���[�h�ɍs��\n");

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

