#include "StateList.h"

//�ȴ�����״̬

bool LinkState::checkStateChange() //״̬�ı䷵��true��״̬δ�ı䷵��false��
{
	if (gameOverFlag)
	{
		CHANGE_TO_STATE(GameOverState); //��ʱ���þ����������Ϸ����״̬
	}
	else if (scene->isLinkFinish())
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		LOGD("CHANGE_TO_STATE(RemoveState); ");
#endif
		CHANGE_TO_STATE(RemoveState); //�����һ�����ߣ����������״̬
	}
	else if (refreshElement)
	{
		log("need refresh!!!");//��ˢ�·��������ߴ���һ��ˢ��״̬
		//CHANGE_TO_STATE(GameOverState);
		if (scene->isResponse())
		{
			scene->refreshElement();
		}	
	}
	return false;
}

bool LinkState::entryState()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("LinkState::entryState() ");
#endif
	scene->allowLink(); //������
	scene->prepareLink(); //Ϊ����׼��

	gameOverFlag = false;
	refreshElement = false;
	
 	return true;
}

void LinkState::onUpdate(float dt)
{
	//time += dt;
	//if (scene->timeLapseShow(dt) <= 0)
	//{
	//	//gameOverFlag = true; //��ʱ���þ�������Ϸ����
	//	log("gameOverFlag");
	//}
	refreshElement = scene->isRefreshElement();
}

bool LinkState::exitState()
{
 	scene->forbiddenLink(); //��������
 	return true;
}