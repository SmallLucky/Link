
#pragma once
#include "Global.h"

#include "TouchSwallowLayer.h"
#include "LayerEffect/LayerEffect.h"

class QuitLayer : public EasePop
{
public:
	bool init();
	CREATE_FUNC(QuitLayer);

	void addUI();

	void QuitGame();
	void backGame();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

};