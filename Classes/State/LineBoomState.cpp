#include "StateList.h"

bool LineBoomState::checkStateChange() //状态改变返回true，状态未改变返回false，
{
	if (matrixFinishFlag)
	{
		CHANGE_TO_STATE(LinkState); //若时间用尽，则进入游戏结束状态
	}
	return false;
}

bool LineBoomState::entryState()
{
	scene->MyShowScoreUp(); //显示分数增加
	//scene->allowLink(); //允许触摸

	scene->forbiddenLink(); //不允许触摸
	matrixFinishFlag = false;
	matrixTime = FALL_TIME;

	gameStartFlag = false;

	return true;
}

void LineBoomState::onUpdate(float dt)
{
	if (scene->showScoreUp() >= GameData::getInstance()->getTargetScore(GameData::getInstance()->getCurLevel()))
	{
		log("score");
		//GAMEDATA->setCurLevel(GAMEDATA->getCurLevel()+1);
		//gameStartFlag = true;
		scene->gameNextLevel();

	}
	matrixTime += dt;
	if (matrixTime > FALL_TIME)
	{
		matrixTime = 0;
		matrixFinishFlag = !scene->elementsFall(); //不断填充空位，直至矩阵填充满
	}
}

bool LineBoomState::exitState()
{
	return true;
}