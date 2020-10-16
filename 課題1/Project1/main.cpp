#include "Dxlib.h"
#include "Scene/SceneMng.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SceneMng::Create();
	SceneMng::Destroy();
	SceneMng::GetInstance().Run();

	return 0;															// ƒuƒƒOƒ‰ƒ€‚ÌI—¹
}