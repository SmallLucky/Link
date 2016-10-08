#pragma once

#include "Global.h"

//±³¾°²ã
class BackgroundLayer : public Layer
{
protected:
	Sprite* backgroun;

public:
	CREATE_FUNC(BackgroundLayer);
	bool init();
};