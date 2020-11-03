#pragma once
#include "BaseScene.h"
#include "SceneMng.h"
#include "../common/Vector2.h"
#define IpSceneMng SceneMng::GetInstance()


class SceneMng
{
public:
	static SceneMng& GetInstance(void)							// �Q�b�g-�C���X�^���X
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

	const int frames(void)const;								// �t���[�����̃Q�b�g�֐�
	const Vector2 screenSize;
	int blendCnt_;

private:
	static SceneMng* sInstance;									// �C���X�^���X
	unique_Base Scene_;									// ��Scene�̈ʒu


	int frames_;												// �t���[����

	~SceneMng();
	SceneMng();
	bool SysInit(void);


};


