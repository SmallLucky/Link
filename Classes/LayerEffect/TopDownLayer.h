#pragma once

#include "BaseLayer.h"
#include "cocos2d.h"

USING_NS_CC;
/*
从上往下
*/
class TopDownLayer : public BaseLayer
{
public:
	virtual bool init();
	virtual void onEnter();
	virtual void close();

protected:
	virtual void onEnterAnimComplete();
	virtual void onExitAnimComplete();

};