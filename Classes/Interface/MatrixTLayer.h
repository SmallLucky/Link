#pragma once 

#include "Global.h"
#include "Expand/TouchableLayer.h"
#include "Unit/ElementUnit.h"


//�����
class MatrixTLayer : public TouchableLayer
{
public:
	MatrixTLayer();//���캯����Ϊ�������ռ�
	~MatrixTLayer(); //������������������ռ�
	CREATE_FUNC(MatrixTLayer);
	bool init();

	void removeLink();	//�������������������ӵ�Ԫ�أ���������ʧ��

	int getRemoveCount();	//��ȡ�ϴ�����������������������Ϊ�´�׼��

	bool elementsFall();	//Ԫ�����䣬���������Ԫ�ء������Ƿ���˿�λ��ÿ�������һ����λ
	bool isLinkFinish();	//�Ƿ���Ч�����ӽ���
	void prepareLink();		//׼����һ������

public:
	int m_count = 0;
	int removeMyCount();

private:
	ElementUnit* **elements; //����Ԫ��
	Point **blocksCenter;	//��������

	bool **signFlag;		//�Ƿ񱻱��

	float containsDis;		//��������ڸ����ڵ�������

	int *elementCount;		//�����и�Ԫ�ص�����
	int *removeCount;		//������Ԫ�ص�����

	vector<Coord> linkIndex; //һ�������е�����˳��
	vector<DrawNode*> linkBrush;	//��������Ԫ�ص���

	int removeAllCount; //һ��������������
	 
	bool touchedFlag;	//�Ƿ��Ѿ���Ч�Ĵ���
	bool linkFinishFlag;	//�Ƿ�����Ч�����ӽ���



private:
	virtual bool onTouchBegan(Touch *touch, Event *unused_event); //��ʼ����
	virtual void onTouchMoved(Touch *touch, Event *unused_event); //�������ƶ�
	virtual void onTouchEnded(Touch *touch, Event *unused_event); //��������
	

	void drawGuideLine(Point leftBottom,Point rightTop);		//���ƾ��θ�����
	void drawLine(Coord from,Coord to);			//������������Ԫ�ص���
	bool removeLatestLine();					//ɾ�����һ�������ߣ������Ƿ�ɾ���ɹ�
	void removeAllLine();						//ɾ��������

	bool checkLink(Coord from,Coord to);		//����Ƿ�����������Ԫ�أ���ͨ���޸ĸú�������ʵ�ֶ���������

	void linkStart(int line,int row);	//�������ߵ����
	void linkElement(Coord from, Coord to);		//��������Ԫ��
	void undoLink(); //�������һ������

	void initBlocks();	//��ʼ�����ӣ�ȷ���������򣬳�ʼ״̬����Ϊ��
	void clearElement();	//��վ���Ԫ��

public:
	int createElement(int element,int line,int row);	//��ָ��λ�ô����������͵�Ԫ��
private:

	int removeElement(int line,int row);	//����ָ��λ�õ�Ԫ�أ�����Ԫ������
	int removeSignedElement();	//��������ǵ�Ԫ�أ�����������
	int getElement(int line, int row);		//��ȡָ��λ�õ�Ԫ������
	int getElement(Coord c);	//��ȡָ��λ��Ԫ������

	void rowFall(int row, int bottom);		//ָ���е�ȫ����λ�Ϸ�Ԫ�����䣬���˳�����Ԫ��
	void appear(int row);		//ָ���еĶ��˳�����Ԫ��

public:
	int randElement();		//ͨ������Ԫ�����ͷֲ���������һ��Ԫ������
private:
	void initSign(); //��ʼ�����
	int signElement(int line, int row);	//���Ԫ��
	int signElement(Coord c);	//���Ԫ��
	int unsignElement(int line, int row); // ȡ�����Ԫ��
	int unsignElement(Coord c); //ȡ�����Ԫ��
	void signClear();	//������б��

	void removeCountCleaar();  //�����������
	void signOnlyBlock(int line,int row);		//ֻ���һ��Ԫ�أ�������Ǳ�ȡ����

};

