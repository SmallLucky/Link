#include "TimeUtil.h"
#include <iostream>

#define FIRSTTIME "firsttime"
#define TIMECOUNT "timecount"
#define SIGNEDTODAY "signedtoday"

TimeUtil::TimeUtil()
{

}

TimeUtil::~TimeUtil()
{

}

void TimeUtil::init()
{
	std::string timemark = "timemark";
	auto userdef = UserDefault::getInstance();

	int key = userdef->getIntegerForKey(timemark.c_str(), -99);
	if (key == -99){
		//TimeUtil::setFirstTime(TimeUtil::getCurTime());
		TimeUtil::setFirstTime(43253);
		TimeUtil::setSignedToday(false);
		userdef->setIntegerForKey(timemark.c_str(),1);
		userdef->flush();
		std::cout << "����ʱ��:"<<getCurTime() << std::endl;

		userdef->setIntegerForKey(TIMECOUNT, 0);
		userdef->flush();
	}
}

void TimeUtil::setFirstTime(int curDay)
{
	auto dfData = UserDefault::getInstance();
	dfData->setStringForKey(FIRSTTIME, Value(curDay).asString());
	dfData->flush();
}

int TimeUtil::getFirstTime()
{
	int firstTime = atoi(UserDefault::getInstance()->getStringForKey(FIRSTTIME, "0").c_str());
	if (firstTime >0){
		return firstTime;
	}
	return 0;
}

int TimeUtil::getCurTime()
{
	return CommonFunction::getCurDay();
}
//�Ƿ��µ�һ��
bool TimeUtil::isNewDay()
{
	int currTime = TimeUtil::getCurTime();
	int saveTime = TimeUtil::getFirstTime();
	if (currTime != saveTime){
		TimeUtil::setSignedToday(false);
		return true;
	}
	return false;
}
//ǩ��������
int TimeUtil::getTimeCount()	
{
	auto dfData = UserDefault::getInstance();
	int timecount = dfData->getIntegerForKey(TIMECOUNT,-1);
	if (timecount >= 0){
		return timecount;
	}
	return 0;
}
//ǩ����һ�죬���
void TimeUtil::addTimeCount()	
{
	int timecount = TimeUtil::getTimeCount();
	auto dfData = UserDefault::getInstance();
	dfData->setIntegerForKey(TIMECOUNT, timecount + 1);
	dfData->flush();
}
//�����Ƿ�ǩ��
bool TimeUtil::getSignedToday()
{
	auto dfData = UserDefault::getInstance();
	bool isSigned = dfData->getBoolForKey(SIGNEDTODAY,false);
	if (isSigned){
		return true;
	}
	return false;
}
//���ý����Ƿ�ǩ��
void TimeUtil::setSignedToday(bool sign)
{
	auto dfData = UserDefault::getInstance();
	dfData->setBoolForKey(SIGNEDTODAY, sign);
	dfData->flush();
	//std::cout << "ǩ����" << std::endl;
}
