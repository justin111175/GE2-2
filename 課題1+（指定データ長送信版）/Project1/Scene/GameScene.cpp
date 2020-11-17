#include "GameScene.h"
#include "../common/_debug/_DebugDispOut.h"

void GameScene::Draw()
{	
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawOwn();

	//ScreenFlip();

}

void GameScene::DrawOwn()
{




	//DrawGraph(0, 0, IpImageMng.GetID("2")[0], true);

	IpNetwork.tmx_->Draw();
	for (auto& player : play_)
	{
		player->Draw();
	}

	SetFontSize(50);
	//DrawString(0, 0, "Game  Scene", 0x000000, 0xFFFFFF);
}



unique_Base GameScene::Update(unique_Base own)
{

	Draw();

	for (auto& player : play_)
	{
		player->Updata();
	}
	
	return std::move(own);

}

GameScene::GameScene()
{
	IpImageMng.GetID("2", "image/2.jpg", { 800,600 }, { 1,1 });

	if (IpNetwork.GetNetWorkMode() == NetWorkMode::OFFLINE)
	{
		IpNetwork.tmx_->LoadTmx("map/ObjTest.tmx");

	}

	std::vector<Vector2> plPos;
	mapDataBase_.resize((__int64)21 * 17);
	for (size_t no = 0; no < 17; no++)
	{
		mapData_.emplace_back(&mapDataBase_[no * 21]);
	}
	for (int i = 0; i < IpNetwork.tmx_->layer_["3"].size(); i++)
	{
		mapDataBase_[i] = IpNetwork.tmx_->layer_["3"][i];

	}

	int cntID_ = 0;
	for (int i = 0; i < IpNetwork.tmx_->layer_["4"].size(); i++)
	{
		if (IpNetwork.tmx_->layer_["4"][i] == 4)
		{
			play_.emplace_back(std::make_unique<Character>(mapDataBase_, mapData_, Vector2{i%21,i/21},cntID_ ));
			cntID_++;

		}

	}
	IpNetwork.updata_;


}

GameScene::~GameScene()
{
}


