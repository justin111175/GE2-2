#pragma once
#include "BaseScene.h"
#include "../InterNet/NetWork.h"
class LoginScene :
	public BaseScene
{
public:
	unique_Base Update(unique_Base own) override;					// çXêV

	LoginScene();
	~LoginScene();

private:
	void DrawOwn() override;

};

