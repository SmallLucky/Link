#pragma once 

#include "LayerEffect/LayerEffect.h"
#include "Global.h"

class TotalLayer : public EasePop
{
public:
	bool init();
	CREATE_FUNC(TotalLayer);

	void addUI();
	void GoOnCallBack();
	void backCallBack();
	void addReward();

	void addGift(Sprite* s);

private:
	Button* nextLevel;
};