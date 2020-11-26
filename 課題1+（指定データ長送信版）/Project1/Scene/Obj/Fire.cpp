#include "Fire.h"
#include "../GameScene.h"


Fire::Fire(Vector2 pos,int lengh, BaseScene& scene):scene_(scene)
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

	lengh_ = lengh-1;
	vecPairFire_.resize(4);

	TRACE("FIRE_TEST");
	cnt_ = 0;
	


	for (int i = 0; i < 4; i++)
	{
		vecPairFire_[i].first = false;

	}


	Vector2 tmpPos = pos_ / 32;

	//ã
	
	if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y- (lengh_-1)][tmpPos.x ] == 8)
	{
		vecPairFire_[0].first = true;
		IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y- (lengh_-1)][tmpPos.x ] = 0;
	}
	else
	{
		if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y- (lengh_ - 1)][tmpPos.x ] == 0)
		{
			vecPairFire_[0].second = lengh_-1;
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y- lengh_][tmpPos.x ] == 0)
			{
				vecPairFire_[0].second = lengh_;
			}
			else if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y- lengh_][tmpPos.x] == 8)
			{
				vecPairFire_[0].second = lengh_;
			}
		}
	}

	//‰E
	if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x+1] == 8)
	{

		vecPairFire_[1].first = true;
		IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + 1] = 0;
	}
	else
	{
		if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + 1] == 0)
		{
			vecPairFire_[1].second = 1;
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + 2] == 0)
			{
				vecPairFire_[1].second = 2;
			}
			else if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + 2] == 8)
			{
				vecPairFire_[1].second = 2;
			}
		}
	}


	//‰º
	if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y + 1][tmpPos.x] == 8)
	{

		vecPairFire_[2].first = true;
		IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y+ 1][tmpPos.x ] = 0;
	}
	else
	{
		if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y+ 1][tmpPos.x ] == 0)
		{
			vecPairFire_[2].second = 1;
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y+ 2][tmpPos.x ] == 0)
			{
				vecPairFire_[2].second = 2;
			}
			else if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y + 2][tmpPos.x] == 8)
			{
				vecPairFire_[2].second = 2;
			}
		}
	}
	//¶
	if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x- 1] == 8)
	{

		vecPairFire_[3].first = true;
		IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x - 1] = 0;
	}
	else
	{
		if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x - 1] == 0)
		{
			vecPairFire_[3].second = 1;
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x-2] == 0)
			{
				vecPairFire_[3].second = 2;
			}
			else if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x - 2] == 8)
			{
				vecPairFire_[3].second = 2;
			}
		}
	}


}

Fire::~Fire()
{

	for (int y = 0; y < 17; y++)
	{
		for (int x = 0; x < 21; x++)
		{
			IpNetwork.tmx_->checkMap_[y][x] = -1;
		}
	}
}

void Fire::Update(void)
{
	Vector2 tmpPos = pos_ / 32;


	end_ = std::chrono::system_clock::now();

	if (std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count() > std::chrono::milliseconds(1000).count()/6)
	{

		count_++;
		start_ = std::chrono::system_clock::now();

	}


	if (count_ >=7)
	{
		isAlive = false;
		isDeath = true;

	}

	if (count_ >= 3)
	{
		if (vecPairFire_[0].second == 2)
		{
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y- 2][tmpPos.x ] == 8)
			{

				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y - 2][tmpPos.x ] = 0;
				IpNetwork.tmx_->checkMap_[tmpPos.y- 2][tmpPos.x ] = 1;

			}
		}
		if (vecPairFire_[1].second == 2)
		{
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + 2] == 8)
			{

				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + 2] = 0;
				IpNetwork.tmx_->checkMap_[tmpPos.y ][tmpPos.x+ 2] = 1;

			}
		}

		if (vecPairFire_[2].second == 2)
		{
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y + 2][tmpPos.x] == 8)
			{

				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y + 2][tmpPos.x] = 0;
				IpNetwork.tmx_->checkMap_[tmpPos.y + 2][tmpPos.x] = 1;

			}
		}
		if (vecPairFire_[3].second == 2)
		{
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x - 2] == 8)
			{

				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x - 2] = 0;
				IpNetwork.tmx_->checkMap_[tmpPos.y ][tmpPos.x- 2] = 1;

			}
		}
	}


	

}

