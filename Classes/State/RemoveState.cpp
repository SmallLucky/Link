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
		CHANGE_TO_STATE(LineBoomState);
	}
	if (matrixFinishFlag)
		CHANGE_TO_STATE(LinkState); //��Ԫ����������ɣ������ȴ�����״̬
	if (gameStartFlag)
	{
		CHANGE_TO_STATE(StartState);
	}

	return false;
}

bool RemoveState::entryState()
{
	scene->showScoreUp(); //��ʾ��������
	scene->showAllCount(); //��ʾ�����ĸ���
	matrixFinishFlag = false;
	matrixTime = FALL_TIME;

	gameOverFlag = false;
	gameStartFlag = false;
	IsLineBoom = false;
	return true;
}

void RemoveState::onUpdate(float dt)
{
	//time += dt;
	//if (scene->timeLapseShow(dt) <= 0)
	//{
	//	//gameOverFlag = true; //��ʱ���þ�������Ϸ����
	//	log("gameOverFlag");
	//}
	if (scene->showScoreUp() >=GameData::getInstance()->getTargetScore(GameData::getInstance()->getCurLevel()))
	{
		log("score");
		//GAMEDATA->setCurLevel(GAMEDATA->getCurLevel()+1);
		//gameStartFlag = true;
		scene->gameNextLevel();
		
	}
	if (scene->getMyCount()<= 0)
	{
		log("gameOverFlag");
		gameOverFlag = true; //�������þ�������Ϸ����
	}
	matrixTime += dt;
	if (matrixTime > FALL_TIME)
	{
		matrixTime = 0;
		matrixFinishFlag = !scene->elementsFall(); //��������λ��ֱ�����������
	}
	if (scene->getIsLine())
	{
		IsLineBoom = true;
	}
}

bool RemoveState::exitState()
{
	if (scene->getIsLine())
	{
		scene->forbiddenLink(); //��������
	}
	return true;
}