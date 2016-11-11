
#include "Global.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

class StartScene : public Scene
{
protected:

	void createBackGround(string filename);		//��������
	void createMenu();		//�����˵�

	void StartGame();		//��ʼ��Ϸ
	void QuitGame(Ref* ref);		//�˳���Ϸ
	void SetButton();		//����

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