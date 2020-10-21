#include "TitleScene.h"
#include "Dxlib.h"
#include "SceneMng.h"
#include "../common/_debug/_DebugConOut.h"
#include "../common/_debug/_DebugDispOut.h"
#include <fstream>
#include <iostream>

#include <sstream>
#include"../common/Input/PadInput.h"
#include "../common/Input/KeyInput.h"

#include "../include/TMXParser.h"

TitleScene::TitleScene()
{
	screen_size_ = { 0,0 };

	mode_=UpdataMode::SetNetWork;

	func_.try_emplace(UpdataMode::SetNetWork, std::bind(&TitleScene::SetNetWork, this));
	func_.try_emplace(UpdataMode::GetHostIP, std::bind(&TitleScene::GetHostIp, this));
	func_.try_emplace(UpdataMode::StartInit, std::bind(&TitleScene::StartInit, this));
	func_.try_emplace(UpdataMode::Play, std::bind(&TitleScene::Play, this));

	Tmx();

	Init();

}

TitleScene::~TitleScene()
{
}

unique_Base TitleScene::Update(unique_Base own)
{
	Draw();



	func_[mode_]();

	IpNetwork.Updata();


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
			mode_ = mode_ = UpdataMode::StartInit;

			
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

				mode_ = UpdataMode::GetHostIP;

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

	//if (IpNetwork.GetActiv() == ActiveState::Stanby)
	//{
	//	mode_ = UpdataMode::GetHostIP;

	//}

	//switch (IpNetwork.GetNetWorkMode())
	//{
	//case NetWorkMode::OFFLINE:
	//	TRACE("OFFLINEです\n");
	//	break;
	//case NetWorkMode::HOST:
	//	TRACE("HOSTです\n");

	//	break;
	//case NetWorkMode::GEST:
	//	TRACE("GESTです\n");

	//	break;
	//default:
	//	break;
	//}
	
	//TRACE("Active状態->0:Non　1:Wait  2:Init  3:Stanby  4:Play\n");
	//TRACE("状態は%dです\n", IpNetwork.GetActiv());
	//if (IpNetwork.GetActiv() ==ActiveState::Init)
	//{
	//	TRACE("接続失敗\n");
	//}

}

void TitleScene::GetHostIp()
{



	if (IpNetwork.Updata())
	{
		if (IpNetwork.GetRevStanby())
		{
			TRACE("準備もらった\n");
			mode_ = UpdataMode::StartInit;
		}
		//if (IpNetwork.GetActiv() == ActiveState::Stanby)
		//{
		//	mode_ = UpdataMode::Play;
		//}
	}


}

void TitleScene::StartInit()
{
	if (IpNetwork.GetNetWorkMode()==NetWorkMode::HOST)
	{
		IpNetwork.Updata();
		if (IpNetwork.GetActiv() == ActiveState::Init)
		{
			IpNetwork.SendStanby();

		}
		else if (IpNetwork.GetActiv() == ActiveState::Stanby)
		{
			TRACE("プレイモードに行く\n");
			mode_ = UpdataMode::Play;

		}


	}
	else
	{

		IpNetwork.Updata();
		
		if (IpNetwork.GetActiv() == ActiveState::Init)
		{
			IpNetwork.SendStart();
			TRACE("プレイモードに行く\n");
			mode_ = UpdataMode::Play;

		}

	}



}

void TitleScene::Play()
{
	IpNetwork.Updata();

	if (IpNetwork.GetActiv() == ActiveState::Wait)
	{
		mode_ = UpdataMode::SetNetWork;
	}

	for (auto&& conType_ : conType())
	{
		Ctl(conType_);
	}
	// もらったデータ（移動量）を足す
	pos_ += IpNetwork.Recv();

	// 移動量をとって、値を送る
	if (pos_ != posOld_)
	{
		Vector2 tmp = pos_ - posOld_;
		IpNetwork.Send(tmp);
	}

}

