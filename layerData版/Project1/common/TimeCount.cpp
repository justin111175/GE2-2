#include "TimeCount.h"
#include "../Scene/SceneMng.h"

void TimeCount::Run(void)
{
	
	auto type = [&](const char* string) {	
		if (_flag[string])
		{
			if (_cnt[string] > 0)
			{
				_cnt[string]-=3;
			}
			else
			{
				_flag[string] = false;
			}
		}
	};
	
	type("連鎖");
	type("連鎖落下");
	type("待つ");


}

bool TimeCount::Set(const char* string,bool flag, double cnt)
{
	_flag[string] = flag;
	_cnt[string] = cnt*60;

	return true;
}

const bool TimeCount::GetFlag(const char* string)
{
	return _flag[string];
}

const double TimeCount::GetCnt(const char* string)
{
	return _cnt[string];
}

TimeCount::TimeCount()
{
	auto add = [&](std::string string) {
		_flag.try_emplace(string, false);
		_cnt.try_emplace(string, 0);

	};
	
	add("連鎖");
	add("連鎖落下");
	add("待つ");

}

TimeCount::~TimeCount()
{
}
