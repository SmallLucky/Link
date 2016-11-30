#pragma once

#include "Global.h"
#include "LevelLayer.h"
#include "LayerEffect/LayerEffect.h"

class ObjectiveLayer : public EasePop
{
public:
	bool init();
	CREATE_FUNC(ObjectiveLayer);

	void addUI();

	void startGame();
	void backButton();
	long millisecondNow();

	int  getTargetElementCount(int level);
	void addTargetElement(Node* node);
private:
	LevelLayer* levelLayer;
	Button* starGame;

	bool isFinish;
	
};