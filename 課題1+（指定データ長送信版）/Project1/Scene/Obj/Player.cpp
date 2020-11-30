#include "Player.h"
#include "../SceneMng.h"
#include "../../InterNet/NetWork.h"
#include "../GameScene.h"
#include"../../common/Input/KeyInput.h"
#include "../../common/Input/PadInput.h"
#include "../../common/_debug/_DebugDispOut.h"
#include "../../common/_debug/_DebugConOut.h"
int Player::fallCnt_ = 0;

Player::Player(Vector2 pos, int id, BaseScene& scene,int playerID)
	:scene_(scene)
{
	if (id == playerID)
	{
		playerType_ = playerType::自分;
	}
	else
	{
		playerType_ = playerType::相手;
	}



	pos_ = { 32 * pos.x, 32 * pos.y };

	IpImageMng.GetID("Play", "image/bomberman.png", { 32,205 / 4 }, { 5,4 });
	dir_ = DIR::Right;
	runCnt_ = 0;
	speed_ = { 4 ,0 };

	id_ = id;

	controller.try_emplace(conType::Key, std::make_unique<KeyInput>());
	controller[conType::Key]->SetUp();

	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());
	controller[conType::Pad]->SetUp();
	isAlive = true;
	isDeath = false;
	
	InputInit();


	animePlayer_.try_emplace(DIR::Up);

	animePlayer_[DIR::Up].emplace_back(8);
	animePlayer_[DIR::Up].emplace_back(13);
	animePlayer_[DIR::Up].emplace_back(8);
	animePlayer_[DIR::Up].emplace_back(18);




	animePlayer_.try_emplace(DIR::Right);
	animePlayer_[DIR::Right].emplace_back(7);
	animePlayer_[DIR::Right].emplace_back(12);
	animePlayer_[DIR::Right].emplace_back(7);
	animePlayer_[DIR::Right].emplace_back(17);

	animePlayer_.try_emplace(DIR::Down);

	animePlayer_[DIR::Down].emplace_back(5);
	animePlayer_[DIR::Down].emplace_back(10);
	animePlayer_[DIR::Down].emplace_back(5);
	animePlayer_[DIR::Down].emplace_back(15);

	animePlayer_.try_emplace(DIR::Left);
	animePlayer_[DIR::Left].emplace_back(6);
	animePlayer_[DIR::Left].emplace_back(11);
	animePlayer_[DIR::Left].emplace_back(6);
	animePlayer_[DIR::Left].emplace_back(16);

	animePlayer_.try_emplace(DIR::Death);

	animePlayer_[DIR::Death].emplace_back(4);
	animePlayer_[DIR::Death].emplace_back(9);
	animePlayer_[DIR::Death].emplace_back(14);
	animePlayer_[DIR::Death].emplace_back(19);
	animePlayer_[DIR::Death].emplace_back(-1);

	zorder_ = 0;
	objID_ = ObjID::PLAYER;
}

Player::~Player()
{


}

