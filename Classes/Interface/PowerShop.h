#pragma once

#include "Global.h"
#include "LayerEffect/LayerEffect.h"

class PowerShop : public TopDownLayer
{
public:
	CREATE_FUNC(PowerShop);
	bool init();
	void addUI();
	void	refreshUI();

private:
	LabelAtlas* nowNum;
};