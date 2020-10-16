#pragma once
#include "NetWorkState.h"

class HostState :
    public NetWorkState
{
public:
    HostState();
    virtual ~HostState();
    virtual NetWorkMode GetMode(void) { return NetWorkMode::HOST; };
    bool CheckNetWork()override;
};