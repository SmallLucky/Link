#pragma once

#include "Global.h"

//������
class BackgroundLayer : public Layer
{
protected:
	Sprite* backgroun;

public:
	CREATE_FUNC(BackgroundLayer);
	bool init();
};