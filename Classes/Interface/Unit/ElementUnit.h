#pragma once

#include "Global.h"
#include "Data/Data.h"
#include "cocos2d.h"

//����Ԫ����Ϣ
enum ElementType
{
	basisEliminate,
	canEliminate,
	beEliminate,
	cannotEliminate,
};


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

	void showQQEffect(Node* node);

	int getElement();	//����Ԫ������

	void setElementType(ElementType eType);
	ElementType  getElementType(){ return _eType; };

	string getImageString(int ele);

protected:
	Sprite* elementSprite; //Ԫ��ͼ��
	int element;	//Ԫ������

	ElementType _eType;

};

