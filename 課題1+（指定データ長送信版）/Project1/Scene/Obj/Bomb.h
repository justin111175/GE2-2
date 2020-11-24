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
	std::chrono::system_clock::time_point start_;

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


private:
	int selfID_;
	std::chrono::system_clock::time_point start_, end_;
	BaseScene& scene_;

};

