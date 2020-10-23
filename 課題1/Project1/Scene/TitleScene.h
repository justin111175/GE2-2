#pragma once
#include "BaseScene.h"
#include"../common/Vector2.h"
#include "../common/Input/Controller.h"
#include <functional>
#include <Dxlib.h>
#include <array>
#include "../InterNet/NetWork.h"
#include <vector>
#include "../tmx/TmxObj.h"

enum class UpdataMode
{
    SetNetWork,
    GetHostIP,
    StartInit,
    Play


};



//using std::array << array, IPDATA >> ARRAYIP;
#define CHIP_MAX_X 32
#define CHIP_MAX_Y 32


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
    void ChipInit();
private:
    void Ctl(conType input);
    UpdataMode mode_;
    std::map<UpdataMode, std::function<void(void)>> func_;

    std::unique_ptr<TmxObj> tmx_;

    //std::vector<MapState> _dataBase;
    //std::vector<MapState*> _data;

    std::vector<std::string> split(std::string& input, char delimiter);

    std::map<std::string, std::vector<int>> mapData_;
    void Tmx();


};

