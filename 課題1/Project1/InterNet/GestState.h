#pragma once
#include "NetWorkState.h"

class GestState :
    public NetWorkState
{
public:
    GestState();
    virtual ~GestState();

    virtual NetWorkMode GetMode(void) { return NetWorkMode::GEST; };
    ActiveState ConnectHost(IPDATA hostIP);
    bool CheckNetWork()override;
    

private:
};

