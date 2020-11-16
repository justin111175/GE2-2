#pragma once
#include "BaseScene.h"
#include "../InterNet/NetWork.h"
#include "../Character.h"

class GameScene :
	public BaseScene
{
public:

	unique_Base Update(unique_Base own) override;					// çXêV

	GameScene();
	~GameScene();

private:
	void Draw();
	void DrawOwn() override;
	std::vector<std::unique_ptr<Character>> play_;

	std::vector<int> mapDataBase_;
	std::vector<int*> mapData_;

};

