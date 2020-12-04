#include "Fire.h"
#include "../GameScene.h"


Fire::Fire(Vector2 pos, int lengh, bool flag, BaseScene& scene):scene_(scene)
{
	pos_ = pos;
	IpImageMng.GetID("Fire", "image/fire.png", { 32,32 }, { 3,4 });
	isAlive = true;
	isDeath = false;
	
	count_ = 0;

	zorder_ = 1;


	vecfire_.emplace_back(0);
	vecfire_.emplace_back(3);
	vecfire_.emplace_back(6);
	vecfire_.emplace_back(9);
	vecfire_.emplace_back(6);
	vecfire_.emplace_back(3);
	vecfire_.emplace_back(0);

	flag_ = flag;
	lengh_.try_emplace(FIRE_DIR::RIGHT);
	lengh_.try_emplace(FIRE_DIR::DOWN);
	lengh_.try_emplace(FIRE_DIR::LEFT);
	lengh_.try_emplace(FIRE_DIR::UP);

	lengh_[FIRE_DIR::RIGHT] = lengh;
	lengh_[FIRE_DIR::DOWN] = lengh;
	lengh_[FIRE_DIR::LEFT] = lengh;
	lengh_[FIRE_DIR::UP] = lengh;

	start_ = std::chrono::system_clock::now();
	SetFireInit();
	IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32] = 1;


}

Fire::Fire(Vector2 pos, double rad,int cnt, BaseScene& scene) :scene_(scene)
{
	pos_ = pos;
	IpImageMng.GetID("Fire", "image/fire.png", { 32,32 }, { 3,4 });
	isAlive = true;
	isDeath = false;

	count_ = 0;

	zorder_ = 1;


	vecfire_.emplace_back(0);
	vecfire_.emplace_back(3);
	vecfire_.emplace_back(6);
	vecfire_.emplace_back(9);
	vecfire_.emplace_back(6);
	vecfire_.emplace_back(3);
	vecfire_.emplace_back(0);

	flag_ = false;

	start_ = std::chrono::system_clock::now();
	rad_ = rad;

	cnt_ = cnt;
	SetFireInit();
	IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32] = 1;


}


Fire::~Fire()
{

	IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32] = -1;

}

void Fire::Update(void)
{
	
	if (count_ < 7)
	{
		Draw();

		end_ = std::chrono::system_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count() > std::chrono::milliseconds(1000).count() / 6)
		{
			count_++;
			setFire_[FIRE_DIR::RIGHT]();
			setFire_[FIRE_DIR::DOWN]();
			setFire_[FIRE_DIR::LEFT]();
			setFire_[FIRE_DIR::UP]();

			start_ = std::chrono::system_clock::now();

		}
	}
	else
	{
		isAlive = false;
		isDeath = true;

	}

}

void Fire::Draw(void)
{

	if (flag_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawGraph(pos_.x, pos_.y, IpImageMng.GetID("Fire")[vecfire_[count_%7]], true);

		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawRotaGraph3(pos_.x, pos_.y, 0, 0, 1.0f, 1.0f, rad_ * DEG, IpImageMng.GetID("Fire")[vecfire_[count_%7]+ cnt_], true, false);

		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	}

}

bool Fire::isPacket()
{
	return false;
}

ObjID Fire::GetObjID()
{
	return ObjID();
}

void Fire::SetFireInit()
{
	setFire_.try_emplace(FIRE_DIR::RIGHT, [&]() {
			lengh_[FIRE_DIR::RIGHT]--;
			int tmp = 0;
			Vector2 tmpPos = pos_ / 32;

			if (lengh_[FIRE_DIR::RIGHT] > 0)
			{
				lengh_[FIRE_DIR::RIGHT] == 1 ? tmp = 2 : tmp = 1;

				//‰E
				if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + count_] == 0)
				{
					dynamic_cast<GameScene&>(scene_).SetFire(pos_ + Vector2{ count_ * 32, 0 }, 0.0, tmp);
				}
				else
				{
					if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + count_] == 8)
					{
						IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + count_] = 0;
						dynamic_cast<GameScene&>(scene_).SetFire(pos_ + Vector2{ count_ * 32, 0 }, 0.0, 2);
					}
					lengh_[FIRE_DIR::RIGHT] = 0;
				}
			}

		});
	setFire_.try_emplace(FIRE_DIR::DOWN, [&]() {

		lengh_[FIRE_DIR::DOWN]--;
		int tmp = 0;
		Vector2 tmpPos = pos_ / 32;

		if (lengh_[FIRE_DIR::DOWN] > 0)
		{
			lengh_[FIRE_DIR::DOWN] == 1 ? tmp = 2 : tmp = 1;

			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y + count_][tmpPos.x] == 0)
			{
				dynamic_cast<GameScene&>(scene_).SetFire(pos_ + Vector2{ 32, count_ * 32 }, 90.0, tmp);
			}
			else
			{
				if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y + count_][tmpPos.x] == 8)
				{
					IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y + count_][tmpPos.x] = 0;
					dynamic_cast<GameScene&>(scene_).SetFire(pos_ + Vector2{ 32, count_ * 32 }, 90.0, 2);
				}
				lengh_[FIRE_DIR::DOWN] = 0;
			}
		}



		});
	setFire_.try_emplace(FIRE_DIR::LEFT, [&]() {

		lengh_[FIRE_DIR::LEFT]--;
		int tmp = 0;
		Vector2 tmpPos = pos_ / 32;

		if (lengh_[FIRE_DIR::LEFT] > 0)
		{
			lengh_[FIRE_DIR::LEFT] == 1 ? tmp = 2 : tmp = 1;

			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x- count_] == 0)
			{
				dynamic_cast<GameScene&>(scene_).SetFire(pos_ + Vector2{ 32-count_*32, 32 }, 180.0,tmp);
			}
			else
			{
				if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x- count_] == 8)
				{
					IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x- count_] = 0;
					dynamic_cast<GameScene&>(scene_).SetFire(pos_ + Vector2{ 32-count_*32, 32 }, 180.0,2);
				}
				lengh_[FIRE_DIR::LEFT] = 0;
			}
		}

		});
	setFire_.try_emplace(FIRE_DIR::UP, [&]() {
		lengh_[FIRE_DIR::UP]--;
		int tmp = 0;
		Vector2 tmpPos = pos_ / 32;

		if (lengh_[FIRE_DIR::UP] > 0)
		{
			lengh_[FIRE_DIR::UP] == 1 ? tmp = 2 : tmp = 1;

			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y- count_][tmpPos.x ] == 0)
			{
				dynamic_cast<GameScene&>(scene_).SetFire(pos_ + Vector2{ 0,32 -count_ * 32 }, 270.0,tmp);
			}
			else
			{
				if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y- count_][tmpPos.x ] == 8)
				{
					IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y- count_][tmpPos.x ] = 0;
					dynamic_cast<GameScene&>(scene_).SetFire(pos_ + Vector2{ 0,32 -count_ * 32 }, 270.0,2);
				}
				lengh_[FIRE_DIR::UP] = 0;
			}
		}

		});

}
