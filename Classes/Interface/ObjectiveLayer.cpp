
#include "ObjectiveLayer.h"
#include "CommonFunction.h"
#include "Data/GameData.h"
#include "TouchSwallowLayer.h"
#include "Scene/GameScene.h"



#define GAMEDATA GameData::getInstance()
bool ObjectiveLayer::init()
{
	if (!EasePop::init())
	{
		return false;
	}
	GameData::getInstance()->praseJsonData();

	//TouchSwallowLayer* touchSwall = TouchSwallowLayer::create();
	//addChild(touchSwall);

	isFinish = true;

	addUI();

	return true;
}

void ObjectiveLayer::addUI()
{
	auto BG_kuang = Sprite::create("popbox/dikuang.png");
	BG_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,this,Vec2(0, 0)));
	m_popNode->addChild(BG_kuang);

	auto hong = Sprite::create("popbox/hong.png");
	hong->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,BG_kuang,Vec2(0,-30)));
	BG_kuang->addChild(hong);

	auto pet = Sprite::create("popbox/pet_1.png");
	pet->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,hong,Vec2(0,-20)));
	pet->setAnchorPoint(Vec2(0.5,0));
	hong->addChild(pet);

	auto level = Sprite::create("popbox/labelLevel.png");
	level->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,hong,Vec2(-40,15)));
	hong->addChild(level);

	auto num = LabelAtlas::create(Value(GAMEDATA->getCurLevel()+1).asString(),"fonts/levelNumber.png",32,50,'0');
	num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, level, Vec2(10, 0)));
	num->setAnchorPoint(Vec2(0,0.5));
	level->addChild(num);

	auto di_kuang = Sprite::create("popbox/neiKuang.png");
	di_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,BG_kuang,Vec2(0,20)));
	BG_kuang->addChild(di_kuang);
	
	auto target_bg = Sprite::create("popbox/target_bg.png");
	target_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, di_kuang, Vec2(-80, 50)));
	di_kuang->addChild(target_bg);

	auto targetNum = LabelAtlas::create(Value(GAMEDATA->getTargetScore(GAMEDATA->getCurLevel())).asString(),"fonts/targetNumber.png",36,50,'0');
	targetNum->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,target_bg,Vec2(0,-20)));
	targetNum->setAnchorPoint(Vec2(0.5,1));
	target_bg->addChild(targetNum);

	auto count_bg = Sprite::create("popbox/count_bg.png");
	count_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,di_kuang,Vec2(100,50)));
	di_kuang->addChild(count_bg);

	auto countNum = LabelAtlas::create(Value(GAMEDATA->getCount(GAMEDATA->getCurLevel())).asString(), "fonts/countNumber.png", 36, 50, '0');
	countNum->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,count_bg,Vec2(0,-20)));
	countNum->setAnchorPoint(Vec2(0.5, 1));
	count_bg->addChild(countNum);

	starGame = Button::create("button/start_game.png");
	starGame->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(0, 100)));
	BG_kuang->addChild(starGame);
	starGame->addClickEventListener(CC_CALLBACK_0(ObjectiveLayer::startGame, this));

	auto backButton = Button::create("popbox/cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, BG_kuang, Vec2(-30, -30)));
	BG_kuang->addChild(backButton);
	backButton->addClickEventListener(CC_CALLBACK_0(ObjectiveLayer::backButton, this));


}

void ObjectiveLayer::startGame()
{
	if (isFinish)
	{
		isFinish = false;
		if (GAMEDATA->getPowerNum() >= 2)
		{
			GAMEDATA->setPowerNum(GAMEDATA->getPowerNum() - 2); //扣两点体力
			//发送消息到UI层
			EventCustom _event(REFRESHUI);
			_eventDispatcher->dispatchEvent(&_event);

			if (UserDefault::getInstance()->getIntegerForKey("GAME_TIME") == 0)
			{
				UserDefault::getInstance()->setIntegerForKey("GAME_TIME", millisecondNow());
			}
			Point powerPoint = CommonFunction::getVisibleAchor(Anchor::Center, starGame, Vec2(37, 10));
			auto power = Sprite::create("popbox/power.png");
			power->setPosition(powerPoint);//CommonFunction::getVisibleAchor(Anchor::MidTop, starGame, Vec2(-200, 600))
			power->setScale(4.0);
			power->setOpacity(0);
			starGame->addChild(power);

			ScaleTo* scaleTo = ScaleTo::create(0.5, 1);
			auto fin = FadeTo::create(0.5, 250);
			//FadeTo
			auto time = DelayTime::create(0.2f);
			Spawn* spawn = Spawn::create(fin, scaleTo, nullptr);
			auto action = Sequence::create(spawn, time, CallFunc::create([&]{
				if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
					AudioData::getInstance()->addOtherEffect(1);
				//this->removeFromParent();
				GameScene* gameScene = GameScene::create();
				Director::getInstance()->replaceScene(TransitionFade::create(1, gameScene));  //跳转TransitionSlideInT::create(2, scene)
			}), nullptr);

			power->runAction(action);
		}
		else
		{
			//没有体力
		}
	}
}
void ObjectiveLayer::backButton()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addButtonEffect(1);
	close();
	//this->removeFromParent();
}

long ObjectiveLayer::millisecondNow()
{
	struct timeval now;
	gettimeofday(&now, nullptr);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}