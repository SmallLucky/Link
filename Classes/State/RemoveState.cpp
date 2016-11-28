#include "StateList.h"
#include "Data/GameData.h"

//����״̬

bool RemoveState::checkStateChange() //״̬�ı䷵��true��״̬δ�ı䷵��false��
{
	if (gameOverFlag)
	{
		CHANGE_TO_STATE(GameOverState); //��ʱ���þ����������Ϸ����״̬
	}
	if (IsLineBoom)
	{
		//CHANGE_TO_STATE(LineBoomState);
	}
	if (matrixFinishFlag && !scene->enoughTargetElement()) // && scene->showScoreUp() < GameData::getInstance()->getTargetScore(GameData::getInstance()->getCurLevel())
		CHANGE_TO_STATE(LinkState); //��Ԫ����������ɣ������ȴ�����״̬
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
	scene->showScoreUp(); //��ʾ��������
	scene->showAllCount(); //��ʾ�����ĸ���
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
	//	//gameOverFlag = true; //��ʱ���þ�������Ϸ����
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
		gameOverFlag = true; //�������þ�������Ϸ����
	}
	matrixTime += dt;
	if (matrixTime > FALL_TIME)
	{
		//log("(matrixTime > FALL_TIME)");
		matrixTime = 0;
		matrixFinishFlag = !scene->elementsFall(); //��������λ��ֱ�����������
	}
}

bool RemoveState::exitState()
{
	//if (scene->getIsLine())
	//{
	//	scene->forbiddenLink(); //��������
	//}
	return true;
}