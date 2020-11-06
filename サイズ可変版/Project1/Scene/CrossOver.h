#pragma once
#include "BaseScene.h"
class CrossOver :
	public BaseScene
{
public:
	CrossOver(unique_Base before, unique_Base next);
	~CrossOver();

	unique_Base Update(unique_Base own) override;					// çXêV

private:
	void DrawOwn() override;
	unique_Base before_;
	unique_Base next_;
	int cnt_;
};

