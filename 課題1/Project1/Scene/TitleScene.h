#pragma once
#include "BaseScene.h"
#include"../common/Vector2.h"
#include "../common/Input/Controller.h"
#include <functional>

enum class UpdataMode
{
    SetNetWork,
    GetHostIP,
    StartInit,
    Play


};

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

    void SetNetWork();
    void GetHostIp();
    void StartInit();

    void Play();
private:
    void Ctl(conType input);
    UpdataMode mode_;
    std::map<UpdataMode, std::function<void(void)>> func_;
};

