#pragma once

#include "Global.h"
#include "Data/Data.h"
#include "cocos2d.h"

//����Ԫ����Ϣ

class ElementUnit : public Node
{
public:
	CREATE_FUNC(ElementUnit);
	bool init();

	void createElement(int ele, Point pos);		//��ָ��λ�ô���ָ�����͵�Ԫ��
	void moveToPosition(Point pos);		//˲���ƶ���ָ��λ��
	void moveToPosition(Point pos, float time);	//�����ƶ���ָ��λ��
	void appear(float time);	//ͼ��˲����С�𽥱����ԭ��С
	void disappear(float time);		//ͼ���𽥱�С����ʧ��ɾ������

	int getElement();	//����Ԫ������

	void setElementHp(int h);
	int  getElementHp(){ return _hp; };

	string getImageString(int ele);

protected:
	Sprite* elementSprite; //Ԫ��ͼ��
	int element;	//Ԫ������

	int _hp;

};

