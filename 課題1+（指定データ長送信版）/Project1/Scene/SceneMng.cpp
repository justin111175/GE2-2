#include "SceneMng.h"
#include "Dxlib.h"
#include"TitleScene.h"
#include "Transition.h"
#include "../common/_debug/_DebugConOut.h"
#include "../common/_debug/_DebugDispOut.h"
#include "GameScene.h"
SceneMng* SceneMng::sInstance = nullptr;



void SceneMng::Run(void)
{

	if (!SysInit())
	{
		//AST();
	}

	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);

	Scene_ = std::make_unique<TitleScene>();

	while (ProcessMessage() == 0 )
	{
		_dbgStartDraw();

		Scene_ = (*Scene_).Update(std::move(Scene_));
		
		SetDrawScreen(DX_SCREEN_BACK);

 		Scene_->Draw();
		_dbgAddDraw();
		ScreenFlip();

		frames_++;
	}
}

const int SceneMng::frames(void) const
{
	return frames_;
}



SceneMng::~SceneMng()
{
}

SceneMng::SceneMng():
	screenSize{1080,720}
{
	frames_ = 0;
	blendCnt_ = 0;
}

bool SceneMng::SysInit(void)
{
	SetWindowText("YANG CHIHCHENG_ネット練習");											// タイトルバー
	SetGraphMode(screenSize.x, screenSize.y, 16);
	ChangeWindowMode(true);												// ture:window false:フルスクリーン



	if (DxLib_Init() == -1)												// DXラリブラリの初期化
	{
		return false;
	}

	_dbgSetup(screenSize.x, screenSize.y, 255);

	return true;

}