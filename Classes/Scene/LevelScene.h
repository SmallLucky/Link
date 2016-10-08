#pragma once 

#include "Global.h"

#include "cocos2d.h"

class LevelScene : public Scene
{
public:

	CREATE_FUNC(LevelScene);
	bool init();

	void addLevelLayer();//Ìí¼Ólevel²ã

private:
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
};