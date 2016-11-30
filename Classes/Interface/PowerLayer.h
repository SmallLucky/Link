#pragma once

#include "Global.h"
#include "LayerEffect/LayerEffect.h"

class PowerLayer : public EasePop
{
public:
	bool init();
	CREATE_FUNC(PowerLayer);

	void addUI();
	void closeCbk(Ref*sender);
	void buyCbk(Ref*sender);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

};