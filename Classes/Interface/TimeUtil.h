#pragma once
#include "cocos2d.h"
#include "CommonFunction.h"

using namespace cocos2d;

class TimeUtil
{
public:
	TimeUtil();
	~TimeUtil();

	static void init();
	static bool isNewDay();

	static int getCurTime();

	static void setFirstTime(int curDay);
	static int getFirstTime();

	static int getTimeCount();
	static void addTimeCount();

	static bool getSignedToday(); 
	static void setSignedToday(bool sign);
};
