
#include "Global.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

class StartScene : public Scene
{
protected:

	void StartGame(Ref* ref);		
	void QuitGame(Ref* ref);		
	void SetButton();		

	static long millisecondNow();
	void logTime();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
public:

	CREATE_FUNC(StartScene);
	bool init();
	void PlayStopBGM();

private:
	void initUI();
	void rotateAction(Node* node);
	void fadeInAction(Node* node, float delay);
	void fadeInAction(Node* node,Vec2 from,Vec2 to,float delay);
	void moveAction(Node* node, Vec2 to, float delay,float d);
	void cloudMoveAction(Node* node, float delay, float d);
	void penguin_shakeAction(Node* node,float delay);
	void blinkAction(Node* node, float delay, int blink);
};