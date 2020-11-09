#pragma once
#include <array>
#include <map>
#include "InputID.h"
#include "conTypeID.h"

//“ü—Í‘•’u
//enum class conType
//{
//	Key,
//	Mouse,
//	Pad,
//	Max
//};
enum class Trg
{
	Now,
	Old,
	Max
};
enum class Player_No
{
	Player_1,
	Player_2,
	MAX
};


using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;
using CntData = std::map<InputID, TrgBool>;

struct Controller
{
	void operator()()
	{
		Updata();
	}
	
	const CntData& GetCntData(void)
	{
		return _data;
	}	


	virtual conType GetConType(void) = 0;
	virtual bool SetUp(void) = 0;
private:
	virtual void Updata(void) = 0;
protected:
	
	CntData _data;

};

