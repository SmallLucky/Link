#include "LevelScene.h" //¹Ø¿¨²ã
#include "Interface/LevelLayer.h"
//test
#include "Interface/MapLayer.h"
#include "Interface/Interface.h"
#include "StartScene.h"


bool LevelScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	addLevelLayer();

	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(LevelScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
	return true;

	return true;
}

//Ìí¼Ólevel²ã
void  LevelScene::addLevelLayer()
{
	LevelLayer* levelLayer = LevelLayer::create();
	if (levelLayer)
	{
		addChild(levelLayer);
	}
	//MapLayer* mapLayer = MapLayer::create();
	//addChild(mapLayer);
}

void LevelScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //·µ»Ø
			{
				auto scene = StartScene::create();
				if (scene)
				{
					Director::getInstance()->replaceScene(scene);
				}
			}
		}
	}
}