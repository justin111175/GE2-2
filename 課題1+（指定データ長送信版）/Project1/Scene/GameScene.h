#pragma once
#include "BaseScene.h"
#include "../InterNet/NetWork.h"


class GameScene :
	public BaseScene
{
public:

	unique_Base Update(unique_Base own) override;					// �X�V

	GameScene();
	~GameScene();

private:
	void Draw();
	void DrawOwn() override;


};

