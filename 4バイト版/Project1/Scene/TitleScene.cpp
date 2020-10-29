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

	tmx_ = std::make_unique<TmxObj>();
	
//	tmx_->SendData("map/testMap.tmx");
	//tmx_->SendData("map/testMap.tmx");
	//tmx_->SendData("map/testMap.tmx");
	
	//tmx_->LoadTmx("map/test.tmx");

	//std::string myString = "Hello World";
	//for (std::size_t i = 0; i < myString.size(); ++i)
	//{
	//	std::cout << std::bitset<1>(myString.c_str()[i]) << std::endl;
	//}

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


	tmx_->Draw();
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
	IpNetwork.Updata();

	if (IpNetwork.GetNetWorkMode()==NetWorkMode::HOST)
	{
		if (IpNetwork.GetActiv() == ActiveState::Init)
		{

			tmx_->SendSize("map/testMap.tmx");
			tmx_->SendData("map/testMap.tmx");


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

void TitleScene::Play()
{
	IpNetwork.Updata();

	if (IpNetwork.GetActiv() == ActiveState::Wait)
	{
		mode_ = UpdataMode::SetNetWork;
	}

	//for (auto&& conType_ : conType())
	//{
	//	Ctl(conType_);
	//}
	//// もらったデータ（移動量）を足す
	//pos_ += IpNetwork.Recv();

	//// 移動量をとって、値を送る
	//if (pos_ != posOld_)
	//{
	//	Vector2 tmp = pos_ - posOld_;
	//	IpNetwork.Send(tmp);
	//}

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

std::vector<std::string> TitleScene::split(std::string& input, char delimiter)
{
	std::istringstream stream(input);
	std::string field;
	std::vector<std::string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}


void TitleScene::Tmx()
{






	//for (rapidxml::xml_node<>* node.= root_node->first_node("layer"); layer_node; layer_node = layer_node->next_sibling("layer")) {
	//	//TileLayer layer;
	//	//layer.name = layer_node->first_attribute("name")->value();
	//	////std::cout << std::endl << "Layer Name: " << layer.name << std::endl;

	//	//if (layer_node->first_node("properties") != 0) {
	//	//	for (rapidxml::xml_node<>* properties_node = layer_node->first_node("properties")->first_node("property"); properties_node; properties_node = properties_node->next_sibling()) {
	//	//		layer.property[properties_node->first_attribute("name")->value()] = properties_node->first_attribute("value")->value();
	//	//	}

	//	//	//std::cout << "Properties: " << std::endl;

	//	//	for (std::map<std::string, std::string>::iterator it = mapInfo.property.begin(); it != mapInfo.property.end(); ++it) {
	//	//		//  std::cout << "-> " << it->first << " : " << it->second << std::endl;
	//	//	}
	//	//}

	//	//rapidxml::xml_node<>* data_node = layer_node->first_node("data");
	//	//layer.data.encoding = data_node->first_attribute("encoding")->value();
	//	////std::cout << "Layer Encoding: " << layer.data.encoding << std::endl;

	//	//if (data_node->first_attribute("compression") > 0) {
	//	//	layer.data.compression = data_node->first_attribute("compression")->value();
	//	//	//  std::cout << "Layer Compression: " << layer.data.compression << std::endl;
	//	//}

	//	//layer.data.contents = data_node->value();
	//	////std::cout << "Layer Data: " << layer.data.contents << std::endl;

	//	//data_.try_emplace(layer.name, layer.data.contents);
	//	//tileLayer[layer.name] = layer;
	//}




	//TMX::Parser tmx;
	//tmx.load("testMap.tmx");

	////std::cout << "Map Version: " << tmx.mapInfo.version << std::endl;
	////std::cout << "Map Orientation: " << tmx.mapInfo.orientation << std::endl;
	//std::cout << "Map Width: " << tmx.mapInfo.width << std::endl;
	//std::cout << "Map Height: " << tmx.mapInfo.height << std::endl;
	//std::cout << "Tile Width: " << tmx.mapInfo.tileWidth << std::endl;
	//std::cout << "Tile Height: " << tmx.mapInfo.tileHeight<<"\n" << std::endl;


	//for (auto data : tmx.data_)
	//{
	//	std::cout <<"Layer Name : "<< data.first<< tmx.data_[data.first] << std::endl;

	//}

	//std::string line;

	//std::vector<std::string> strvec = split(tmx.data_["Iteam"], ',');

	//for (int i = 0; i < strvec.size(); i++) {
	//	printf("%d", stoi(strvec.at(i)));
	//}
	
}
