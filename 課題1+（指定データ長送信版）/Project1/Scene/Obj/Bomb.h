#pragma once
#include "Obj.h"
#include <chrono>
#include "../BaseScene.h"

struct BombStruct
{

	Vector2 pos_;
	int ownID_;
	int selfID_;
	bool initFlag_;
	int lengh_;
	std::chrono::system_clock::time_point start_;

};

enum class AnimeState
{
	生きている,
	死んだ
};

class Bomb :
	public Obj
{
public:

	Bomb(BombStruct, BaseScene& scene);
	~Bomb();
	void Update(void) override;

	void Draw(void)override;
	bool isPacket()override;
	ObjID GetObjID()override;


private:
	int selfID_;
	int lengh_;
	std::chrono::system_clock::time_point start_, end_;
	BaseScene& scene_;

	std::map<AnimeState,std::vector<int>> animeBomb_;

	int count_;
};

