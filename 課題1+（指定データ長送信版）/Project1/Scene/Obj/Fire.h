#pragma once
#include "Obj.h"
#include "../BaseScene.h"
#include "../../InterNet/NetWork.h"

#define PI 3.1415926f
#define DEG (PI/180.0)

using PairFire = std::pair<bool, int>;

enum class FIRE_DIR
{
	RIGHT,
	DOWN,
	LEFT,
	UP

};

class Fire :
	public Obj
{
public:

	Fire(Vector2 pos, int lengh,bool flag, BaseScene& scene);
	Fire(Vector2 pos, double rad, int cnt, BaseScene& scene);
	~Fire();
	void Update(void) override;

	void Draw(void)override;
	bool isPacket()override;
	ObjID GetObjID()override;

	void SetFireInit();
private:
	std::list<unique_Obj> listFire_;
	std::map<FIRE_DIR, std::function<void(void)>> setFire_;
	BaseScene& scene_;
	bool flag_;
	std::chrono::system_clock::time_point start_, end_;

	int rad_;
	std::map<FIRE_DIR,int> lengh_;
	int count_;
	int cnt_;
	std::vector<int> vecfire_;
};