void Fire::Draw(void)
{

	Vector2 tmpPos = pos_ / 32;
	int tmp = count_ / 10 % 7;

	if (count_ <= 4)
	{
		DrawGraph(pos_.x, pos_.y, IpImageMng.GetID("Fire")[vecfire_[count_%7]], true);
		IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x] = 1;
	}


	//ã
	if (vecPairFire_[0].first)
	{
		if (count_ > 1)
		{
			DrawRotaGraph3(pos_.x, pos_.y, 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + 2], true, false);
			IpNetwork.tmx_->checkMap_[tmpPos.y-1][tmpPos.x] = 1;
		}
	}
	else
	{
		for (int i = 1; i <= vecPairFire_[0].second; i++)
		{
			if (count_ < 4)
			{
				if (count_ >= i)
				{
					DrawRotaGraph3(pos_.x, pos_.y - 32 * (i - 1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
					IpNetwork.tmx_->checkMap_[tmpPos.y -i][tmpPos.x] = 1;

				}
			}
			else
			{
				if (count_ < 5 + i)
				{
					DrawRotaGraph3(pos_.x, pos_.y - 32 * (i - 1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
				}
				else
				{
					if (vecPairFire_[0].second > 1)
					{
						i++;
						DrawRotaGraph3(pos_.x, pos_.y - 32 * (i - 1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ % 7] + i], true, false);

					}
				}
			}
		}
	}



	//‰E
	if (vecPairFire_[1].first)
	{
		if (count_ > 1)
		{
			DrawRotaGraph3(pos_.x + 32, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ % 7] + 2], true, false);
			IpNetwork.tmx_->checkMap_[tmpPos.y ][tmpPos.x+1] = 1;

		}
	}
	else
	{
		for (int i = 1; i <= vecPairFire_[1].second; i++)
		{
			if (count_ < 4)
			{
				if (count_ >= i)
				{
					DrawRotaGraph3(pos_.x + 32 * i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ % 7] + i], true, false);
					IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x +i] = 1;

				}
			}
			else
			{
				if (count_ < 5+i)
				{
					DrawRotaGraph3(pos_.x + 32 *i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ % 7] + i], true, false);
				}
				else
				{
					if (vecPairFire_[1].second > 1)
					{
						i++;
						DrawRotaGraph3(pos_.x + 32 * i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ % 7] + i], true, false);
					}
				}
			}
		}
	}

	////‰º

	if (vecPairFire_[2].first)
	{
		if (count_ > 1)
		{
			DrawRotaGraph3(pos_.x + 32, pos_.y + 32, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + 2], true, false);
			IpNetwork.tmx_->checkMap_[tmpPos.y+ 1][tmpPos.x ] = 1;

		}
	}
	else
	{
		for (int i = 1; i <= vecPairFire_[2].second; i++)
		{
			if (count_ < 4)
			{
				if (count_ >= i)
				{
					DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
					IpNetwork.tmx_->checkMap_[tmpPos.y +i][tmpPos.x] = 1;

				}
			}
			else
			{
				if (count_ <= 5 + i)
				{
					DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
				}
				else
				{
					if (vecPairFire_[2].second >= 1)
					{
						i++;
						DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
					}
				}
			}
		}
	}



	//¶
	if (vecPairFire_[3].first)
	{
		if (count_ > 1)
		{
			DrawRotaGraph3(pos_.x, pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + 2], true, false);
			IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x - 1] = 1;

		}
	}
	else
	{
		for (int i = 1; i <= vecPairFire_[3].second; i++)
		{
			if (count_ < 4)
			{
				if (count_ >= i)
				{
					DrawRotaGraph3(pos_.x - 32 * (i - 1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
					IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x -i] = 1;

				}
			}
			else
			{
				if (count_ <= 5 + i)
				{
					DrawRotaGraph3(pos_.x - 32 * (i - 1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
				}
				else
				{
					if (vecPairFire_[3].second >= 1)
					{
						i++;
						DrawRotaGraph3(pos_.x - 32 * (i - 1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
					}
				}
			}
		}
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
