
#include "ElementUnit.h"

bool ElementUnit:: init()
{
	if (!Node::init())
	{
		return false;
	}

	return true;
}

//在指定位置创建指定类型的元素
void ElementUnit::createElement(int ele, Point pos)
{
	element = ele; //5
	_hp = 0;
	this->setPosition(pos);		//设置自身位置，将子节点位置设置为原点即可出现在屏幕对应位置上

	//elementSprite = Sprite::create(ELEMENT_NORMAL_IMG[ele]);	//根据元素类型选择
	elementSprite = Sprite::create(getImageString( ele));	//根据元素类型选择
	if (elementSprite)
	{
		log("elementSprite::%d , point: %f -- %f",ele , pos.x,pos.y);
		elementSprite->setPosition(Point::ZERO);
		setSize(elementSprite, ELEMENT_NORMAL_SIZE);	//设置图片的尺寸
		addChild(elementSprite);
	}
}

string ElementUnit::getImageString(int ele)
{
	string str = StringUtils::format("element/element_%d.png", ele);
	return str;
}

//瞬间移动至指定位置
void ElementUnit::moveToPosition(Point pos)
{
	this->setPosition(pos);
}

//匀速移动至指定位置
void ElementUnit::moveToPosition(Point pos, float time)
{
	this->setScale(1);
	this->stopAllActions();
	Action* move = MoveTo::create(time,pos); // 匀速移动
	this->runAction(move);
}

//图案瞬间缩小逐渐变大至原大小
void ElementUnit::appear(float time)
{
	this->setScale(0);	//缩小
	Action* scale = ScaleTo::create(time, 1);	//逐渐放大
	this->runAction(scale);
}

//图案逐渐变小，消失后删除自身
void ElementUnit::disappear(float time)
{
	Action* scale = Sequence::create(ScaleTo::create(time, 0), RemoveSelf::create(), NULL);
	this->runAction(scale);
}
 
//返回元素类型
int ElementUnit::getElement()
{
	return element;
}

void ElementUnit::setElementHp(int h)
{
	_hp = h;
}