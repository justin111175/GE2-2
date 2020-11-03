#pragma once
#include "BaseScene.h"
#include "SceneMng.h"
#include "../common/Vector2.h"
#define IpSceneMng SceneMng::GetInstance()


class SceneMng
{
public:
	static SceneMng& GetInstance(void)							// ゲット-インスタンス
	{
		Create();
		return *sInstance;
	}
	static void Create()
	{
		if (sInstance == nullptr)
		{
			sInstance = new SceneMng();
		}
	}
	static void Destroy()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
		}
		sInstance = nullptr;
	}
	void Run(void);

	const int frames(void)const;								// フレーム数のゲット関数
	const Vector2 screenSize;
	int blendCnt_;

private:
	static SceneMng* sInstance;									// インスタンス
	unique_Base Scene_;									// 今Sceneの位置


	int frames_;												// フレーム数

	~SceneMng();
	SceneMng();
	bool SysInit(void);


};


