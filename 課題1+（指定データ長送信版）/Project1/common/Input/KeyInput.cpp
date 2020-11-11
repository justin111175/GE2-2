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
			{InputID::Up,KEY_INPUT_UP},
			{InputID::Down,KEY_INPUT_DOWN},
			{InputID::Left,KEY_INPUT_LEFT},
			{InputID::Right,KEY_INPUT_RIGHT},
			{InputID::テスト用,KEY_INPUT_T},
			{InputID::Btn1,KEY_INPUT_SPACE},
			{InputID::Btn2,KEY_INPUT_V},
			{InputID::Btn3,KEY_INPUT_B},
			{InputID::Mean,KEY_INPUT_ESCAPE}
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