void TitleScene::ChipInit()
{

	_dataBase.resize((__int64)CHIP_MAX_X * CHIP_MAX_Y);
	for (size_t no = 0; no < CHIP_MAX_Y; no++)
	{
		_data.emplace_back(&_dataBase[no * CHIP_MAX_Y]);
	}


	// csvファイルを読み込む
	int type = NULL;
	int y = 0;
	int x = 0;

	FILE* fp = NULL;
	fopen_s(&fp, "csv/reiya1.csv", "rb");
	while (fscanf_s(fp, "%d", &type) != EOF)
	{
		_dataBase[x] = static_cast<MapState>(type);
		x++;
	}


	chipState_.try_emplace(Layer::BG, new ChipMng());



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

void TitleScene::Tmx()
{

	TMX::Parser tmx;
	tmx.load("testMap.tmx");

	std::cout << "Map Version: " << tmx.mapInfo.version << std::endl;
	std::cout << "Map Orientation: " << tmx.mapInfo.orientation << std::endl;
	std::cout << "Map Width: " << tmx.mapInfo.width << std::endl;
	std::cout << "Map Height: " << tmx.mapInfo.height << std::endl;
	std::cout << "Tile Width: " << tmx.mapInfo.tileWidth << std::endl;
	std::cout << "Tile Height: " << tmx.mapInfo.tileHeight<<"\n" << std::endl;





	//for (std::map<std::string, std::string>::iterator it = tmx.mapInfo.property.begin(); it != tmx.mapInfo.property.end(); ++it) {
	//	std::cout << "-> " << it->first << " : " << it->second << std::endl;
	//}
	//std::cout << std::endl;
	//for (int i = 0; i < tmx.tilesetList.size(); i++) {
	//	std::cout << "Tileset[ First GID: " << tmx.tilesetList[i].firstGID << " Source: " << tmx.tilesetList[i].source << " ]" << std::endl;
	//}
	//std::cout << std::endl;
	for (std::map<std::string, TMX::Parser::TileLayer>::iterator it = tmx.tileLayer.begin(); it != tmx.tileLayer.end(); ++it) {
		std::cout << std::endl;
		std::cout << "Tile Layer Name: " << it->first << std::endl;
		std::cout << "Tile Layer Visibility: " << tmx.tileLayer[it->first].visible << std::endl;
		std::cout << "Tile Layer Opacity: " << tmx.tileLayer[it->first].opacity << std::endl;
		std::cout << "Tile Layer Properties:" << std::endl;
		if (tmx.tileLayer[it->first].property.size() > 0) {
			for (std::map<std::string, std::string>::iterator it2 = tmx.tileLayer[it->first].property.begin(); it2 != tmx.tileLayer[it->first].property.end(); ++it2) {
				std::cout << "-> " << it2->first << " : " << it2->second << std::endl;
			}
		}
		std::cout << "Tile Layer Data Encoding: " << tmx.tileLayer[it->first].data.encoding << std::endl;
		if (tmx.tileLayer[it->first].data.compression != "none") {
			std::cout << "Tile Layer Data Compression: " << tmx.tileLayer[it->first].data.compression << std::endl;
		}
		std::cout << "Tile Layer Data Contents: " << tmx.tileLayer[it->first].data.contents << std::endl;
		//TRACE("%u", tmx.tileLayer[it->first].data.contents);



	}
	//for (std::map<std::string, TMX::Parser::ObjectGroup>::iterator it = tmx.objectGroup.begin(); it != tmx.objectGroup.end(); ++it) {
	//	std::cout << std::endl;
	//	std::cout << "Object Group Name: " << it->first << std::endl;
	//	std::cout << "Object Group Color: " << tmx.objectGroup[it->first].color << std::endl;
	//	std::cout << "Object Group Visibility: " << tmx.objectGroup[it->first].visible << std::endl;
	//	std::cout << "Object Group Opacity: " << tmx.objectGroup[it->first].opacity << std::endl;
	//	std::cout << "Object Group Properties:" << std::endl;
	//	if (tmx.objectGroup[it->first].property.size() > 0) {
	//		for (std::map<std::string, std::string>::iterator it2 = tmx.objectGroup[it->first].property.begin(); it2 != tmx.objectGroup[it->first].property.end(); ++it2) {
	//			std::cout << "-> " << it2->first << " : " << it2->second << std::endl;
	//		}
	//	}
	//	for (std::map<std::string, TMX::Parser::Object>::iterator it2 = tmx.objectGroup[it->first].object.begin(); it2 != tmx.objectGroup[it->first].object.end(); ++it2) {
	//		std::cout << std::endl;
	//		if (it2->second.name != "") { std::cout << "Object Name: " << it2->first << std::endl; }
	//		if (it2->second.type != "") { std::cout << "Object Type: " << tmx.objectGroup[it->first].object[it2->first].type << std::endl; }
	//		std::cout << "Object Position X: " << tmx.objectGroup[it->first].object[it2->first].x << std::endl;
	//		std::cout << "Object Position Y: " << tmx.objectGroup[it->first].object[it2->first].y << std::endl;
	//		std::cout << "Object Width: " << tmx.objectGroup[it->first].object[it2->first].width << std::endl;
	//		std::cout << "Object Height: " << tmx.objectGroup[it->first].object[it2->first].height << std::endl;
	//		if (it2->second.gid != 0) { std::cout << "Object Tile GID: " << tmx.objectGroup[it->first].object[it2->first].gid << std::endl; }
	//		std::cout << "Object Visible: " << tmx.objectGroup[it->first].object[it2->first].visible << std::endl;
	//	}
	//}

	//for (std::map<std::string, TMX::Parser::ImageLayer>::iterator it = tmx.imageLayer.begin(); it != tmx.imageLayer.end(); ++it) {
	//	std::cout << std::endl;
	//	std::cout << "Image Layer Name: " << it->first << std::endl;
	//	std::cout << "Image Layer Visibility: " << tmx.imageLayer[it->first].visible << std::endl;
	//	std::cout << "Image Layer Opacity: " << tmx.imageLayer[it->first].opacity << std::endl;
	//	std::cout << "Image Layer Properties:" << std::endl;
	//	if (tmx.imageLayer[it->first].property.size() > 0) {
	//		for (std::map<std::string, std::string>::iterator it2 = tmx.imageLayer[it->first].property.begin(); it2 != tmx.imageLayer[it->first].property.end(); ++it2) {
	//			std::cout << "-> " << it2->first << " : " << it2->second << std::endl;
	//		}
	//	}
	//	std::cout << "Image Layer Source: " << tmx.imageLayer[it->first].image.source << std::endl;
	//	std::cout << "Image Layer Transparent Color: " << tmx.imageLayer[it->first].image.transparencyColor << std::endl;
	//}
}
