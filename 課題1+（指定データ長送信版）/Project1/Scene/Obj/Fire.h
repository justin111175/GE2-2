#pragma once
#include "Obj.h"
#include "../BaseScene.h"
#include "../../InterNet/NetWork.h"

#define PI 3.1415926f
#define DEG (PI/180.0)

using PairFire = std::pair<bool, int>;

enum class FIRE_DIR
{
	’†ŠÔ,
	UP,
	RIGHT,
	DOWN,
	LEFT
};

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
	void DrawInit();
private:
	BaseScene& scene_;

	std::chrono::system_clock::time_point start_, end_;
	std::vector<PairFire> vecPairFire_;

	std::vector<int> vecfire_;
	int fireWidth_;

	int lengh_;
	int animeCnt_;
	int count_;
	std::vector<int> cnt_;
	int tmpCnt_;
	std::map<int, std::function<void(int)>> Drawfire_;
};

