#pragma once

#include "Global.h"
#include "CommonFunction.h"
//#include "MatrixTLayer.h"

#include "Expand/TouchableLayer.h"
#include "Unit/ElementUnit.h"
#include "Unit/MyEffect.h"

class MapLayer : public TouchableLayer
{
public:
	MapLayer();
	~MapLayer();
	bool init();
	CREATE_FUNC(MapLayer);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event); //��ʼ����
	virtual void onTouchMoved(Touch *touch, Event *unused_event); //�������ƶ�
	virtual void onTouchEnded(Touch *touch, Event *unused_event); //��������

	Vec2  positionForTileCoord(const Vec2& tileCoord);
	Vec2  MypositionForTileCoord(const Vec2& tileCoord);

	Point getMCenterByCoord(int row, int line); //ͨ�������ø�������
	Point getMCenterByCoord(Coord c);//ͨ�������ø�������

	bool checkIsNull(int _row,int _line);

	int getElementType();
	void setElementType(int ele);
	int elementType;

	Coord lastPoint;
	bool isLine;
	bool getIsLine(){ return isLine; };

	//vector<int> special_Line;
	//vector<int> special_row;
	//vector<Coord> special_round;

	void removeBeEliminate(int r, int l);//��Ⲣ�������Ա�������Ԫ�أ��ı�typeֵ

	bool checkIsNeedRefresh();  //����Ƿ���Ҫ����ˢ��λ��
	void refreshAllElement();	//ˢ�����л���Ԫ�ص�λ��

	bool checkIsDrop(int r,int l);
	int green;
	int blue;
	int red;
	int yellor;
	int purple;

	//�Ƿ���ը��
	bool isBoom;

	bool getIsBoom() { return isBoom; }
	void setIsBoom( bool b) { isBoom = b; }
	//ը��ը����Ԫ��
	void isBoomElement(int _row,int _line);
	bool isCount;
	int  getSpecialScore(){  int s = specialScore; specialScore = 0;  return s; };
private:
	int  specialSttlement( int ele); // 
	int  specialScore;
	void  changeTiledType(int r,int l); // �޸�ָ��λ������Ƭ������ֵ

	Vector<ParticleSystem*> m_effects;
	void showEffect(Point p);
	bool removeEffect();
	void showLineEffect(Point p);
	void showRowEffect(Point p);
	void showBoomEffect(Point p);
private:

	TMXTiledMap* _tileMap;
	//MatrixTLayer* matrixTL;
	TMXLayer* mapLayer;
	TMXLayer* typeLayer;
	Value getType;

	//MyEffect* myeffect;

	Point mapPoint;

	int line; //��
	int row; //��
	
	
	vector<Coord> spriteNull;
	
private:
	ElementUnit* **elements; //����Ԫ��
	Point **blocksCenter;	//��������

	bool **signFlag;		//�Ƿ񱻱��
	bool **signFlagSpecial;		//�Ƿ�����Ԫ�ر�ǵ�

	float containsDis;		//��������ڸ����ڵ�������

	int *elementCount;		//�����и�Ԫ�ص�����
	int *removeCount;		//������Ԫ�ص�����

	vector<Coord> linkIndex; //һ�������е�����˳��
	vector<DrawNode*> linkBrush;	//��������Ԫ�ص���

	int removeAllCount; //һ��������������

	bool touchedFlag;	//�Ƿ��Ѿ���Ч�Ĵ���
	bool linkFinishFlag;	//�Ƿ�����Ч�����ӽ���
public:
	void removeLink();	//�������������������ӵ�Ԫ�أ���������ʧ��

	
	void specialSignElement(int ele ,Coord c); //������Ԫ����Ҫ������Ԫ�ؼ��ϱ��

	int getRemoveCount();	//��ȡ�ϴ�����������������������Ϊ�´�׼��

	bool elementsFall();	//Ԫ�����䣬���������Ԫ�ء������Ƿ���˿�λ��ÿ�������һ����λ
	bool isLinkFinish();	//�Ƿ���Ч�����ӽ���
	void prepareLink();		//׼����һ������

public:
	int m_count = 0;
	int removeMyCount();

public:
	//void drawGuideLine(Point leftBottom, Point rightTop);		//���ƾ��θ�����
	void drawLine(Coord from, Coord to);			//������������Ԫ�ص���
	bool removeLatestLine();					//ɾ�����һ�������ߣ������Ƿ�ɾ���ɹ�
	void removeAllLine();						//ɾ��������

	bool checkLink(Coord from, Coord to);		//����Ƿ�����������Ԫ�أ���ͨ���޸ĸú�������ʵ�ֶ���������

	void linkStart(int row, int line);	//�������ߵ����
	void linkElement(Coord from, Coord to);		//��������Ԫ��
	void undoLink(); //�������һ������

	void initBlocks();	//��ʼ�����ӣ�ȷ���������򣬳�ʼ״̬����Ϊ��
	void clearElement();	//��վ���Ԫ��

public:
	int createElement(int element, int row, int line);	//��ָ��λ�ô����������͵�Ԫ��
private:

	int removeElement(int row, int line);	//����ָ��λ�õ�Ԫ�أ�����Ԫ������
	int removeSignedElement();	//��������ǵ�Ԫ�أ�����������
	int getElement(int line, int row);		//��ȡָ��λ�õ�Ԫ������
	int getElement(Coord c);	//��ȡָ��λ��Ԫ������

	void rowFall(int _row, int bottom);		//ָ���е�ȫ����λ�Ϸ�Ԫ�����䣬���˳�����Ԫ��

	/**/
	void rowMyFall(int _row, int bottom);

	void appear(int row);		//ָ���еĶ��˳�����Ԫ��

public:
	int randElement();		//ͨ������Ԫ�����ͷֲ���������һ��Ԫ������
private:
	void initSign(); //��ʼ�����
	int signElement(int row, int line);	//���Ԫ��
	int signElement(Coord c);	//���Ԫ��
	int unsignElement(int row,int line); // ȡ�����Ԫ��
	int unsignElement(Coord c); //ȡ�����Ԫ��
	void signClear();	//������б��

	int signSpecialElement(Coord c); //�������Ԫ���漰��Ԫ��
	int unsignSpecialElement(Coord c); // ȡ������Ԫ���漰Ԫ�صı��
	void removeSignSpecial(); //ȡ����������������Ԫ���漰�����б�ʶ
	void removeCountCleaar();  //�����������
	void signOnlyBlock(int row, int line);		//ֻ���һ��Ԫ�أ�������Ǳ�ȡ����

};