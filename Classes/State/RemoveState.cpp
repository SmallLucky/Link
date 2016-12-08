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
	if (matrixFinishFlag)
	{
		if (score < GAMEDATA->getTargetScore(GAMEDATA->getCurLevel()) || !scene->enoughTargetElement())
		{
			//log("score3:%d", score);
			if (count <= 0)
			{
				scene->addCountLayer();
			}
			CHANGE_TO_STATE(LinkState); //��Ԫ����������ɣ������ȴ�����״̬
		}
		else
		{
			cout << "����Ԫ�ض��M��" << endl;
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
	score = scene->showScoreUp(); //��ʾ��������
	log("score:%d",score);
	count = scene->showAllCount(); //��ʾ�����ĸ���
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
		gameOverFlag = true; //�������þ�������Ϸ����
	}
	matrixTime += dt;
	if (matrixTime > 0.1 )//FALL_TIME
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