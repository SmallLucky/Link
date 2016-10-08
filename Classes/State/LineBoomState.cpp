#include "StateList.h"

bool LineBoomState::checkStateChange() //״̬�ı䷵��true��״̬δ�ı䷵��false��
{
	if (matrixFinishFlag)
	{
		CHANGE_TO_STATE(LinkState); //��ʱ���þ����������Ϸ����״̬
	}
	return false;
}

bool LineBoomState::entryState()
{
	scene->MyShowScoreUp(); //��ʾ��������
	//scene->allowLink(); //������

	scene->forbiddenLink(); //��������
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
		matrixFinishFlag = !scene->elementsFall(); //��������λ��ֱ�����������
	}
}

bool LineBoomState::exitState()
{
	return true;
}