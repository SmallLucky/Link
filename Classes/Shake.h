#pragma once

#include "cocos2d.h"  
USING_NS_CC;
/*
用法:

pSprite->runAction(CCShake::create(0.1f,10));
pSprite:想抖动的物体

第一个参数是:抖动的时间

第一个参数是:抖动的幅度

注意

一个CCNode同时执行多个CCShake动作,或者一个CCShake没有完又执行一个CCShake的话就会出现问题,会出现偏移的现象!

解决方案:

1).不要同时执行多个CCShake动作.

2.自己外部记录这个CCNode的位置,执行完成后手动setPosition();

*/

class Shake : public ActionInterval
{
public:
	Shake();

	// Create the action with a time and a strength (same in x and y)  
	// 产生震动效果的初始化函数参数,两个方向相同  
	// @param d 震动持续的时间  
	// @param strength 震动的幅度  
	static Shake* create(float d, float strength);
	// Create the action with a time and strengths (different in x and y)  
	// 产生震动效果的初始化函数,两个方向值不一样  
	static Shake* create(float d, float strength_x, float strength_y);
	bool initWithDuration(float d, float strength_x, float strength_y);

	//以下都是重写父类抽象类的函数(必须重写)  
	virtual Shake* clone() const override;
	virtual Shake* reverse(void) const override;
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;
	virtual void stop(void);

protected:
	// Initial position of the shaked node  
	// 精灵的位置  
	float _initial_x, _initial_y;
	// Strength of the action  
	// 抖动的幅度  
	float _strength_x, _strength_y;
};
