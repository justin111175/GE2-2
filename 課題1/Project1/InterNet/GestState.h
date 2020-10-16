#pragma once
#include "NetWorkState.h"

class GestState :
    public NetWorkState
{
public:
    GestState();
    virtual ~GestState();

    virtual NetWorkMode GetMode(void) { return NetWorkMode::GEST; };
    bool ConnectHost(IPDATA hostIP);
    bool CheckNetWork()override;
};

