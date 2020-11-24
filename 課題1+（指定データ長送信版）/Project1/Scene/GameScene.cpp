#include "GameScene.h"
#include "../common/_debug/_DebugDispOut.h"
#include "Obj/Player.h"
#include "Obj/Fire.h"

void GameScene::Draw()
{	
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawOwn();

}

void GameScene::DrawOwn()
{




	//DrawGraph(0, 0, IpImageMng.GetID("2")[0], true);

	IpNetwork.tmx_->Draw();



	for (auto &&data : objList_)
	{
		data->Draw();
	}

}

void GameScene::SetBomb(int ownID, int selfID, Vector2 pos, std::chrono::system_clock::time_point time, bool sendFlag)
{
	MesPacket data;
	UnionData unionD;

	UnionTime time_ = { time };

	unionD.iData = ownID;

	data.insert(data.end(), unionD);

	unionD.iData = selfID;
	data.insert(data.end(), unionD);

	unionD.iData = pos.x;
	data.insert(data.end(), unionD);

	unionD.iData = pos.y;
	data.insert(data.end(), unionD);

	unionD.iData = time_.iData[0];
	data.insert(data.end(), unionD);

	unionD.iData = time_.iData[1];
	data.insert(data.end(), unionD);

	if (sendFlag)
	{
		IpNetwork.SendMes(MesType::SET_BOMB, data);

	}


	bomb_ = { pos ,ownID,selfID ,true ,time_.start_ };
	
	objList_.emplace_back(std::make_unique<Bomb>(bomb_,*this));


}

void GameScene::SetFire(Vector2 pos)
{

	objList_.emplace_back(std::make_unique<Fire>(pos));


}

void GameScene::DeathEraser()
{

	auto itr = std::remove_if(objList_.begin(), objList_.end(), [](unique_Obj& obj) {return obj->GetAlive() == false; });

	if (itr != objList_.end())
	{
		objList_.erase(itr, objList_.end());
	}
}

int GameScene::GetBombSize(int ownID_)
{
	auto count = std::count_if(objList_.begin(), objList_.end(), [&](unique_Obj& obj) {return obj->GetID() == ownID_; });

	return count;
}

void GameScene::RevBomb()
{

	MesPacket data_;
	data_=IpNetwork.GetPacket(MesType::SET_BOMB);
	UnionTime time = { std::chrono::system_clock::now() };

	if (data_.size())
	{
		time.iData[0] = data_[4].iData;
		time.iData[1] = data_[5].iData;

		SetBomb(data_[0].iData, data_[1].iData, { data_[2].iData,data_[3].iData }, time.start_,false);
	}


	

}

unique_Base GameScene::Update(unique_Base own)
{

	Draw();

	objList_.sort([](unique_Obj& playA, unique_Obj& playB) {return playA->isPacket() > playA->isPacket(); });


	for (auto&& data : objList_)
	{
		data->Update();
	}

	end_ = std::chrono::system_clock::now();

	if (std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count() > std::chrono::milliseconds(1000).count())
	{
		start_ = end_;
		count_++;

		Cnt_ = Player::fallCnt_/ count_;
		Player::fallCnt_ = 0;
	}


	_dbgDrawFormatString(0, 600, 0xFFFFFF, "BombSize : %d", GetBombSize(0)-1);

	_dbgDrawFormatString(0, 550, 0xFFFFFF, "FallCnt : %d", Cnt_);


	RevBomb();

	IpNetwork.EraserPac();
	DeathEraser();

	return std::move(own);

}

GameScene::GameScene()
{
	IpImageMng.GetID("2", "image/2.jpg", { 800,600 }, { 1,1 });

	if (IpNetwork.GetNetWorkMode() == NetWorkMode::OFFLINE)
	{
		IpNetwork.tmx_->LoadTmx("map/ObjTest.tmx");

	}

	std::vector<Vector2> plPos;
	mapDataBase_.resize((__int64)21 * 17);
	for (size_t no = 0; no < 17; no++)
	{
		mapData_.emplace_back(&mapDataBase_[no * 21]);
	}
	for (int i = 0; i < IpNetwork.tmx_->layer_["3"].size(); i++)
	{
		mapDataBase_[i] = IpNetwork.tmx_->layer_["3"][i];

	}

	int cntID_ = 0;
	for (int i = 0; i < IpNetwork.tmx_->layer_["4"].size(); i++)
	{
		if (IpNetwork.tmx_->layer_["4"][i] == 4)
		{
			objList_.emplace_back(std::make_unique<Player>(mapDataBase_, mapData_, Vector2{ i % 21,i / 21 }, cntID_, *this));
			cntID_+= UNIT_ID_BASE;
			IpNetwork.tmx_->layer_["4"][i] = 0;

		}

	}


	bomb_.initFlag_ = false;
	count_ = 0;

}

GameScene::~GameScene()
{
}


