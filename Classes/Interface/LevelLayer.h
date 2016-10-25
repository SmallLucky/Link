#pragma once 

#include "Global.h"

#include "cocos2d.h"

USING_NS_CC;

class LevelLayer : public Layer
{
public:
	LevelLayer();
	~LevelLayer();
	bool init();
	CREATE_FUNC(LevelLayer);;
	void	addScrollView();
	void	addUI();
	//添加关卡
	void addLevelButton();
	void back_buttonCallBack();

	void refreshUI();
	void addCallBack();
	void setCallBack();
	void timeUI(); //
	void setTimeUI(); //
	void update(float dt); // 
	long millisecondNow();
private:
	ScrollView* scrollview;
	Point oldPoint;
	int numBG; //背景张数
	Size visibleSize;

	LabelAtlas * powerNum;
	LabelAtlas * moneyNum;
	LabelAtlas * loveNum;

	LabelAtlas * minutes;
	LabelAtlas * seconds; 
	int minutsNum;
	int mt;
	int st;
};