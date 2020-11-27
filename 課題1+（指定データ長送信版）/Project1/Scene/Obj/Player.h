#pragma once
#include "Obj.h"
#include "../BaseScene.h"
#include <list>
#include <utility>
#include <functional>



#define SPEED 4
#define UNIT_ID_BASE 5

class Player :
	public Obj
{
public:

	Player(Vector2 pos, int id,BaseScene& scene);
	~Player();
	void Update(void) override;

	void UpdateDef(conType input);
	void UpdateAuto(void);
	void UpdateNet(void);

	void Draw(void)override;
	void SendPos();
	void SendDeath();
	bool isPacket()override;
	void RevData();

	void RunCheck();

	int GetFallCnt();

	ObjID GetObjID()override;

	void InputInit();
	
	static int fallCnt_;

	bool MapCheck(int no);
private:
	Vector2 speed_;

	std::map<conType, std::unique_ptr<Controller>> controller;

	BaseScene& scene_;
	std::vector<int> mapDataBase_;
	std::vector<int*> mapData_;

	std::list<int> bombList_;


	std::list<std::function<bool(CntData,bool)>> InputList_;

	std::map<InputID, std::function<bool(void)>> CheckList_;

	DIR dir_;
	DIR Newdir_;

	int runCnt_;

	std::map<DIR, std::vector<int>> animePlayer_;

	std::chrono::system_clock::time_point start_, end_;


};

