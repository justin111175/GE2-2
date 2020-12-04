#pragma once
#include "BaseScene.h"
#include "../InterNet/NetWork.h"
#include <list>
//#include "../Id/LoginMean_ID.h"

enum class LoginMean_ID
{
	HOST,
	GEST,
	GEST_BEFORE,
	NON

};
enum class IN_OUT
{
	Out,
	In,
	MAX
};

class LoginScene :
	public BaseScene
{
public:
	unique_Base Update(unique_Base own) override;					// �X�V
	void  GetHostIP();
	LoginScene();
	~LoginScene();

private:
	void GetCount_Down();
	void GetTmx();
	Vector2 screen_size_;

	void Ctl(conType input);
	void InCtl(conType input);

	UpdataMode mode_;

	void HOST();
	void GEST();
	void B_GEST();

	std::map<UpdataMode, std::function<void(void)>> func_;
	std::chrono::system_clock::time_point end_, start_time_;

	void SetNetWork();
	void GetHostIp();
	void StartInit();
	void Play();
	std::map<LoginMean_ID, const char*> mean_;
	LoginMean_ID meanID_;


	std::vector<std::string> VectorIP_;
	int hostIpCnt_;
	IPDATA ipData;
	void Draw();
	void DrawOwn() override;

	char Name[31];
	bool testFlag_;
	
	IN_OUT intOut_;
};

