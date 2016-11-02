
#include "StartScene.h"
#include "Data/Data.h"
#include "GameScene.h"
#include "LevelScene.h"
#include "Interface/SetLayer.h"
#include "Interface/ShopLayer.h"
#include "Interface/QuitLayer.h"
#include "CommonFunction.h"
//test
#include "Interface/ObjectiveLayer.h"
#include "Interface/TotalLayer.h"
#include "Interface/LoserLayer.h"
#include "Interface/MapLayer.h"
#include "TTButton.h"
#include "DrawLight.h"


bool StartScene::init()
{
		if (!Scene::init())
		{
			return false;
		}
		string str = "bg/bg.png";
		createBackGround(str); //创建背景
		createMenu();				//创建菜单

		PlayStopBGM();
		auto listenerkeyPad = EventListenerKeyboard::create();
		listenerkeyPad->onKeyReleased = CC_CALLBACK_2(StartScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

		return true;
}

void	StartScene::PlayStopBGM()
{
	//UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true); // 
	bool flag = UserDefault::getInstance()->getBoolForKey("IS_MUSIC",true);
	if (flag)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM, true);
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
	}
	else
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	}
}

//创建背景
void StartScene:: createBackGround(string filename)
{
	BG = Sprite::create(filename);
	if (BG)
	{
		BG->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 0)));
		addChild(BG);
	}

	auto icon = Sprite::create("bg/starticon.png");
	if (icon)
	{
		icon->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,BG,Vec2(0,230)));
		BG->addChild(icon);
	}
}

//创建菜单
void StartScene::createMenu()
{
	auto startGame = Button::create("button/gamez_button.png");
	if (startGame)
	{
		startGame->addClickEventListener(CC_CALLBACK_0(StartScene::StartGame, this));
		startGame->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG, Vec2(0, -270)));
		BG->addChild(startGame);
	}


	//auto setButton = Button::create("button/set_button.png");
	//setButton->addClickEventListener(CC_CALLBACK_0(StartScene::SetButton, this));
	//setButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,Vec2(0,150)));
	//addChild(setButton);


}

//开始游戏 
void StartScene::StartGame()
{
	//log("test");
	//logTime();
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
	}
	LevelScene* levelScene = LevelScene::create();
	if (levelScene)
	{
		Director::getInstance()->replaceScene(levelScene);
	}
	//TotalLayer * total = TotalLayer::create();
	//addChild(total);
	//LoserLayer* lose = LoserLayer::create();
	//addChild(lose);
	//GameScene* scene = GameScene::create();
	//Director::getInstance()->replaceScene(scene);
	//MapLayer* layer = MapLayer::create();
	//addChild(layer);
	//TTButton* layer = TTButton::create();
	//addChild(layer);
}

long StartScene::millisecondNow()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}
void StartScene::logTime()
{
	log("Time:%ld", millisecondNow());
}

//设置
void StartScene::SetButton()
{
	SetLayer* setLayer = SetLayer::create();
	if (setLayer)
	{
		addChild(setLayer);
	}
}

//退出游戏
void StartScene::QuitGame()
{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
//	return;
//#endif
	auto quitLayer = QuitLayer::create();
	if (quitLayer)
	{
		addChild(quitLayer);
	}
	//Director::getInstance()->end();

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	exit(0);
//#endif
}

void StartScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
			{
				auto quitLayer = QuitLayer::create();
				if (quitLayer)
				{
					addChild(quitLayer);
				}
			}
		}
	}
}