#pragma once
#include "BaseScene.h"
#include"../common/Vector2.h"
#include "../common/Input/Controller.h"
class TitleScene :
    public BaseScene
{

public:
    TitleScene();
    ~TitleScene();
    unique_Base Update(unique_Base own) override;
    void Init(void);
    Vector2 screen_size_;
    void Draw(void);
    Vector2 pos_;
    Vector2 posOld_;
private:
    void Ctl(conType input);
};

