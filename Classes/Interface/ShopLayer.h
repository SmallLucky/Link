#pragma once

#include "Global.h"

class ShopLayer : public Layer
{
public:
	bool init();
	CREATE_FUNC(ShopLayer);

	void addUI();

	void backButCallBack();

};
