#include "StateList.h"
#include "Data/GameData.h"

//消除状态

bool RemoveState::checkStateChange() //状态改变返回true，状态未改变返回false，
{
	if (gameOverFlag)
	{
		CHANGE_TO_STATE(GameOverState); //若时间用尽，则进入游戏结束状态
	}
	if (IsLineBoom)
	{
		//CHANGE_TO_STATE(LineBoomState);
	}
	if (matrixFinishFlag && !scene->enoughTargetElement()) // && scene->showScoreUp() < GameData::getInstance()->getTargetScore(GameData::getInstance()->getCurLevel())
		CHANGE_TO_STATE(LinkState); //若元素已下落完成，则进入等待连线状态
	if (gameStartFlag)
	{
		CHANGE_TO_STATE(StartState);
	}

	return false;
}

bool RemoveState::entryState()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("RemoveState::entryState()");
#endif
	scene->showScoreUp(); //显示分数增加
	scene->showAllCount(); //显示步数的更改
	matrixFinishFlag = false;
	matrixTime = FALL_TIME;

	gameOverFlag = false;
	gameStartFlag = false;
	IsLineBoom = false;     
	//log("RemoveState::entryState()");
	return true;
}

void RemoveState::onUpdate(float dt)
{
	//log("RemoveState::onUpdate(float dt)");
	//time += dt;
	//if (scene->timeLapseShow(dt) <= 0)
	//{
	//	//gameOverFlag = true; //若时间用尽，则游戏结束
	//	log("gameOverFlag");
	//}
	if (matrixFinishFlag)
	if (scene->showScoreUp() >= GAMEDATA->getTargetScore(GAMEDATA->getCurLevel()) )
	{
		log("score");
		if (scene->enoughTargetElement() && matrixFinishFlag)
		{
			scene->gameNextLevel();
		}
	}
	if (scene->getIsGameOver())
	{
		gameOverFlag = true; //若步数用尽，则游戏结束
	}
	matrixTime += dt;
	if (matrixTime > FALL_TIME)
	{
		//log("(matrixTime > FALL_TIME)");
		matrixTime = 0;
		matrixFinishFlag = !scene->elementsFall(); //不断填充空位，直至矩阵填充满
	}
}

bool RemoveState::exitState()
{
	//if (scene->getIsLine())
	//{
	//	scene->forbiddenLink(); //不允许触摸
	//}
	return true;
}