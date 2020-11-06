#include "LoginScene.h"
#include"Transition.h"




unique_Base LoginScene::Update(unique_Base own)
{
	DrawOwn();

	//if (IpNetwork.Updata())
	{
		if (IpNetwork.GetMode() != UpdataMode::SetNetWork)
		{
			if (IpNetwork.GetActiv() == ActiveState::Play)
			{
				return std::make_unique<Transition>(std::move(own), SceneChange::Login_GO_Game);

			}
		}

	}






	return std::move(own);
}

LoginScene::LoginScene()
{
	IpImageMng.GetID("1", "image/1.jpg", { 800,600 }, { 1,1 });

	//IpNetwork.tmx_->SendData();

	IpNetwork.updata_;

}

LoginScene::~LoginScene()
{
}

void LoginScene::DrawOwn()
{

	SetDrawScreen(screenID_);
	//ClsDrawScreen();


	DrawGraph(0, 0, IpImageMng.GetID("1")[0], true);


	SetFontSize(50);
	DrawString(0, 0, "Login  Scene", 0x000000, 0xFFFFFF);
	//ScreenFlip();

}
