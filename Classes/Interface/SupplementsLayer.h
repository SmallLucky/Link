#pragma once

#include "Global.h"
#include "LayerEffect/LayerEffect.h"

class SupplementsLayer :public DownTopLayer
{
public:
	SupplementsLayer();
	~SupplementsLayer();
	static SupplementsLayer* createInit(int type);
	virtual bool init(int type);

	void	AddUI(int type);
	void	supplementsCallBack();
	void	breakCallBack();
};