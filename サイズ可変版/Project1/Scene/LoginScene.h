#pragma once
#include "BaseScene.h"
#include "../InterNet/NetWork.h"
class LoginScene :
	public BaseScene
{
public:
	unique_Base Update(unique_Base own) override;					// �X�V

	LoginScene();
	~LoginScene();

private:
	void DrawOwn() override;

};

