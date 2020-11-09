#pragma once
#include <memory>
#include <map>
#include <vector>
#include "Vector2.h"
#include <string>

using VecInt = std::vector<int>;

#define IpImageMng ImageMng::GetInstance()

#define IMAGE_ID(KEY)(ImageMng::GetInstance().GetID(KEY))
#define IMAGE_SOUND(KEY)(ImageMng::GetInstance().GetSound(KEY))


class ImageMng
{
public:
	//特殊なシングルトン
	static ImageMng &GetInstance(void)
	{
		return *sInstance;
	}
	
	// &参照
	const VecInt& GetID(const std::string&key);
	const VecInt& GetID(const std::string&key, const std::string & fileName);
	const VecInt& GetID(const std::string&key, const std::string & fileName, const Vector2 & divSize, const Vector2 & divCnt);

	const VecInt& GetSound(const std::string&key);
	const VecInt& GetSound(const std::string&key, const std::string & fileName);


private:
	struct ImageMngDeleter
	{
		//operator()は種類
		void operator()(ImageMng* imageMng)const
		{
			delete imageMng;
		}
	};
	
	ImageMng();
	~ImageMng();
	
	//テンプレート,デリダは関数オフジェクト
	static std::unique_ptr <ImageMng,ImageMngDeleter> sInstance;
	
	// map 連想配列,std::string  Key
	std::map<std::string, VecInt> imgMap;
	std::map<std::string, VecInt> soundMng;


};


