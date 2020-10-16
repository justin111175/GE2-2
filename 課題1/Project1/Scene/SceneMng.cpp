#include "SceneMng.h"
#include "Dxlib.h"
#include"TitleScene.h"
//#include"common/_debug/_DebugConOut.h"
//#include "common/_debug/_DebugDispOut.h"

SceneMng* SceneMng::sInstance = nullptr;



void SceneMng::Run(void)
{

	if (!SysInit())
	{
		//AST();
	}

	activeScene_ = std::make_unique<TitleScene>();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//_dbgAddDraw();
		activeScene_ = (*activeScene_).Update(std::move(activeScene_));


		frames_++;
	}
}

const int SceneMng::frames(void) const
{
	return frames_;
}

void SceneMng::Draw(void)
{
	ClsDrawScreen();
	//SetFontSize(100);
	//DrawString(100, 100, "練習", 0xFFFFFF);
	
	ScreenFlip();

}

SceneMng::~SceneMng()
{
}

SceneMng::SceneMng():
	screenSize{800,600}
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



	return true;

}