#pragma once
#include "Vector2.h"

struct Number
{
public:
	void Init(void);								// ������
	void Draw(Vector2 pos, Vector2Dbl exrate, int scor);		// �`��
private:
	int numberImage[10];							// �����摜�̕ۑ��p
	int numLen;										// ����
};

