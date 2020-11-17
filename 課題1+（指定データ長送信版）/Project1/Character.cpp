#include "Character.h"
#include "common/ImageMng.h"
#include <Dxlib.h>
#include "Scene/SceneMng.h"
#include "InterNet/NetWork.h"
	
int Character::fallCnt_ = 0;

Character::Character(std::vector<int> mapDataBase, std::vector<int*> mapData, Vector2 pos, int id)
{
	pos_ = { 32*pos.x, 32*pos.y };

	IpImageMng.GetID("Play", "image/bomberman.png", { 32,205/4 }, { 5,4 });
	dir_ = DIR::Right;
	runCnt_ = 0;
	speed_ = { 4 ,0};
	mapDataBase_= mapDataBase;
	mapData_= mapData;

	id_ = id;
}

Character::~Character()
{
}

void Character::Draw()
{
	DrawGraph(pos_.x, pos_.y-30, IpImageMng.GetID("Play")[(static_cast<int>(dir_)+5*(2+IpSceneMng.frames()/10%2))], true);
	if (id_ == 0)
	{
		DrawCircle(pos_.x, pos_.y, 5, 0x000000, true);
	}

}

void Character::Updata()
{

	if (IpNetwork.GetNetWorkMode() == NetWorkMode::HOST)
	{
		if (id_ %2==0)
		{
			SendData();

			RunCheck();
		}
		else
		{
			RevData();

		}
	}
	else
	{
		if (id_%2 == 1)
		{
			SendData();

			RunCheck();
		}
		else
		{
			RevData();
		}
	}


}

void Character::SendData()
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

void Character::RevData()
{
	MesPacket data;
	data = IpNetwork.GetPacket(id_);
	if (data.size())
	{
		id_ = data[0].iData;
		pos_ = { data[1].iData ,data[2].iData};
		dir_ = static_cast<DIR>(data[3].iData);

	}
	else
	{
		fallCnt_++;
	}


}

void Character::RunCheck()
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
