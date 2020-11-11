#include "CrossOver.h"
#include "LoginScene.h"
CrossOver::CrossOver(unique_Base before, unique_Base next)
{
	before_ = std::move(before);
	next_ = std::move(next);
		
	cnt_ = 0;
}

CrossOver::~CrossOver()
{
}

unique_Base CrossOver::Update(unique_Base own)
{


	DrawOwn();


	if (cnt_ <= 255)
	{
		cnt_+=5;
	}
	else
	{
		return next_->Update(std::move(next_));

	}



	return std::move(own);
}



void CrossOver::DrawOwn()
{

	SetDrawScreen(screenID_);


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	next_->DrawOwn();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - cnt_);
	before_->DrawOwn();


	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);



	ScreenFlip();





}
