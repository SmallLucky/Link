#include "PreloadUtils.h"
#include <iostream>

using namespace std;

PreloadUtils* PreloadUtils::_instance = nullptr;

PreloadUtils* PreloadUtils::getInstance()
{
	if (!_instance)
	{
		_instance = new PreloadUtils();
	}
	return _instance;
}

bool PreloadUtils::purgeInstance()
{
	CCASSERT(_instance, "instance can not empty");
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
	return true;
}

PreloadUtils::PreloadUtils() :_isNeedPreload(true)
{

}

PreloadUtils::~PreloadUtils()
{
	_isNeedPreload = false;
}

void PreloadUtils::preloadBg()
{
	TextureCache * cache = TextureCache::getInstance();
	for (int i = 0; i < 9; i++){
		string str = StringUtils::format("bg/levelbg/bg_00%d.png", i);
		cache->addImageAsync(str, [=](Texture2D* tex){
			cout << "ok..."<< endl;
		});
	}
}

void PreloadUtils::preloadKuang()
{
	TextureCache * cache = TextureCache::getInstance();
	string str = "signed/main/kuang_img.png";
	string str1 = "signed/reward/bg_img.png";
	cache->addImageAsync(str, [=](Texture2D* tex){});
	cache->addImageAsync(str1, [=](Texture2D* tex){});
}