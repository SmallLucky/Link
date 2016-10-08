
#include "ElementUnit.h"

bool ElementUnit:: init()
{
	if (!Node::init())
	{
		return false;
	}

	return true;
}

//��ָ��λ�ô���ָ�����͵�Ԫ��
void ElementUnit::createElement(int ele, Point pos)
{
	element = ele; //5
	_hp = 0;
	this->setPosition(pos);		//��������λ�ã����ӽڵ�λ������Ϊԭ�㼴�ɳ�������Ļ��Ӧλ����

	//elementSprite = Sprite::create(ELEMENT_NORMAL_IMG[ele]);	//����Ԫ������ѡ��
	elementSprite = Sprite::create(getImageString( ele));	//����Ԫ������ѡ��
	if (elementSprite)
	{
		log("elementSprite::%d , point: %f -- %f",ele , pos.x,pos.y);
		elementSprite->setPosition(Point::ZERO);
		setSize(elementSprite, ELEMENT_NORMAL_SIZE);	//����ͼƬ�ĳߴ�
		addChild(elementSprite);
	}
}

string ElementUnit::getImageString(int ele)
{
	string str = StringUtils::format("element/element_%d.png", ele);
	return str;
}

//˲���ƶ���ָ��λ��
void ElementUnit::moveToPosition(Point pos)
{
	this->setPosition(pos);
}

//�����ƶ���ָ��λ��
void ElementUnit::moveToPosition(Point pos, float time)
{
	this->setScale(1);
	this->stopAllActions();
	Action* move = MoveTo::create(time,pos); // �����ƶ�
	this->runAction(move);
}

//ͼ��˲����С�𽥱����ԭ��С
void ElementUnit::appear(float time)
{
	this->setScale(0);	//��С
	Action* scale = ScaleTo::create(time, 1);	//�𽥷Ŵ�
	this->runAction(scale);
}

//ͼ���𽥱�С����ʧ��ɾ������
void ElementUnit::disappear(float time)
{
	Action* scale = Sequence::create(ScaleTo::create(time, 0), RemoveSelf::create(), NULL);
	this->runAction(scale);
}
 
//����Ԫ������
int ElementUnit::getElement()
{
	return element;
}

void ElementUnit::setElementHp(int h)
{
	_hp = h;
}