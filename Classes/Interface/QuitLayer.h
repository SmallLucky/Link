
#pragma once
#include "Global.h"

#include "TouchSwallowLayer.h"

class QuitLayer : public Layer
{
public:
	bool init();
	CREATE_FUNC(QuitLayer);

	void addUI();

	void QuitGame();
	void backGame();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

};