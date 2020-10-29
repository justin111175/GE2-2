#pragma once
#include "Vector2.h"

struct Number
{
public:
	void Init(void);								// ‰Šú‰»
	void Draw(Vector2 pos, Vector2Dbl exrate, int scor);		// •`‰æ
private:
	int numberImage[10];							// ”š‰æ‘œ‚Ì•Û‘¶—p
	int numLen;										// Œ…”
};

