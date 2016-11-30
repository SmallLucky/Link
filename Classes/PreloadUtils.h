#pragma once

#include "cocos2d.h"
USING_NS_CC;

class PreloadUtils
{
public:
	static PreloadUtils* getInstance();
	static bool purgeInstance(); 
	PreloadUtils();
	~PreloadUtils();

public:
	void preloadBg();
	void preloadKuang();
private:
	static PreloadUtils* _instance;
	bool _isNeedPreload;
};