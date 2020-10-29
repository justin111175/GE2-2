#pragma once
#include <memory>
#include <String>
#include "Dxlib.h"
#include "../common/Input/Controller.h"
// クラスのプロトタイプ宣言
class BaseScene;

using unique_Base = std::unique_ptr<BaseScene>;							// スマートポインタ-BaseScene


class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own) = 0;					// 純粋仮想関数

protected:
	std::map<conType, std::unique_ptr<Controller>> controller;
};


