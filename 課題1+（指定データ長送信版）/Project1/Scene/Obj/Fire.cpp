#include "Fire.h"
Fire::Fire(Vector2 pos)
{
	pos_ = pos;
	IpImageMng.GetID("Fire", "image/fire.png", { 32,32 }, { 3,4 });
	isAlive = true;
	isDeath = false;
	count_ = 0;

	vecfire_.emplace_back(0);
	vecfire_.emplace_back(3);
	vecfire_.emplace_back(6);
	vecfire_.emplace_back(9);
	vecfire_.emplace_back(6);
	vecfire_.emplace_back(3);
	vecfire_.emplace_back(0);


	vecPairFire_.resize(4);

	//pairMap_ = IpNetwork.tmx_->GetMap();
	TRACE("FIRE_TEST");
	cnt_ = 0;
	


	for (int i = 0; i < 4; i++)
	{
		vecPairFire_[i].first = false;

	}


	Vector2 tmpPos = pos_ / 32;

	//上
	if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y-1][tmpPos.x ] == 8)
	{
		vecPairFire_[0].first = true;
		IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y-1][tmpPos.x ] = 0;
	}
	else
	{
		if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y-1][tmpPos.x ] == 0)
		{
			vecPairFire_[0].second = 1;
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y-2][tmpPos.x ] == 0)
			{
				vecPairFire_[0].second = 2;
			}
			else if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y-2][tmpPos.x] == 8)
			{
				vecPairFire_[0].second = 2;

				vecPairFire_[0].first = false;
				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y-2][tmpPos.x ] = 0;
			}
		}
	}

	//右
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

				vecPairFire_[1].first = false;
				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + 2] = 0;
			}
		}
	}

	//下
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

				vecPairFire_[2].first = false;
				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y + 2][tmpPos.x] = 0;
			}
		}
	}
	//左
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

				vecPairFire_[3].first = false;
				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x - 2] = 0;
			}
		}
	}


}

Fire::~Fire()
{
}

void Fire::Update(void)
{
	Vector2 tmpPos = pos_ / 32;


	if (count_ < 10 * 7)
	{
		count_++;


	}
	else
	{
		isAlive = false;
		isDeath = true;

	}



	

}

void Fire::Draw(void)
{

	Vector2 tmpPos = pos_ / 32;
	int tmp = count_ / 10 % 7;

	if (count_ <= 40)
	{
		DrawGraph(pos_.x, pos_.y, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7]], true);
	}


	//上

	if (vecPairFire_[0].first)
	{
		DrawRotaGraph3(pos_.x, pos_.y, 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + 2], true, false);
	}
	else
	{
		for (int i = 1; i <= vecPairFire_[0].second; i++)
		{
			DrawRotaGraph3(pos_.x, pos_.y - 32 * (i - 1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);

		}
	}



	//右
	if (vecPairFire_[1].first)
	{
		DrawRotaGraph3(pos_.x + 32, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + 2], true, false);
	}
	else
	{
		for (int i = 1; i <= vecPairFire_[1].second; i++)
		{
			DrawRotaGraph3(pos_.x + 32 * i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);

		}
	}

	//下

	if (vecPairFire_[2].first)
	{
		DrawRotaGraph3(pos_.x + 32, pos_.y + 32 , 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + 2], true, false);
	}
	else
	{
		for (int i = 1; i <= vecPairFire_[2].second; i++)
		{
			DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);

		}
	}

	//左

	if (vecPairFire_[3].first)
	{
		DrawRotaGraph3(pos_.x , pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + 2], true, false);
	}
	else
	{
		for (int i = 1; i <= vecPairFire_[3].second; i++)
		{
			DrawRotaGraph3(pos_.x - 32*(i-1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);

		}
	}


	//for (int i = 1; i <= 2; i++)
	//{
	//	if (count_ <= 40)
	//	{

	//		if (count_ > -10+10 * i)
	//		{
	//			//上
	//			DrawRotaGraph3(pos_.x, pos_.y-32*(i-1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
	//			//右
	//			DrawRotaGraph3(pos_.x + 32 * i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
	//			
	//			//下
	//			DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
	//			//左
	//			DrawRotaGraph3(pos_.x - 32*(i-1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
	//		}
	//	}
	//	else
	//	{
	//		if (count_ < 60 + i * 10)
	//		{
	//			//上
	//			DrawRotaGraph3(pos_.x, pos_.y - 32 * (i - 1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
	//			//右

	//			DrawRotaGraph3(pos_.x + 32 * i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
	//			
	//			//下
	//			DrawRotaGraph3(pos_.x + 32, pos_.y + 32*i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
	//			//左
	//			DrawRotaGraph3(pos_.x - 32 * (i - 1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
	//		}
	//	}
	//}



}

bool Fire::isPacket()
{
	return false;
}
