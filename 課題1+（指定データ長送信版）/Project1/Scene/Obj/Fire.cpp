#include "Fire.h"
#include "../GameScene.h"
#include "../SceneMng.h"

Fire::Fire(Vector2 pos,int lengh, BaseScene& scene):scene_(scene)
{
	pos_ = pos;
	IpImageMng.GetID("Fire", "image/fire.png", { 32,32 }, { 3,4 });
	isAlive = true;
	isDeath = false;
	count_ = 0;
	tmpCnt_ = 0;
	zorder_ = 1;


	vecfire_.emplace_back(0);
	vecfire_.emplace_back(3);
	vecfire_.emplace_back(6);
	vecfire_.emplace_back(9);
	vecfire_.emplace_back(6);
	vecfire_.emplace_back(3);
	vecfire_.emplace_back(0);

	lengh_ = lengh-1;


	TRACE("FIRE_TEST");
	


	for (int i = 0; i < lengh; i++)
	{
		cnt_.emplace_back(lengh);
	}


	Vector2 tmpPos = pos_ / 32;
	vecPairFire_.resize(4);

	for (int i = 0; i < 4; i++)
	{
		vecPairFire_[i].first = false;
		vecPairFire_[i].second = 0;
	}


	for (int i = 1; i <= lengh_; i++)
	{

		if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y - i][tmpPos.x] != 0)
		{
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y - i][tmpPos.x] == 8)
			{
				vecPairFire_[0].first = true;
			}
			vecPairFire_[0].second = i;
			break;
		}
	}

	for (int i = 1; i <= lengh_; i++)
	{

		if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + i] != 0)
		{
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x + i] == 8)
			{
				vecPairFire_[1].first = true;
			}
			vecPairFire_[1].second = i;
			break;
		}
	}

	for (int i = 1; i <= lengh_; i++)
	{

		if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y + i][tmpPos.x] != 0)
		{
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y + i][tmpPos.x] == 8)
			{
				vecPairFire_[2].first = true;
			}
			vecPairFire_[2].second = i;
			break;
		}
	}

	for (int i = 1; i <= lengh_; i++)
	{

		if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x - i] != 0)
		{
			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y][tmpPos.x - i] == 8)
			{
				vecPairFire_[3].first = true;
			}
			vecPairFire_[3].second = i;
			break;
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

		for (int i = 0; i <= tmpCnt_; i++)
		{
			if (cnt_[i] >= 0)
			{
				cnt_[i]--;

			}

		}
		if (tmpCnt_ <= lengh_-1)
		{
			tmpCnt_++;

		}
		
		if (tmpCnt_ > 1)
		{
			count_++;

		}
		if (animeCnt_ < 7)
		{
			animeCnt_++;
		}
		start_ = std::chrono::system_clock::now();

	}


	if (count_ >lengh_*2)
	{
		isAlive = false;
		isDeath = true;

	}

	//if (vecPairFire_[0].first)
	{
		if (count_ >= vecPairFire_[0].second)
		{

			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y - vecPairFire_[0].second][tmpPos.x] == 8)
			{

				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y - vecPairFire_[0].second][tmpPos.x] = 0;
				//IpNetwork.tmx_->checkMap_[tmpPos.y - 2][tmpPos.x] = 1;

			}
		
		}
	}


	//if (vecPairFire_[1].first)
	{
		if (count_ >= vecPairFire_[1].second)
		{

			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x+ vecPairFire_[1].second] == 8)
			{

				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x+ vecPairFire_[1].second] = 0;

			}
		}
	}


	//if (vecPairFire_[2].first)
	{
		if (count_ >= vecPairFire_[2].second)
		{

			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y+ vecPairFire_[2].second][tmpPos.x ] == 8)
			{

				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y+ vecPairFire_[2].second][tmpPos.x ] = 0;

			}
		}
	}

	//if (vecPairFire_[3].first)
	{
		if (count_ >= vecPairFire_[3].second)
		{

			if (IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x- vecPairFire_[3].second] == 8)
			{

				IpNetwork.tmx_->pairMap_["3"].second[tmpPos.y ][tmpPos.x- vecPairFire_[3].second] = 0;

			}
		}
	}

	


	

}

