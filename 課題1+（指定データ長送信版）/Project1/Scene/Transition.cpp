#include "Transition.h"
#include "TitleScene.h"
#include "LoginScene.h"
#include "GameScene.h"

#include "../common/_debug/_DebugConOut.h"

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
		//tranScene_ = std::make_unique<TitleScene>();
		childScene_[1]= std::make_unique<LoginScene>();
		break;
	case SceneChange::Login_GO_Game:
		childScene_[1] = std::make_unique<GameScene>();
		break;
	default:
		break;
	}
	
	cnt_ = 0;

}



Transition::~Transition()
{
}





void Transition::DrawOwn()
{


	TRACE("Transition‚µ‚Ä‚¢‚é\n");



}
