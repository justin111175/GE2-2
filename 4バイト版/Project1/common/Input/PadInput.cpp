#include <Dxlib.h>
#include "PadInput.h"

conType PadInput::GetConType(void)
{
	return conType::Pad;
}

bool PadInput::SetUp(void)
{


		_inputTbl= {
			{InputID::Up, PAD_INPUT_UP},
			{InputID::Down, PAD_INPUT_DOWN},
			{InputID::Left, PAD_INPUT_LEFT},
			{InputID::Right, PAD_INPUT_RIGHT},
			{InputID::Btn1,PAD_INPUT_1},
			{InputID::Btn2,PAD_INPUT_2},
			{InputID::Btn3,PAD_INPUT_3},
			{InputID::Mean,PAD_INPUT_8}

		};



	return true;
}

void PadInput::Updata(void)
{
	_keyData= GetJoypadInputState(DX_INPUT_PAD1);
	
	for (auto id : InputID())
	{

			_data[id][static_cast<int>(Trg::Old)] = _data[id][static_cast<int>(Trg::Now)];
			_data[id][static_cast<int>(Trg::Now)] = _keyData & _inputTbl[id];

		
	}



}