void Fire::Draw(void)
{

	Vector2 tmpPos = pos_ / 32;
	int tmpCnt =( animeCnt_ - 1) % 7;

	if (cnt_[0] > 0)
	{
		DrawGraph(pos_.x, pos_.y, IpImageMng.GetID("Fire")[vecfire_[tmpCnt]], true);

	}
	//auto DRAW_DIR = [&](Vector2 pos,int i,int no,) {
	//
	//	if (vecPairFire_[0].first)
	//	{
	//		if (i <= vecPairFire_[0].second - 1)
	//		{
	//			if (i == vecPairFire_[0].second - 1)
	//			{
	//				DrawRotaGraph3(pos.x, pos.y , 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 2], true, false);
	//			}
	//			else
	//			{
	//				DrawRotaGraph3(pos.x, pos.y , 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (i < vecPairFire_[0].second - 1)
	//		{
	//			if (i >= lengh_ - 1)
	//			{
	//				DrawRotaGraph3(pos.x, pos.y - 32 * (i), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 2], true, false);
	//			}
	//			else
	//			{
	//				DrawRotaGraph3(pos.x, pos.y - 32 * (i), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
	//			}
	//		}
	//	}
	//
	//
	//};
	
	for (int i =0; i < lengh_; i++)
	{

		if (cnt_[i] >= 0)
		{
			if (count_> i)
			{
				//è„
				if (vecPairFire_[0].first)
				{
					if (i <= vecPairFire_[0].second-1 )
					{
						if (i == vecPairFire_[0].second -1)
						{
							DrawRotaGraph3(pos_.x, pos_.y - 32 * (i), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 2], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y - i][tmpPos.x] = 1;
						}
						else
						{
							DrawRotaGraph3(pos_.x, pos_.y - 32 * (i), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y - i][tmpPos.x] = 1;
						}
					}
				}
				else
				{

					if (vecPairFire_[0].second == 0)
					{
						if (i >= lengh_ - 1)
						{
							DrawRotaGraph3(pos_.x, pos_.y - 32 * (i), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 2], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y - i][tmpPos.x] = 1;
						}
						else
						{
							DrawRotaGraph3(pos_.x, pos_.y - 32 * (i), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y - i][tmpPos.x] = 1;
						}
					}
					else
					{
						if (i <= vecPairFire_[0].second - 2)
						{

							DrawRotaGraph3(pos_.x, pos_.y - 32 * (i), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y - i][tmpPos.x] = 1;

						}
					}
				}
				
				//âE

				if (vecPairFire_[1].first)
				{
					if (i <= vecPairFire_[1].second - 1)
					{
						if (i == vecPairFire_[1].second - 1)
						{
							DrawRotaGraph3(pos_.x + 32 * (i + 1), pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 2], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y ][tmpPos.x+1] = 1;
						}
						else
						{
							DrawRotaGraph3(pos_.x + 32 * (i + 1), pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x + 1] = 1;
						}
					}
				}
				else
				{
					if (vecPairFire_[1].second == 0)
					{
						if (i >= lengh_ - 1)
						{
							DrawRotaGraph3(pos_.x + 32 * (i + 1), pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 2], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x + 1] = 1;

						}
						else
						{
							DrawRotaGraph3(pos_.x + 32 * (i + 1), pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x + 1] = 1;

						}
					}
					else
					{
						if (i <= vecPairFire_[1].second - 2)
						{

							DrawRotaGraph3(pos_.x + 32 * (i + 1), pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x + 1] = 1;


						}
					}
				}
				//â∫

				if (vecPairFire_[2].first)
				{
					if (i <= vecPairFire_[2].second - 1)
					{
						if (i == vecPairFire_[2].second - 1)
						{
							DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * (i + 1), 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 2], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y+ 1][tmpPos.x ] = 1;

						}
						else
						{
							DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * (i + 1), 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y + 1][tmpPos.x] = 1;

						}
					}
				}
				else
				{

					if (vecPairFire_[2].second == 0)
					{
						if (i >= lengh_ - 1)
						{
							DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * (i + 1), 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 2], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y + 1][tmpPos.x] = 1;

						}
						else
						{
							DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * (i + 1), 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y + 1][tmpPos.x] = 1;

						}
					}
					else
					{
						if (i <= vecPairFire_[2].second - 2)
						{

							DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * (i + 1), 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y + 1][tmpPos.x] = 1;


						}
					}


				}

				//ç∂
				if (vecPairFire_[3].first)
				{
					if (i <= vecPairFire_[3].second - 1)
					{
						if (i == vecPairFire_[3].second - 1)
						{
							DrawRotaGraph3(pos_.x - 32 * (i), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 2], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y ][tmpPos.x- 1] = 1;

						}
						else
						{
							DrawRotaGraph3(pos_.x - 32 * (i), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x - 1] = 1;

						}
					}
				}
				else
				{
					if (vecPairFire_[3].second == 0)
					{
						if (i >= lengh_ - 1)
						{
							DrawRotaGraph3(pos_.x - 32 * (i), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 2], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x - 1] = 1;

						}
						else
						{
							DrawRotaGraph3(pos_.x - 32 * (i), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x - 1] = 1;

						}
					}
					else
					{
						if (i <= vecPairFire_[3].second - 2)
						{

							DrawRotaGraph3(pos_.x - 32 * (i), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[tmpCnt] + 1], true, false);
							IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x - 1] = 1;

							
						}
					}

				}

			}
		}	

	}




	////è„
	//if (vecPairFire_[0].first)
	//{
	//	if (count_ > 1)
	//	{
	//		DrawRotaGraph3(pos_.x, pos_.y, 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + 2], true, false);
	//		IpNetwork.tmx_->checkMap_[tmpPos.y-1][tmpPos.x] = 1;
	//	}
	//}
	//else
	//{
	//	for (int i = 1; i <= vecPairFire_[0].second; i++)
	//	{
	//		if (count_ < 4)
	//		{
	//			if (count_ >= i)
	//			{
	//				DrawRotaGraph3(pos_.x, pos_.y - 32 * (i - 1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
	//				IpNetwork.tmx_->checkMap_[tmpPos.y -i][tmpPos.x] = 1;

	//			}
	//		}
	//		else
	//		{
	//			if (count_ < 5 + i)
	//			{
	//				DrawRotaGraph3(pos_.x, pos_.y - 32 * (i - 1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
	//			}
	//			else
	//			{
	//				if (vecPairFire_[0].second > 1)
	//				{
	//					i++;
	//					DrawRotaGraph3(pos_.x, pos_.y - 32 * (i - 1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ % 7] + i], true, false);

	//				}
	//			}
	//		}
	//	}
	//}



	////âE
	//if (vecPairFire_[1].first)
	//{
	//	if (count_ > 1)
	//	{
	//		DrawRotaGraph3(pos_.x + 32, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ % 7] + 2], true, false);
	//		IpNetwork.tmx_->checkMap_[tmpPos.y ][tmpPos.x+1] = 1;

	//	}
	//}
	//else
	//{
	//	for (int i = 1; i <= vecPairFire_[1].second; i++)
	//	{
	//		if (count_ < 4)
	//		{
	//			if (count_ >= i)
	//			{
	//				DrawRotaGraph3(pos_.x + 32 * i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ % 7] + i], true, false);
	//				IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x +i] = 1;

	//			}
	//		}
	//		else
	//		{
	//			if (count_ < 5+i)
	//			{
	//				DrawRotaGraph3(pos_.x + 32 *i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ % 7] + i], true, false);
	//			}
	//			else
	//			{
	//				if (vecPairFire_[1].second > 1)
	//				{
	//					i++;
	//					DrawRotaGraph3(pos_.x + 32 * i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ % 7] + i], true, false);
	//				}
	//			}
	//		}
	//	}
	//}

	//////â∫

	//if (vecPairFire_[2].first)
	//{
	//	if (count_ > 1)
	//	{
	//		DrawRotaGraph3(pos_.x + 32, pos_.y + 32, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + 2], true, false);
	//		IpNetwork.tmx_->checkMap_[tmpPos.y+ 1][tmpPos.x ] = 1;

	//	}
	//}
	//else
	//{
	//	for (int i = 1; i <= vecPairFire_[2].second; i++)
	//	{
	//		if (count_ < 4)
	//		{
	//			if (count_ >= i)
	//			{
	//				DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
	//				IpNetwork.tmx_->checkMap_[tmpPos.y +i][tmpPos.x] = 1;

	//			}
	//		}
	//		else
	//		{
	//			if (count_ <= 5 + i)
	//			{
	//				DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
	//			}
	//			else
	//			{
	//				if (vecPairFire_[2].second >= 1)
	//				{
	//					i++;
	//					DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
	//				}
	//			}
	//		}
	//	}
	//}



	////ç∂
	//if (vecPairFire_[3].first)
	//{
	//	if (count_ > 1)
	//	{
	//		DrawRotaGraph3(pos_.x, pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + 2], true, false);
	//		IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x - 1] = 1;

	//	}
	//}
	//else
	//{
	//	for (int i = 1; i <= vecPairFire_[3].second; i++)
	//	{
	//		if (count_ < 4)
	//		{
	//			if (count_ >= i)
	//			{
	//				DrawRotaGraph3(pos_.x - 32 * (i - 1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
	//				IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x -i] = 1;

	//			}
	//		}
	//		else
	//		{
	//			if (count_ <= 5 + i)
	//			{
	//				DrawRotaGraph3(pos_.x - 32 * (i - 1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
	//			}
	//			else
	//			{
	//				if (vecPairFire_[3].second >= 1)
	//				{
	//					i++;
	//					DrawRotaGraph3(pos_.x - 32 * (i - 1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_  % 7] + i], true, false);
	//				}
	//			}
	//		}
	//	}
	//}

}

bool Fire::isPacket()
{
	return false;
}

ObjID Fire::GetObjID()
{
	return ObjID();
}

void Fire::DrawInit()
{

	Drawfire_.try_emplace(0, [](int lengh_) {});
	Drawfire_.try_emplace(1, [](int lengh_) {});
	Drawfire_.try_emplace(2, [](int lengh_) {});
	Drawfire_.try_emplace(3, [](int lengh_) {});
	Drawfire_.try_emplace(4, [](int lengh_) {});
	Drawfire_.try_emplace(5, [](int lengh_) {});
	Drawfire_.try_emplace(6, [](int lengh_) {});


}
