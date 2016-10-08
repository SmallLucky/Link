#pragma once 

#include "Global.h"

class TotalLayer : public Layer
{
public:
	bool init();
	CREATE_FUNC(TotalLayer);

	void addUI();
	void GoOnCallBack();
	void backCallBack();
};