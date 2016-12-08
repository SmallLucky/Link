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
	if (matrixFinishFlag)
	{
		if (score < GAMEDATA->getTargetScore(GAMEDATA->getCurLevel()) || !scene->enoughTargetElement())
		{
			//log("score3:%d", score);
			if (count <= 0)
			{
				scene->addCountLayer();
			}
			CHANGE_TO_STATE(LinkState); //若元素已下落完成，则进入等待连线状态
		}
		else
		{
			cout << "特殊元素都滿足" << endl;
			//EventCustom _event(FINISHTARGETELEMENT);
			//_eventDispatcher->dispatchEvent(&_event);
		}
	}
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
	score = scene->showScoreUp(); //显示分数增加
	log("score:%d",score);
	count = scene->showAllCount(); //显示步数的更改
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
	if (matrixFinishFlag)
	if (score >= GAMEDATA->getTargetScore(GAMEDATA->getCurLevel()) )
	{
		log("score2:%d :: %d", score, GAMEDATA->getTargetScore(GAMEDATA->getCurLevel()));
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
	if (matrixTime > 0.1 )//FALL_TIME
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