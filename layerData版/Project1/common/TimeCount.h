#pragma once
#include <map>
#include <iostream> 
#define _timeCount TimeCount::GetInstance()

class TimeCount
{
public:

	static TimeCount& GetInstance()
	{
		static TimeCount s_Instance;
		return s_Instance;
	}
	void Run(void);
	bool Set(const char* string,bool flag, double cnt);

	const bool GetFlag(const char* string);
	const double GetCnt(const char* string);

private:
	TimeCount();
	~TimeCount();

	std::map<std::string, bool> _flag;
	std::map<std::string, double> _cnt;
};

