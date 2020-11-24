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

	pairMap_ = IpNetwork.tmx_->GetMap();
	TRACE("FIRE_TEST");
	cnt_ = 0;
	
	Vector2 tmpPos = pos_ / 32;
	if (IpNetwork.tmx_->layer_["3"][tmpPos.y * 21 + tmpPos.x + 1] == 8)
	{	

		IpNetwork.tmx_->layer_["3"][tmpPos.y * 21 + tmpPos.x + 1] = 0;

		if (IpNetwork.tmx_->layer_["3"][tmpPos.y * 21 + tmpPos.x + 1] == 0)
		{
			cnt_++;

		}
	}
	else
	{
		if (IpNetwork.tmx_->layer_["3"][tmpPos.y * 21 + tmpPos.x + 1] == 0)
		{
			cnt_++;
			if (IpNetwork.tmx_->layer_["3"][tmpPos.y * 21 + tmpPos.x + 2] == 0)
			{
				cnt_++;
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

	//if (IpNetwork.tmx_->layer_["3"][tmpPos.y * 21 + tmpPos.x + 1] == 8)
	//{
	//	IpNetwork.tmx_->layer_["3"][tmpPos.y * 21 + tmpPos.x + 1] = 0;
	//}






	




	//count_=50;







	if (count_ < 10 * 7)
	{
		count_++;


	}
	else
	{
		isAlive = false;

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



	for (int i = 1; i <= 2; i++)
	{
		if (count_ <= 40)
		{

			if (count_ > -10+10 * i)
			{
				//è„
				DrawRotaGraph3(pos_.x, pos_.y-32*(i-1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
				//âE
				DrawRotaGraph3(pos_.x + 32 * i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
				//â∫
				DrawRotaGraph3(pos_.x + 32, pos_.y + 32 * i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
				//ç∂
				DrawRotaGraph3(pos_.x - 32*(i-1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
			}
		}
		else
		{
			if (count_ < 50 + i * 10)
			{
				//è„
				DrawRotaGraph3(pos_.x, pos_.y - 32 * (i - 1), 0, 0, 1.0f, 1.0f, 270 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
				//âE
				DrawRotaGraph3(pos_.x + 32 * i, pos_.y, 0, 0, 1.0f, 1.0f, 0, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);

				//â∫
				DrawRotaGraph3(pos_.x + 32, pos_.y + 32*i, 0, 0, 1.0f, 1.0f, 90 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
				//ç∂
				DrawRotaGraph3(pos_.x - 32 * (i - 1), pos_.y + 32, 0, 0, 1.0f, 1.0f, 180 * DEG, IpImageMng.GetID("Fire")[vecfire_[count_ / 10 % 7] + i], true, false);
			}
		}
	}



}

bool Fire::isPacket()
{
	return false;
}
