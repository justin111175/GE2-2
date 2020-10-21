#pragma once
#include "BaseScene.h"
#include"../common/Vector2.h"
#include "../common/Input/Controller.h"
#include <functional>
#include <Dxlib.h>
#include <array>
#include "../InterNet/NetWork.h"
#include <vector>
#include "../ChipMng.h"

enum class UpdataMode
{
    SetNetWork,
    GetHostIP,
    StartInit,
    Play


};

enum class Layer
{
    CHAR,
    OBJ,
    ITEAM,
    BG



};


enum class MapState
{
    Non,
    ��1,
    ��2,
    ���e,
    Non2,
    ��1,
    ��2,
    ��3,
    �A�C�e��1,
    �A�C�e��2,
    �A�C�e��3,
    �A�C�e��4,
    MAX

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

    std::map<Layer,std::unique_ptr<ChipMng>> chipState_;


    std::vector<MapState> _dataBase;
    std::vector<MapState*> _data;
    void Tmx();


};

