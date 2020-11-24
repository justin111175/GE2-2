#pragma once
#include "Obj.h"

#include "../../InterNet/NetWork.h"

#define PI 3.1415926f
#define DEG (PI/180.0)

class Fire :
	public Obj
{
public:

	Fire(Vector2);
	~Fire();
	void Update(void) override;

	void Draw(void)override;
	bool isPacket()override;

private:
	std::chrono::system_clock::time_point start_, end_;
	int count_;


	std::vector<int> vecfire_;
	int fireWidth_;

	VecMap pairMap_;
	int cnt_;

};

