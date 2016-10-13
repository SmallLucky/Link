#include "StateList.h"

//开始状态

bool StartState::checkStateChange() //状态改变返回true，状态未改变返回false，
{
	if (time > 1)
		CHANGE_TO_STATE(InitState); //进入初始化状态
	return false;
}

bool StartState::entryState()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("StartState::entryState()");
#endif
	scene->initBackground(); //初始化背景
	scene->initMatrix(); //初始化矩阵
	scene->initInformation(); //初始化信息

	return true;
}

void StartState::onUpdate(float dt)
{
	time += dt;
}

bool StartState::exitState()
{
	return true;
}