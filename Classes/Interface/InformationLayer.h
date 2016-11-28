#pragma once 

#include "Global.h"


//��Ϣ��

class InformationLayer : public Layer
{
protected:
	Label* timeLabel; //ʱ���ǩ
	LabelAtlas* scoreLabel; //������ǩ
	LabelAtlas* countLabel; //������ǩ
	LabelAtlas* targatLabel; //������ǩ
	LabelAtlas* targetElementBNum; //Ŀ������ǩ
	LabelAtlas* targetElementPNum; //Ŀ������ǩ
	LabelAtlas* targetElementGNum; //Ŀ������ǩ
	LabelAtlas* targetElementRNum; //Ŀ������ǩ
	LabelAtlas* targetElementYNum; //Ŀ������ǩ

	Sprite* info_bg;
	Sprite* count_bg;
	Sprite* score_bg;
	Sprite* targetScore_bg;

public:
	InformationLayer();
	~InformationLayer();
	CREATE_FUNC(InformationLayer);
	bool init();

	void createLabel(Label* &label, char* text, Color4B color, Point pos, float size, Point anchor = Point(0.5, 0.5)); //���һ����ǩ

	//void initTimeLabel(); //��ʼ��ʱ���ǩ
	//void showTime(float t); //��ʾʱ��

	void initScoreLabel(); //��ʼ���÷ֱ�ǩ
	void showScore(int s); //��ʾ�÷�

	void initCountLabel(); //��ʼ��������ǩ
	void showCount(int c); //��ʾ����

	//void initPowerLabel();//��ʼ��������ǩ
	//void showPower(int p);//��ʾ����

	void scoreUp(int delta, int time); //��ʾ�÷�����

	void initTargetScore();
	void	addUI();
	void	addTargetElement();
	void	showTargetElementNum(int ele ,int n);
};