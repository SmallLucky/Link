#pragma once

#include "Global.h"
#include "LayerEffect/LayerEffect.h"

class LoserLayer : public EasePop
{
public:
	bool init();
	CREATE_FUNC(LoserLayer);
	void addUI();

	void quitCallBack();
	void againCallBack();
};