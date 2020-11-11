#include "GameScene.h"


void GameScene::Draw()
{	
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawOwn();
	ScreenFlip();

}

void GameScene::DrawOwn()
{




	DrawGraph(0, 0, IpImageMng.GetID("2")[0], true);

	IpNetwork.tmx_->Draw();

	SetFontSize(50);
	DrawString(0, 0, "Game  Scene", 0x000000, 0xFFFFFF);

}



unique_Base GameScene::Update(unique_Base own)
{

	Draw();



	return std::move(own);

}

GameScene::GameScene()
{
	IpImageMng.GetID("2", "image/2.jpg", { 800,600 }, { 1,1 });

}

GameScene::~GameScene()
{
}


