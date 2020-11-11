#pragma once
#include <memory>
#include <String>
#include "Dxlib.h"
#include "../common/Input/Controller.h"
#include "../common/ImageMng.h"

enum class SceneChange
{
	Title_GO_Login,
	Login_GO_Game



};



class BaseScene;

using unique_Base = std::unique_ptr<BaseScene>;							// スマートポインタ-BaseScene





class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own)=0;

	
	virtual void Draw();
	virtual void DrawOwn() = 0;

protected:
	std::map<conType, std::unique_ptr<Controller>> controller;
	int screenID_;


};