void Player::Update(void)
{


	mapDataBase_ = IpNetwork.tmx_->pairMap_["3"].first;
	mapData_ = IpNetwork.tmx_->pairMap_["3"].second;
	Vector2 tmpPos = pos_ / 32;
	
	MesPacket data_ = IpNetwork.GetPacket(MesType::DEATH);


	if (isAlive)
	{




		if (data_.size())
		{
			if (id_ == data_[0].iData)
			{
				runCnt_ = 0;
				isAlive = false;
			}
		}


		switch (playerType_)
		{
		case playerType::自分:
			if (IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x] == 1)
			{
				SendDeath();
				runCnt_ = 0;

				isAlive = false;
			}
			UpdateDef(conType::Key);

			break;
		case playerType::相手:
			RevData();

			break;
		default:
			break;
		}
		//if (IpNetwork.GetNetWorkMode() == NetWorkMode::HOST)
		//{

		//	if (id_ != 5)
		//	{

		//		if (IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x] == 1)
		//		{
		//			SendDeath();
		//			runCnt_ = 0;

		//			isAlive = false;
		//		}

		//		if (id_ == 0)
		//		{
		//			if (IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x] == 1)
		//			{
		//				runCnt_ = 0;
		//				isAlive = false;
		//			}
		//			UpdateDef(conType::Key);

		//		}
		//		else
		//		{
		//			UpdateAuto();
		//		}
		//	}
		//}
		//else if (IpNetwork.GetNetWorkMode() == NetWorkMode::GEST)
		//{
		//	if (id_ != 0)
		//	{

		//		if (IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x] == 1)
		//		{
		//			SendDeath();
		//			runCnt_ = 0;

		//			isAlive = false;
		//		}

		//		if (id_ == 5)
		//		{

		//			UpdateDef(conType::Key);
		//		}
		//		else
		//		{
		//			UpdateAuto();

		//		}
		//	}
		//}
		//else if(IpNetwork.GetNetWorkMode() == NetWorkMode::OFFLINE)
		//{
		//	if (IpNetwork.tmx_->checkMap_[tmpPos.y][tmpPos.x] == 1)
		//	{
		//		SendDeath();
		//		runCnt_ = 0;
		//		isAlive = false;
		//	}
		//	if (id_ == 0)
		//	{
		//		UpdateDef(conType::Key);

		//	}
		//	else
		//	{
		//		UpdateAuto();
		//	}
		//}
	}
	else
	{



		end_ = std::chrono::system_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count() > std::chrono::milliseconds(1000/2).count())
		{

			start_ = std::chrono::system_clock::now();
			runCnt_++;
			if (runCnt_ >= 4)
			{
				isDeath = true;
			}
		}


	}


}

void Player::UpdateDef(conType input)
{

	(*controller[input])();

	auto keyData = controller[input]->GetCntData();

	for (auto Inputdata = InputList_.begin(); Inputdata != InputList_.end(); Inputdata++)
	{
		if ((*Inputdata)(keyData, true))
		{
			runCnt_++;

			InputList_.splice(InputList_.begin(), InputList_, Inputdata);
			InputList_.sort([&](auto testA, auto testB) {return testA(keyData, false) < testB(keyData, false);});
			break;
		}

	}
	

	SendPos();



}

void Player::UpdateAuto(void)
{
	if (IpNetwork.GetNetWorkMode() == NetWorkMode::HOST)
	{
		if (id_ % 2 == 0)
		{
			SendPos();

			RunCheck();
		}
		else
		{
			RevData();

		}
	}
	else if(IpNetwork.GetNetWorkMode() == NetWorkMode::GEST)
	{
		if (id_ % 2 == 1)
		{
			SendPos();

			RunCheck();

		}
		else
		{
			RevData();
		}
	}
	else
	{
		RunCheck();
	}
}

void Player::UpdateNet(void)
{




}

void Player::Draw(void)
{

	if (id_==0)
	{
		_dbgDrawBox(pos_.x, pos_.y, pos_.x + 32 , pos_.y + 32 , 0xFFFFF, false);
	}
	_dbgDrawFormatString(pos_.x, pos_.y, 0xFFFFFF, "%d", id_);

	if (isAlive)
	{
		DrawGraph(pos_.x , pos_.y - 30 , IpImageMng.GetID("Play")[animePlayer_[dir_][runCnt_/10%4]], true);

	}
	else
	{
		DrawGraph(pos_.x, pos_.y-30 , IpImageMng.GetID("Play")[animePlayer_[DIR::Death][runCnt_ % 4]], true);

	}

}

void Player::SendPos()
{
	MesPacket data;
	UnionData unionD;

	unionD.iData = id_;
	data.insert(data.end(), unionD);

	unionD.iData = pos_.x;
	data.insert(data.end(), unionD);

	unionD.iData = pos_.y;
	data.insert(data.end(), unionD);

	unionD.iData = static_cast<int>(dir_);
	data.insert(data.end(), unionD);

	IpNetwork.SendMes(MesType::POS, data,IpNetwork.GetHandle());

}

void Player::SendDeath()
{

	MesPacket data;
	UnionData unionD;

	unionD.iData = id_;
	data.insert(data.end(), unionD);



	IpNetwork.SendMes(MesType::DEATH, data, IpNetwork.GetHandle());


}

