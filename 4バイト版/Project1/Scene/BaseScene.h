#pragma once
#include <memory>
#include <String>
#include "Dxlib.h"
#include "../common/Input/Controller.h"
// �N���X�̃v���g�^�C�v�錾
class BaseScene;

using unique_Base = std::unique_ptr<BaseScene>;							// �X�}�[�g�|�C���^-BaseScene


class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own) = 0;					// �������z�֐�

protected:
	std::map<conType, std::unique_ptr<Controller>> controller;
};


