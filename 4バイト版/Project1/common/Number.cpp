#include "Number.h"
#include "ImageMng.h"
#include <Dxlib.h>

// 初期化
void Number::Init(void)
{
	IpImageMng.GetID("数字1", "image/Number/Number3.png", { 45,50 }, { 10,1 });
	numLen = 1;
}

// 描画
void Number::Draw(Vector2 pos,Vector2Dbl exrate, int score)
{
	// 初期値1桁目
	numLen = 1;

	//桁数チェック
	for (int i = 10; score >= i; i *= 10)
	{
		numLen++;
	}

	//数字描画
	for (int i = 0; i < numLen; i++)
	{
		//IpSceneMng.AddDrawQue({ IMAGE_ID("数字1")[score % 10], {pos.x - 90 * exrate.x * i,pos.y} ,{0,0},{exrate.x,exrate.y},0,false,0,LAYER::NUMBER });
		DrawRotaGraph3(pos.x - 45 * exrate.x * i, pos.y, 0, 0, exrate.x, exrate.y, 0, IMAGE_ID("数字1")[score % 10], true, false);
		score /= 10;
	}

	// scoreの値で事前チェック
	if (score <= 0)
	{
		return;
	}

}