#pragma once

#include "Global.h"
#include "LayerEffect/LayerEffect.h"

class ShopLayer : public TopDownLayer
{
public:
	ShopLayer();
	~ShopLayer();
	bool init();
	CREATE_FUNC(ShopLayer);

	void addUI();

	void backButCallBack();

private:
	Sprite* props_bg;
	Sprite* money_bg;
	Sprite* love_bg;
	Button* proButton;
	Button* monButton;
	Button* gifButton;
};
