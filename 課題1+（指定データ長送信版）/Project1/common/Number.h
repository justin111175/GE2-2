#pragma once
#include "Vector2.h"

struct Number
{
public:
	void Init(void);								// 初期化
	void Draw(Vector2 pos, Vector2Dbl exrate, int scor);		// 描画
private:
	int numberImage[10];							// 数字画像の保存用
	int numLen;										// 桁数
};

