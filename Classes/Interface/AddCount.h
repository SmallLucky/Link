#pragma once

#include "Global.h"

class AddCount : public Layer
{
public:
	CREATE_FUNC(AddCount);
	bool init();
	void addUI();
	void addCountButton();

private:

};