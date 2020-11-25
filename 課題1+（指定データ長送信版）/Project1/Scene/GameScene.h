#pragma once
#include "BaseScene.h"
#include "../InterNet/NetWork.h"
#include "Obj/Obj.h"

#include "Obj/Bomb.h"

#include <list>


class GameScene :
	public BaseScene
{
public:

	unique_Base Update(unique_Base own) override;					// çXêV

	GameScene();
	~GameScene();
	void SetBomb(int ownID,int selfID,Vector2 pos, std::chrono::system_clock::time_point time,bool sendFlag);
	void SetFire(Vector2);
	void DeathEraser();

	int GetBombSize(int ownID_);

	void RevBomb();

private:
	void Draw();
	void DrawOwn() override;

	//std::vector<std::unique_ptr<Character>> playList_;

	std::list<unique_Obj> objList_;


	std::chrono::system_clock::time_point start_, end_;
	std::vector<int> fallCnt;
	int Cnt_;
	int count_;
	BombStruct bomb_;


};

