#include "Player.h"
#include "../SceneMng.h"
#include "../../InterNet/NetWork.h"
#include "../GameScene.h"
#include"../../common/Input/KeyInput.h"
#include "../../common/Input/PadInput.h"
#include "../../common/_debug/_DebugDispOut.h"
#include "../../common/_debug/_DebugConOut.h"
int Player::fallCnt_ = 0;

Player::Player(Vector2 pos, int id, BaseScene& scene)
	:scene_(scene)
{

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
}

Player::~Player()
{


}

void Player::Update(void)
{

	mapDataBase_ = IpNetwork.tmx_->pairMap_["3"].first;
	mapData_ = IpNetwork.tmx_->pairMap_["3"].second;

	if (IpNetwork.GetNetWorkMode() == NetWorkMode::HOST)
	{
		if (id_ != 5)
		{
			if (id_ == 0)
			{
				UpdateDef(conType::Key);

			}
			else
			{
				UpdateAuto();
			}
		}
	}
	else if (IpNetwork.GetNetWorkMode() == NetWorkMode::GEST)
	{
		if (id_ != 0)
		{
			if (id_ == 5)
			{
				UpdateDef(conType::Key);
			}
			else
			{
				UpdateAuto();
			}
		}
	}
	else if(IpNetwork.GetNetWorkMode() == NetWorkMode::OFFLINE)
	{
		if (id_ == 0)
		{
			UpdateDef(conType::Key);

		}
		else
		{
			//UpdateAuto();
		}
	}

}

void Player::UpdateDef(conType input)
{

	(*controller[input])();
	int count;

	auto keyData = controller[input]->GetCntData();

	for (auto Inputdata = InputList_.begin(); Inputdata != InputList_.end(); Inputdata++)
	{
		if ((*Inputdata)(keyData, true))
		{
			InputList_.splice(InputList_.begin(), InputList_, Inputdata);
			InputList_.sort([&](auto testA, auto testB) {return testA(keyData, false) < testB(keyData, false);});
			break;
		}

	}


	SendData();



}

void Player::UpdateAuto(void)
{
	if (IpNetwork.GetNetWorkMode() == NetWorkMode::HOST)
	{
		if (id_ % 2 == 0)
		{
			SendData();

			RunCheck();
		}
		else
		{
			//RevData();

		}
	}
	else if(IpNetwork.GetNetWorkMode() == NetWorkMode::GEST)
	{
		if (id_ % 2 == 1)
		{
			SendData();

			RunCheck();
		}
		else
		{
			//RevData();
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

	DrawGraph(pos_.x , pos_.y - 30 , IpImageMng.GetID("Play")[(static_cast<int>(dir_) + 5 * (2 + IpSceneMng.frames() / 10 % 2))], true);

}

void Player::SendData()
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

	IpNetwork.SendMes(MesType::POS, data);

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

	Vector2 tmpPos_;
	switch (dir_)
	{
	case DIR::Up:
		if (mapData_[(pos_.y) / 32][(pos_.x) / 32] != 0)
		{
			tmpPos_.y = pos_.y / 32 + 1;
			pos_.y = tmpPos_.y * 32;
			dir_ = DIR::Right;
		}
		else
		{
			runCnt_ = 3;
			speed_ = { 0,-SPEED };
			pos_ += speed_;
		}
		break;
	case DIR::Right:
		if (mapData_[(pos_.y) / 32][(pos_.x) / 32 + 1] != 0)
		{
			tmpPos_.x = (pos_.x) / 32;
			pos_.x = tmpPos_.x * 32;
			dir_ = DIR::Down;
		}
		else
		{
			runCnt_ = 2;
			speed_ = { SPEED,0 };
			pos_ += speed_;
		}
		break;
	case DIR::Down:
		if (mapData_[(pos_.y) / 32 + 1][(pos_.x) / 32] != 0)
		{
			tmpPos_.y = pos_.y / 32;
			pos_.y = tmpPos_.y * 32;
			dir_ = DIR::Left;
		}
		else
		{
			runCnt_ = 0;
			speed_ = { 0,SPEED };
			pos_ += speed_;
		}
		break;
	case DIR::Left:
		if (mapData_[(pos_.y) / 32][(pos_.x) / 32] != 0)
		{
			tmpPos_.x = pos_.x / 32 + 1;
			pos_.x = tmpPos_.x * 32;
			dir_ = DIR::Up;

		}
		else
		{
			runCnt_ = 1;
			speed_ = { -SPEED,0 };
			pos_ += speed_;

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
			//if(pos_.x < tmpPos_.x * 32-32 )
			//{
			//	pos_.x = tmpPos_.x * 32;
			//}
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


		//if (mapData_[(pos_.y) / 32+1][(pos_.x) / 32] != 0)
		//{
		//	tmpPos_.x = pos_.x / 32 ;
		//	pos_.x = tmpPos_.x * 32;
		//	pos_ = { (tmpPos_.x = pos_.x / 32) * 32,pos_.y };
		//}


		return true;


		});




	InputList_.emplace_back([&](CntData data, bool flag) {

		if (data[InputID::Down][static_cast<int>(Trg::Now)])
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
		return false;

		});


	InputList_.emplace_back([&](CntData data, bool flag) {

		if (data[InputID::Up][static_cast<int>(Trg::Now)])
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
		return false;

		});
	InputList_.emplace_back([&](CntData data, bool flag) {

		if (data[InputID::Right][static_cast<int>(Trg::Now)])
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
		return false;

		});
	InputList_.emplace_back([&](CntData data, bool flag) {

		if (data[InputID::Left][static_cast<int>(Trg::Now)])
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
					dynamic_cast<GameScene&>(scene_).SetBomb(id_, id_ + 1, pos_, std::chrono::system_clock::now(), true);

				}


			}
			return true;
		}
		return false;

		});


}


