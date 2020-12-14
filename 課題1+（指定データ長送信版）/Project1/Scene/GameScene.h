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

	unique_Base Update(unique_Base own) override;					// 更新

	GameScene();
	~GameScene();
	void SetBomb(int ownID,int selfID,Vector2 pos,int lengh, std::chrono::system_clock::time_point time,bool sendFlag);
	void SetFire(Vector2,int);
	void SetFire(Vector2,double, int cnt);
	void DeathEraser();

	int GetBombSize(int ownID_);

	void RevBomb();
private:
	void Draw();
	void DrawOwn() override;

	void GetPlayerPac();
	void GetStartTime();
	void GetDeathID();
	void EraserLost();
	int lostID_;

	std::list<int> deathID_;

	std::pair<int, int> playerPac_;//プレイヤーのID、プレイヤーの数

	std::list<unique_Obj> objList_;


	std::chrono::system_clock::time_point start_, end_, start_time_;
	std::vector<int> fallCnt;
	int Cnt_;
	int count_;
	BombStruct bomb_;

};