bool Player::isPacket()
{
	MesPacket data;
	data = IpNetwork.GetPacket(id_);
	if (data.size())
	{
		id_ = data[0].iData;
		pos_ = { data[1].iData ,data[2].iData };
		dir_ = static_cast<DIR>(data[3].iData);
		//fallCnt_ = 0;
		return true;
	}
	else
	{
		fallCnt_++;
		return false;

	}
}

void Player::RevData()
{
	MesPacket data;
	data = IpNetwork.GetPacket(id_);
	if (data.size())
	{
		id_ = data[0].iData;
		pos_ = { data[1].iData ,data[2].iData };
		dir_ = static_cast<DIR>(data[3].iData);
		fallCnt_ = 0;
	}
	else
	{
		fallCnt_++;
	}
}

void Player::RunCheck()
{
	runCnt_++;

	Vector2 tmpPos_;
	switch (dir_)
	{
	case DIR::Up:
		if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32] == -1)
		{
			if (mapData_[(pos_.y) / 32][(pos_.x) / 32] != 0)
			{
				tmpPos_.y = pos_.y / 32 + 1;
				pos_.y = tmpPos_.y * 32;
				dir_ = DIR::Right;
			}
			else
			{
				speed_ = { 0,-SPEED };
				pos_ += speed_;
			}
		}
		break;
	case DIR::Right:
		if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32 + 1] == -1)
		{
			if (mapData_[(pos_.y) / 32][(pos_.x) / 32 + 1] != 0)
			{
				tmpPos_.x = (pos_.x) / 32;
				pos_.x = tmpPos_.x * 32;
				dir_ = DIR::Down;
			}
			else
			{
				speed_ = { SPEED,0 };
				pos_ += speed_;
			}
		}
		break;
	case DIR::Down:
		if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32 + 1][(pos_.x) / 32] == -1)
		{
			if (mapData_[(pos_.y) / 32 + 1][(pos_.x) / 32] != 0)
			{
				tmpPos_.y = pos_.y / 32;
				pos_.y = tmpPos_.y * 32;
				dir_ = DIR::Left;
			}
			else
			{
				speed_ = { 0,SPEED };
				pos_ += speed_;
			}
		}
		break;
	case DIR::Left:
		if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32] == -1)
		{
			if (mapData_[(pos_.y) / 32][(pos_.x) / 32] != 0)
			{
				tmpPos_.x = pos_.x / 32 + 1;
				pos_.x = tmpPos_.x * 32;
				dir_ = DIR::Up;

			}
			else
			{
				speed_ = { -SPEED,0 };
				pos_ += speed_;

			}
		}
		break;

	default:
		break;
	}
}



int Player::GetFallCnt()
{
	return fallCnt_;

}

ObjID Player::GetObjID()
{
	return objID_;
}



