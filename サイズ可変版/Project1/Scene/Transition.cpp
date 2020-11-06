#include "Transition.h"
#include "TitleScene.h"
#include "LoginScene.h"
#include "GameScene.h"



unique_Base Transition::Update(unique_Base own)
{

	return std::make_unique<CrossOver>(std::move(childScene_[0]), std::move(childScene_[1]));
}

Transition::Transition(unique_Base own,SceneChange scene_)
{

	childScene_[0] = std::move(own);

	switch (scene_)
	{
	case SceneChange::Title_GO_Login:
		childScene_[1] = std::make_unique<LoginScene>();
		break;
	case SceneChange::Login_GO_Game:
		childScene_[1] = std::make_unique<GameScene>();
		break;
	default:
		break;
	}
	

}

Transition::~Transition()
{
}



void Transition::DrawOwn()
{

	//SetDrawScreen(screenID_);
	//ClsDrawScreen();
	////childScene_[0]->Updata();
	////scene_[1]->Updata();


	//SetFontSize(50);
	//DrawString(0, 0, "Transition  Scene", 0x000000, 0xFFFFFF);



	//ScreenFlip();


}
