#pragma once
#include "BaseScene.h"
#include "CrossOver.h"

class Transition :
	public BaseScene
{
public:
	unique_Base Update(unique_Base own) override;					// çXêV

	Transition(unique_Base own, SceneChange scene_);
	~Transition();


private:
	void DrawOwn() override;
	unique_Base childScene_[2];										
	int cnt_;


};