void Player::InputInit()
{
	Vector2 tmpPos_;
	CheckList_.try_emplace(InputID::Down, [&]() {


			if (mapData_[(pos_.y + 32) / 32][(pos_.x) / 32] != 0)
			{
				if (pos_.x > (pos_.x) / 32 * 32 + 16)
				{
					pos_.x = (pos_.x) / 32 * 32 + 32;
				}

				return false;
			}

			if (mapData_[(pos_.y + 32) / 32][(pos_.x+31) / 32] != 0)
			{
				if(pos_.x < (pos_.x + 31) / 32*32)
				{
					pos_.x = (pos_.x ) / 32*32;
				}

				return false;
			}

			return true;
		
		});

	CheckList_.try_emplace(InputID::Up, [&]() {

		if (mapData_[(pos_.y-4 ) / 32][(pos_.x) / 32] != 0)
		{
			if (pos_.x > (pos_.x) / 32 * 32 + 16)
			{
				pos_.x = (pos_.x) / 32 * 32 + 32;
			}

			return false;
		}

		if (mapData_[(pos_.y-4) / 32][(pos_.x + 31) / 32] != 0)
		{
			if (pos_.x < (pos_.x + 31) / 32 * 32)
			{
				pos_.x = (pos_.x) / 32 * 32;
			}

			return false;
		}

		return true;


		});
	CheckList_.try_emplace(InputID::Right, [&]() {

		if (mapData_[(pos_.y) / 32][(pos_.x) / 32 + 1] != 0)
		{
			if (pos_.y > (pos_.y) / 32 * 32 + 16)
			{
				pos_.y = (pos_.y) / 32 * 32+32;
			}

			return false;
		}

		if (mapData_[(pos_.y+31) / 32][(pos_.x) / 32 + 1] != 0)
		{
			if (pos_.y < (pos_.y+30) / 32 * 32)
			{
				pos_.y = (pos_.y) / 32 * 32;
			}

			return false;
		}

		return true;


		});
	CheckList_.try_emplace(InputID::Left, [&]() {

		if (mapData_[(pos_.y) / 32][(pos_.x-4) / 32 ] != 0)
		{
			if (pos_.y > (pos_.y) / 32 * 32 + 16)
			{
				pos_.y = (pos_.y) / 32 * 32 + 32;
			}

			return false;
		}

		if (mapData_[(pos_.y + 31) / 32][(pos_.x-4) / 32 ] != 0)
		{
			if (pos_.y < (pos_.y + 30) / 32 * 32)
			{
				pos_.y = (pos_.y) / 32 * 32;
			}

			return false;
		}

		return true;


		});




	InputList_.emplace_back([&](CntData data, bool flag) {

		if (data[InputID::Down][static_cast<int>(Trg::Now)])
		{
			if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32 + 1][(pos_.x) / 32] == -1)
			{
				if (CheckList_[InputID::Down]())
				{
					if (flag)
					{
						pos_.y += SPEED;
						dir_ = DIR::Down;

					}
					return true;
				}
			}


		}
		return false;

		});


	InputList_.emplace_back([&](CntData data, bool flag) {

		if (data[InputID::Up][static_cast<int>(Trg::Now)])
		{
			if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32] == -1)
			{
			
				if (CheckList_[InputID::Up]())
				{
					if (flag)
					{
						pos_.y -= SPEED;
						dir_ = DIR::Up;


					}

					return true;
				}
			}

		}
		return false;
		});

	InputList_.emplace_back([&](CntData data, bool flag) {

		if (data[InputID::Right][static_cast<int>(Trg::Now)])
		{
			if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32 + 1] == -1)
			{
				if (CheckList_[InputID::Right]())
				{
					if (flag)
					{
						pos_.x += SPEED;

						dir_ = DIR::Right;

					}
					return true;
				}
			}

		}

		return false;

		});
	InputList_.emplace_back([&](CntData data, bool flag) {

		if (data[InputID::Left][static_cast<int>(Trg::Now)])
		{
			if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32] == -1)
			{
				if (CheckList_[InputID::Left]())
				{
					if (flag)
					{
						pos_.x -= SPEED;
						dir_ = DIR::Left;


					}
					return true;
				}
			}

		}
		return false;

	});

	InputList_.emplace_back([&](CntData data, bool flag) {
		int cnt_;
		cnt_ = dynamic_cast<GameScene&>(scene_).GetBombSize(id_);

		if (data[InputID::テスト用][static_cast<int>(Trg::Now)] && !data[InputID::テスト用][static_cast<int>(Trg::Old)])
		{
			if (flag)
			{
				if (cnt_ - 1 < 4)
				{
					dynamic_cast<GameScene&>(scene_).SetBomb(id_, id_ + 1, pos_,6, std::chrono::system_clock::now(), true);

				}


			}
			return true;
		}
		return false;

		});


}

bool Player::MapCheck(int no)
{
	
	switch (dir_)
	{
	case DIR::Up:
		if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32] == no)
		{
			return false;
		}

		break;
	case DIR::Right:
		if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32 + 1] == no)
		{
			return false;

		}

		break;
	case DIR::Down:
		if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32 + 1][(pos_.x) / 32] == no)
		{
			return false;

		}

		break;
	case DIR::Left:
		if (IpNetwork.tmx_->checkMap_[(pos_.y) / 32][(pos_.x) / 32] == no)
		{
			return false;

		}

		break;

	default:
		break;
	}

	return true;

}


