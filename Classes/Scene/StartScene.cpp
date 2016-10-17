
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


bool StartScene::init()
{
		if (!Scene::init())
		{
			return false;
		}
		visibleSize = Director::getInstance()->getVisibleSize();
		string str = "bg/stargame_bg.png";
		createBackGround(str); //创建背景
		createMenu();				//创建菜单


		auto listenerkeyPad = EventListenerKeyboard::create();
		listenerkeyPad->onKeyReleased = CC_CALLBACK_2(StartScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
		return true;
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
}

//创建菜单
void StartScene::createMenu()
{
	auto startGame = Button::create("button/gamez_button.png");
	if (startGame)
	{
		startGame->addClickEventListener(CC_CALLBACK_0(StartScene::StartGame, this));
		startGame->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Vec2(0, 300)));
		addChild(startGame);
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
	logTime();
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