#pragma once 

#include "Global.h"


class StopLayer :public Layer
{
public:
	CREATE_FUNC(StopLayer);
	bool init();
	void	newGameCallBack();
private:
	void addUI();
};