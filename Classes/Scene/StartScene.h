
#include "Global.h"

#include "cocos2d.h"

USING_NS_CC;

class StartScene : public Scene
{
protected:

	void createBackGround(string filename);		//��������
	void createMenu();		//�����˵�

	void StartGame();		//��ʼ��Ϸ
	void QuitGame();		//�˳���Ϸ
	void SetButton();		//����

	static long millisecondNow();
	void logTime();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
public:
	CREATE_FUNC(StartScene);
	bool init();

private:
	Size visibleSize;
	Sprite * BG;
};