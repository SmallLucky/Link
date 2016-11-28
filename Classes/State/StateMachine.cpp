#include "StateMachine.h"
#include "State.h"
#include "Scene/GameScene.h"

//��ʼ��״̬��������ʱ�Զ�����
bool StateMachine::initWithGameScene(GameScene* scene)
{
	this->state = nullptr;
	this->scene = scene;
	CC_SAFE_RETAIN(scene);

	return true;
}

//���øú�������״̬��
StateMachine* StateMachine::createWithGameScene(GameScene* scene)
{
	StateMachine *pRet = new StateMachine();
	if (pRet && pRet->initWithGameScene(scene))
	{
		pRet->autorelease();
		CC_SAFE_RETAIN(pRet);
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

//���ص�ǰ״̬
State* StateMachine::getState()
{
	return state;
}

//����״̬
void StateMachine::changeState(State* state)
{
	CC_SAFE_RELEASE(this->state);
	this->state = state;
}

//ÿ֡�Զ�����״̬��Ӧ����
void StateMachine::update(float delta)
{
	//log("StateMachine::update(float delta)");
	if (state)
	{
		if (!state->checkStateChange())
			state->onUpdate(delta);
	}
}