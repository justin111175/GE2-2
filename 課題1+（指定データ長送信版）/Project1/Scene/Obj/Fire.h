#pragma once
#include "Obj.h"
#include "../BaseScene.h"
#include "../../InterNet/NetWork.h"

#define PI 3.1415926f
#define DEG (PI/180.0)

using PairFire = std::pair<bool, int>;

class Fire :
	public Obj
{
public:

	Fire(Vector2,int, BaseScene& scene);
	~Fire();
	void Update(void) override;

	void Draw(void)override;
	bool isPacket()override;
	ObjID GetObjID()override;

private:
	BaseScene& scene_;

	std::chrono::system_clock::time_point start_, end_;
	int count_;
	std::vector<PairFire> vecPairFire_;

	std::vector<int> vecfire_;
	int fireWidth_;

	VecMap pairMap_;
	int cnt_;
	int lengh_;

};

