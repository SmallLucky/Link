
#include "Global.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

class StartScene : public Scene
{
protected:

	void createBackGround(string filename);		//创建背景
	void createMenu();		//创建菜单

	void StartGame();		//开始游戏
	void QuitGame(Ref* ref);		//退出游戏
	void SetButton();		//设置

	static long millisecondNow();
	void logTime();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
public:
	CREATE_FUNC(StartScene);
	bool init();
	void PlayStopBGM();

private:
	Sprite * BG;

};