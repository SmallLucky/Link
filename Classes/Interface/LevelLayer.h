#pragma once 

#include "Global.h"

#include "cocos2d.h"

USING_NS_CC;

class LevelLayer : public Layer
{
public:

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
private:
	ScrollView* scrollview;
	Point oldPoint;
	int numBG; //背景张数
	Size visibleSize;
};