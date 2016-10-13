#include "StateList.h"

//��ʼ״̬

bool StartState::checkStateChange() //״̬�ı䷵��true��״̬δ�ı䷵��false��
{
	if (time > 1)
		CHANGE_TO_STATE(InitState); //�����ʼ��״̬
	return false;
}

bool StartState::entryState()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("StartState::entryState()");
#endif
	scene->initBackground(); //��ʼ������
	scene->initMatrix(); //��ʼ������
	scene->initInformation(); //��ʼ����Ϣ

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