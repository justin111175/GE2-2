#pragma once
#include "common/Vector2.h"
#include <string>
#include <vector>
#include <map>
enum class DIR
{
	Down,
	Left,
	Right,
	Up,
	Deth,
	MAX
};

#define SPEED 4

class Character
{
public:
	Character(std::vector<int> mapDataBase, std::vector<int*> mapData,Vector2 pos,int id);
	~Character();
	void Draw();
	void Updata();
	void SendData();
	void RevData();
	void RunCheck();

private:
	std::vector<int> mapDataBase_;
	std::vector<int*> mapData_;

	DIR dir_;
	
	int runCnt_;
	Vector2 speed_;
	Vector2 pos_;
	int id_;
};

