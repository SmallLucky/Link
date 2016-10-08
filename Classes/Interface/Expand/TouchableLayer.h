#pragma once

#include "Global.h"

//可触摸层继承接口
//继承后需实现onTouchBegan、onTouchMoved、onTouchEnded函数

class TouchableLayer : public Layer
{
protected:
	bool responseFlag;	//是否响应触控

public:
	void onEnter();		//接收触控信息，子类不要覆盖

	void allowResponse();	//允许触控
	void forbiddenResponse();	//不允许触控，需在回调函数中用isResponse函数进行判断

	bool isResponse();	//是否允许触控

	virtual bool onTouchBegan(Touch *touch, Event *unused_event) = 0; //开始触控接口
	virtual void onTouchMoved(Touch *touch, Event *unused_event) = 0; //触点移动接口
	virtual void onTouchEnded(Touch *touch, Event *unused_event) = 0; //触控结束接口
	//virtual void onTouchCancelled(Touch *touch, Event *unused_event) = 0;
	
};

