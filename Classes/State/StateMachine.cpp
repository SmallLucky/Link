#include "StateMachine.h"
#include "State.h"
#include "Scene/GameScene.h"

//初始化状态机，创建时自动调用
bool StateMachine::initWithGameScene(GameScene* scene)
{
	this->state = NULL;
	this->scene = scene;
	CC_SAFE_RETAIN(scene);

	return true;
}

//调用该函数创建状态机
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
		pRet = NULL;
		return NULL;
	}
}

//返回当前状态
State* StateMachine::getState()
{
	return state;
}

//更改状态
void StateMachine::changeState(State* state)
{
	CC_SAFE_RELEASE(this->state);
	this->state = state;
}

//每帧自动调用状态相应方法
void StateMachine::update(float delta)
{
	if (state)
	{
		if (!state->checkStateChange())
			state->onUpdate(delta);
	}
}