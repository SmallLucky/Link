
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
void ElementUnit::createElement(int ele, Point pos)//22
{
	element = ele; //5
	//this->setPosition(pos);		//��������λ�ã����ӽڵ�λ������Ϊԭ�㼴�ɳ�������Ļ��Ӧλ����

	//elementSprite = Sprite::create(ELEMENT_NORMAL_IMG[ele]);	//����Ԫ������ѡ��
	elementSprite = Sprite::create(getImageString( ele));	//����Ԫ������ѡ��
	//log("createElement(int ele, Point pos) : %s", getImageString(ele));
	if (elementSprite)
	{
		//log("elementSprite::%d , point: %f -- %f",ele , pos.x,pos.y);
		this->setPosition(pos);		//��������λ�ã����ӽڵ�λ������Ϊԭ�㼴�ɳ�������Ļ��Ӧλ����
		elementSprite->setPosition(Point::ZERO);
		setSize(elementSprite, ELEMENT_NORMAL_SIZE);	//����ͼƬ�ĳߴ�
		addChild(elementSprite);
	}
	//log("ElementUnit::createElement(int ele, Point pos)");
}

string ElementUnit::getImageString(int ele)//22
{
	//log("ElementUnit::getImageString(int ele)%d", ele);
	string str = StringUtils::format("element/element_%i.png", ele);
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
	auto dale = DelayTime::create(0);
	Action* move = MoveTo::create(time,pos); // �����ƶ�
	auto call = CallFunc::create([this]{
		showQQEffect(this);
	});
	auto spew = Sequence::create(dale, move, call,nullptr);
	this->runAction(spew);
}

void ElementUnit::showQQEffect(Node* node)
{
	if (node)
	{
		Point p = node->getPosition();
		auto s1 = ScaleTo::create(0.05, 1.15, 0.8);
		auto s2 = ScaleTo::create(0.06, 1.1, 0.85);
		auto s3 = ScaleTo::create(0.065, 1.05, 0.95);
		auto s4 = ScaleTo::create(0.07, 0.95, 1.05);
		auto s5 = ScaleTo::create(0.075, 1, 1);

		auto m1 = MoveTo::create(0.1, Vec2(p.x, p.y + 2));
		auto m2 = MoveTo::create(0.09, Vec2(p.x, p.y));

		auto s6 = ScaleTo::create(0.055, 1.1, 0.89);
		auto s7 = ScaleTo::create(0.06, 1.07, 0.92);
		auto s8 = ScaleTo::create(0.065, 1.05, 0.98);
		auto s9 = ScaleTo::create(0.07, 1.01, 0.99);
		auto s10 = ScaleTo::create(0.07, 1, 1);

		auto m3 = MoveTo::create(0.09, Vec2(p.x, p.y + 1));
		auto m4 = MoveTo::create(0.085, p);

		auto s11 = ScaleTo::create(0.06, 1.05, 0.97);
		auto s12 = ScaleTo::create(0.065, 1.03, 0.98);
		auto s13 = ScaleTo::create(0.07, 1.01, 1.0);
		//auto s14 = ScaleTo::create(0.075, 0.99, 1.0);
		auto s15 = ScaleTo::create(0.08, 1, 1);

		auto m5 = MoveTo::create(0.08, Vec2(p.x, p.y + 2));
		auto m6 = MoveTo::create(0.08, p);

		auto seq = Sequence::create(s1, s2, s3, s4, s5, m1, m2, nullptr);// s6, s7, s8, s9, s10,s14m5,m6, s11, s12, s13, s15, m3, m4, s6, s7, s8, s9, s10,

		node->runAction(Repeat::create(seq, 1));
	}
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
	CallFunc * fun = CallFunc::create([this]{
		if (getParent())
		{
			this->removeFromParent();
		}
	});
	Action* scale = Sequence::create(ScaleTo::create(time, 0), fun, nullptr);// RemoveSelf::create()
	this->runAction(scale);
}
 
//����Ԫ������
int ElementUnit::getElement()
{
	return element;
}

void ElementUnit::setElementType(ElementType eType)
{
	_eType = eType;
}