#include "Bomb.h"
#include "../../common/_debug/_DebugConOut.h"
#include "../../common/_debug/_DebugDispOut.h"
#include "../GameScene.h"
#include "../SceneMng.h"
Bomb::Bomb(BombStruct bomb, BaseScene& scene):scene_(scene)
{
	IpImageMng.GetID("Bomb", "image/bomb.png", { 32,32 }, { 2,7 });
	Vector2 tmpPos= bomb.pos_/32;
	pos_ = tmpPos*32;
	
	id_ = bomb.ownID_;
	selfID_ = bomb.selfID_;

	start_ = bomb.start_;
	isAlive = true;
	isDeath = false;



}

Bomb::~Bomb()
{
}

void Bomb::Update(void)
{
	end_ = std::chrono::system_clock::now();

	if (std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count() > std::chrono::milliseconds(3000).count())
	{
		dynamic_cast<GameScene&>(scene_).SetFire(pos_);

		isAlive = false;
	}

	
}

void Bomb::Draw(void)
{
	_dbgDrawFormatString(pos_.x, pos_.y-20, 0xFFFFFF, "%d/%d", id_,selfID_);

	DrawGraph(pos_.x, pos_.y, IpImageMng.GetID("Bomb")[IpSceneMng.frames()/10 % 2 * 2], true);


}

bool Bomb::isPacket()
{
	return false;
}


