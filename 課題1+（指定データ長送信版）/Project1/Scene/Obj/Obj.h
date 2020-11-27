#pragma once
#include <memory>
#include <vector>
#include "../../common/Vector2.h"
#include "../../common/ImageMng.h"
#include "Dxlib.h"
#include "../../common/_debug/_DebugConOut.h"
#include "../../common/_debug/_DebugDispOut.h"
class Obj;

enum class DIR
{
	Down,
	Left,
	Right,
	Up,
	Death,
	MAX
};
using unique_Obj = std::unique_ptr<Obj>;							// スマートポインタ-BaseScene

enum class ObjID
{
	PLAYER,
	FIRE,
	MAX
};

class Obj
{
public:
	Obj();
	virtual~Obj();

	virtual void Update(void) = 0;

	
	virtual void Draw(void) = 0;
	virtual ObjID GetObjID() = 0;

	virtual bool isPacket()=0;

	Vector2 GetPos();
	bool GetAlive();
	bool GetIsDeath();
	int GetID();
	int GetZorder();
private:



protected:
	int zorder_;
	Vector2 pos_;
	ObjID objID_;
	int id_;
	bool isAlive;
	bool isDeath;

};

