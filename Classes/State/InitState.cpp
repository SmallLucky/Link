#include "StateList.h"

//初始化阶段

bool InitState::checkStateChange() //状态改变返回true，状态未改变返回false，
{
	if (matrixFinishFlag)
		CHANGE_TO_STATE(LinkState); //若矩阵已填充完成，则进入等待连线状态
	return false;
}

bool InitState::entryState()
{
	scene->initGame(); //初始化一局游戏
	scene->initPlayers(); //初始化玩家
	scene->initMaster(); //设置主控玩家

	matrixFinishFlag = false;
	matrixTime = FALL_TIME;
	time = 0;
	return true;
}

void InitState::onUpdate(float dt)
{
	time += dt;
	matrixTime += dt;
	if (matrixTime > FALL_TIME)
	{
		matrixTime = 0;
		matrixFinishFlag = !scene->elementsFall(); //有一层下落结束后， 开始下落下一层，直至矩阵填充满
	}
}

bool InitState::exitState()
{
	return true;
}