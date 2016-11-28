#pragma once 

#include "Global.h"


//信息层

class InformationLayer : public Layer
{
protected:
	Label* timeLabel; //时间标签
	LabelAtlas* scoreLabel; //分数标签
	LabelAtlas* countLabel; //步数标签
	LabelAtlas* targatLabel; //体力标签
	LabelAtlas* targetElementBNum; //目标数标签
	LabelAtlas* targetElementPNum; //目标数标签
	LabelAtlas* targetElementGNum; //目标数标签
	LabelAtlas* targetElementRNum; //目标数标签
	LabelAtlas* targetElementYNum; //目标数标签

	Sprite* info_bg;
	Sprite* count_bg;
	Sprite* score_bg;
	Sprite* targetScore_bg;

public:
	InformationLayer();
	~InformationLayer();
	CREATE_FUNC(InformationLayer);
	bool init();

	void createLabel(Label* &label, char* text, Color4B color, Point pos, float size, Point anchor = Point(0.5, 0.5)); //添加一个标签

	//void initTimeLabel(); //初始化时间标签
	//void showTime(float t); //显示时间

	void initScoreLabel(); //初始化得分标签
	void showScore(int s); //显示得分

	void initCountLabel(); //初始化步数标签
	void showCount(int c); //显示步数

	//void initPowerLabel();//初始化体力标签
	//void showPower(int p);//显示体力

	void scoreUp(int delta, int time); //显示得分增加

	void initTargetScore();
	void	addUI();
	void	addTargetElement();
	void	showTargetElementNum(int ele ,int n);
};