#pragma once
#include "NetWorkState.h"
class OffLine :
	public NetWorkState
{
public:
    OffLine();
    virtual ~OffLine();
    NetWorkMode GetMode(void) { return NetWorkMode::OFFLINE; };
};

