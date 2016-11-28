#pragma once

#include "Global.h"
#include "LayerEffect/LayerEffect.h"

class AddCount : public TopDownLayer
{
public:
	CREATE_FUNC(AddCount);
	bool init();
	void addUI();
	void addCountButton();

private:

};