#include "BaseScene.h"
#include"../common/Input/PadInput.h"
#include "../common/Input/KeyInput.h"



BaseScene::BaseScene()
{
	controller.try_emplace(conType::Key, std::make_unique<KeyInput>());
	controller[conType::Key]->SetUp();

	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());
	controller[conType::Pad]->SetUp();

}

BaseScene::~BaseScene()
{
}



void BaseScene::Draw()
{

	DrawGraph(0, 0, screenID_, true);
}


