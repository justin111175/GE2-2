#include <Dxlib.h>
#include "KeyInput.h"
#include "InputID.h"
conType KeyInput::GetConType(void)
{
	return conType::Key;
}

bool KeyInput::SetUp(void)
{

		_inputTbl = {
			{InputID::Up,KEY_INPUT_W},
			{InputID::Down,KEY_INPUT_S},
			{InputID::Left,KEY_INPUT_A},
			{InputID::Right,KEY_INPUT_D},
			{InputID::�e�X�g�p,KEY_INPUT_T},
			{InputID::Btn1,KEY_INPUT_C},
			{InputID::Btn2,KEY_INPUT_V},
			{InputID::Btn3,KEY_INPUT_B},
			{InputID::Mean,KEY_INPUT_SPACE}
		};


	return true;
}

void KeyInput::Updata(void)
{
	GetHitKeyStateAll(_keyData.data());

	for (auto id : InputID())
	{

		_data[id][static_cast<int>(Trg::Old)] = _data[id][static_cast<int>(Trg::Now)];
		_data[id][static_cast<int>(Trg::Now)] = _keyData[_inputTbl[id]];
		
	}
}
