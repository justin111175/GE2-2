#include "TitleScene.h"
#include "Dxlib.h"
#include "SceneMng.h"
#include "../common/_debug/_DebugConOut.h"
#include "../common/_debug/_DebugDispOut.h"
#include <fstream>
#include <iostream>

#include <sstream>

#include "LoginScene.h"
#include "Transition.h"
#include "LoginScene.h"
TitleScene::TitleScene()
{
	screen_size_ = { 0,0 };



	

	IpImageMng.GetID("0", "image/0.jpg", {800,600}, { 1,1 });





	Init();
	TRACE("スペースキー押し待ち\n");
}

TitleScene::~TitleScene()
{
}




unique_Base TitleScene::Update(unique_Base own)
{
	Draw();


   	if(CheckHitKey(KEY_INPUT_SPACE))
	{

		return std::make_unique<Transition>(std::move(own), SceneChange::Title_GO_Login);
		//return std::make_unique<LoginScene>();
	}


	return std::move(own);
}





void TitleScene::Init(void)
{
	GetDrawScreenSize(&screen_size_.x,&screen_size_.y);

}




void TitleScene::Draw()
{
	ClsDrawScreen();

	SetDrawScreen(screenID_);

	DrawOwn();
	ScreenFlip();

}

void TitleScene::DrawOwn()
{

	DrawGraph(pos_.x, pos_.y, IpImageMng.GetID("0")[0], true);
	
	SetFontSize(50);
	DrawString(0, 0, "Title  Scene", 0x000000, 0xFFFFFF);



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



