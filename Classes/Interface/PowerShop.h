#pragma once

#include "Global.h"

class PowerShop : public Layer
{
public:
	CREATE_FUNC(PowerShop);
	bool init();
	void addUI();
	void	refreshUI();

private:
	LabelAtlas* nowNum;
};