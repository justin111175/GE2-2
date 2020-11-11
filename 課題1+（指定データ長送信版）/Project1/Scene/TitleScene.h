#pragma once
#include "BaseScene.h"
#include"../common/Vector2.h"
#include "../common/Input/Controller.h"
#include <functional>
#include <Dxlib.h>
#include <array>
#include "../InterNet/NetWork.h"
#include <vector>


//using std::array << array, IPDATA >> ARRAYIP;
#define CHIP_MAX_X 32
#define CHIP_MAX_Y 32


class TitleScene :
    public BaseScene
{

public:
    TitleScene();
    ~TitleScene();

    unique_Base Update(unique_Base own) override;					// çXêV
    void Init(void);
    Vector2 screen_size_;
    Vector2 pos_;
    Vector2 posOld_;


private:
    void Draw();
    void DrawOwn() override;



    void Ctl(conType input);
    //UpdataMode mode_;
    //std::map<UpdataMode, std::function<void(void)>> func_;



    std::vector<std::string> split(std::string& input, char delimiter);

    std::map<std::string, std::vector<int>> mapData_;
    

};

