#pragma once

#include "Global.h"
#include "LevelLayer.h"

class ObjectiveLayer : public Layer
{
public:
	bool init();
	CREATE_FUNC(ObjectiveLayer);

	void addUI();

	void startGame();
	void backButton();
private:
	LevelLayer* levelLayer;
	Button* starGame;
	
};