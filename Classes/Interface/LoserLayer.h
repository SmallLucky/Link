#pragma once

#include "Global.h"

class LoserLayer : public Layer
{
public:
	bool init();
	CREATE_FUNC(LoserLayer);
	void addUI();

	void quitCallBack();
	void againCallBack();
};