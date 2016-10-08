#pragma once

#include "Global.h"
#include "Data/Data.h"
#include "cocos2d.h"

//单个元素信息

class ElementUnit : public Node
{
public:
	CREATE_FUNC(ElementUnit);
	bool init();

	void createElement(int ele, Point pos);		//在指定位置创建指定类型的元素
	void moveToPosition(Point pos);		//瞬间移动至指定位置
	void moveToPosition(Point pos, float time);	//匀速移动至指定位置
	void appear(float time);	//图案瞬间缩小逐渐变大至原大小
	void disappear(float time);		//图案逐渐变小，消失后删除自身

	int getElement();	//返回元素类型

	void setElementHp(int h);
	int  getElementHp(){ return _hp; };

	string getImageString(int ele);

protected:
	Sprite* elementSprite; //元素图案
	int element;	//元素类型

	int _hp;

};

