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

	animeBomb_.try_emplace(AnimeState::ê∂Ç´ÇƒÇ¢ÇÈ);

	animeBomb_[AnimeState::ê∂Ç´ÇƒÇ¢ÇÈ].emplace_back(0);
	animeBomb_[AnimeState::ê∂Ç´ÇƒÇ¢ÇÈ].emplace_back(2);
	
	animeBomb_.try_emplace(AnimeState::éÄÇÒÇæ);
	animeBomb_[AnimeState::éÄÇÒÇæ].emplace_back(1);
	animeBomb_[AnimeState::éÄÇÒÇæ].emplace_back(3);
	animeBomb_[AnimeState::éÄÇÒÇæ].emplace_back(5);
	animeBomb_[AnimeState::éÄÇÒÇæ].emplace_back(7);
	animeBomb_[AnimeState::éÄÇÒÇæ].emplace_back(9);
	animeBomb_[AnimeState::éÄÇÒÇæ].emplace_back(11);
	animeBomb_[AnimeState::éÄÇÒÇæ].emplace_back(13);

	count_ = 0;

}

Bomb::~Bomb()
{
}

void Bomb::Update(void)
{
	if (isAlive)
	{
		end_ = std::chrono::system_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count() > std::chrono::milliseconds(3000).count())
		{
			dynamic_cast<GameScene&>(scene_).SetFire(pos_);

			isAlive = false;
		}
	}
	else
	{
		end_ = std::chrono::system_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count() > std::chrono::milliseconds(1000/6).count())
		{

			count_++;
			start_ = std::chrono::system_clock::now();

			if (count_ >= 7)
			{
				isDeath = true;
			}
		}
	}


	
}

void Bomb::Draw(void)
{
	_dbgDrawFormatString(pos_.x, pos_.y-20, 0xFFFFFF, "%d/%d", id_,selfID_);
	if (isAlive)
	{
		DrawGraph(pos_.x, pos_.y, IpImageMng.GetID("Bomb")[animeBomb_[AnimeState::ê∂Ç´ÇƒÇ¢ÇÈ][IpSceneMng.frames()/10%2]], true);
	}
	else
	{
		DrawGraph(pos_.x, pos_.y, IpImageMng.GetID("Bomb")[animeBomb_[AnimeState::éÄÇÒÇæ][count_]], true);

	}

}

bool Bomb::isPacket()
{
	return false;
}


