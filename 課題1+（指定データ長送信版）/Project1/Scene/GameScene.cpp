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


	IpNetwork.tmx_->Draw();

	objList_.sort([](unique_Obj& playA, unique_Obj& playB) {return playA->GetZorder() > playB->GetZorder(); });


	for (auto &&data : objList_)
	{
		data->Draw();
	}




}

void GameScene::GetPlayerPac()
{
	MesPacket data_;
	data_ = IpNetwork.GetPacket(MesType::ID);

	if (data_.size())
	{
		playerPac_.first = data_[0].iData;
		playerPac_.second = data_[1].iData;

	}
}

void GameScene::GetStartTime()
{

	MesPacket data_;
	data_ = IpNetwork.GetPacket(MesType::START_TIME);
	UnionTime time_ = { std::chrono::system_clock::now() };

	if (data_.size())
	{
		time_.iData[0] = data_[0].iData;
		time_.iData[1] = data_[1].iData;
		start_time_ = time_.start_;

		start_time_ += std::chrono::milliseconds(10000);
	}


}

void GameScene::EraserLost()
{
	MesPacket data_;
	data_ = IpNetwork.GetPacket(MesType::LOST);

	if (data_.size())
	{
		lostID_ = data_[0].iData;

		auto itr = std::remove_if(objList_.begin(), objList_.end(), [&](unique_Obj& obj) {return obj->GetID() == lostID_; });

		if (itr != objList_.end())
		{
			objList_.erase(itr, objList_.end());
		}

	}






}

void GameScene::SetBomb(int ownID, int selfID, Vector2 pos, int lengh, std::chrono::system_clock::time_point time, bool sendFlag)
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

	unionD.iData = lengh;
	data.insert(data.end(), unionD);

	unionD.iData = time_.iData[0];
	data.insert(data.end(), unionD);

	unionD.iData = time_.iData[1];
	data.insert(data.end(), unionD);

	if (sendFlag)
	{
		IpNetwork.SendMes(MesType::SET_BOMB, data, IpNetwork.GetHandle());

	}


	bomb_ = { pos ,ownID,selfID ,true ,lengh,time_.start_ };
	
	objList_.emplace_back(std::make_unique<Bomb>(bomb_,*this));


}

void GameScene::SetFire(Vector2 pos,int lengh)
{

	objList_.emplace_back(std::make_unique<Fire>(pos, lengh,*this));


}

void GameScene::DeathEraser()
{

	auto itr = std::remove_if(objList_.begin(), objList_.end(), [](unique_Obj& obj) {return obj->GetIsDeath() == true; });

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
		time.iData[0] = data_[5].iData;
		time.iData[1] = data_[6].iData;

		SetBomb(data_[0].iData, data_[1].iData, { data_[2].iData,data_[3].iData }, data_[4].iData, time.start_,false);
	}


	

}



unique_Base GameScene::Update(unique_Base own)
{
	Draw();

	GetStartTime();





	end_ = std::chrono::system_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(start_time_ - end_).count() / 1000;
	_dbgDrawFormatString(0, 700, 0xFFFFFF, "ŠJŽnŽžŠÔ : %d", time);

	IpNetwork.EraserPac();
	DeathEraser();
	EraserLost();


	if (time > 0)
	{
		return own;

	}




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


	_dbgDrawFormatString(0, 600, 0xFFFFFF, "BombSize : %d", GetBombSize(playerPac_.first)-1);

	_dbgDrawFormatString(0, 550, 0xFFFFFF, "FallCnt : %d", Cnt_);


	RevBomb();

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

	GetPlayerPac();
	GetStartTime();


	int cntID_ = 0;
	for (int i = 0; i < IpNetwork.tmx_->pairMap_["4"].first.size(); i++)
	{
	
		if (IpNetwork.tmx_->pairMap_["4"].first[i] == 4)
		{
			if (cntID_ < playerPac_.second*5)
			{
				objList_.emplace_back(std::make_unique<Player>( Vector2{ i % 21,i / 21 }, cntID_, *this, playerPac_.first));

			}
			cntID_+= UNIT_ID_BASE;
			IpNetwork.tmx_->pairMap_["4"].first[i] = 0;

		}

	}


	bomb_.initFlag_ = false;
	count_ = 0;
	lostID_ = -1;

}

GameScene::~GameScene()
{
}


