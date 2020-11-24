#pragma once
#include <memory>
#include <vector>
#include "../../common/Vector2.h"
#include "../../common/ImageMng.h"
#include "Dxlib.h"
#include "../../common/_debug/_DebugConOut.h"
#include "../../common/_debug/_DebugDispOut.h"
class Obj;

using unique_Obj = std::unique_ptr<Obj>;							// スマートポインタ-BaseScene



class Obj
{
public:
	Obj();
	virtual~Obj();

	virtual void Update(void) = 0;

	
	virtual void Draw(void) = 0;

	virtual bool isPacket()=0;

	bool GetAlive();
	int GetID();
private:



protected:
	Vector2 pos_;

	int id_;
	bool isAlive;
	bool isDeath;

};

