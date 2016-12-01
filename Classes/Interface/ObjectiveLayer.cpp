
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
	BG_kuang->addChild(hong);;

	Node * node = Node::create();
	node->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,hong,Vec2(-25,15)));
	hong->addChild(node);

	auto level = Sprite::create("popbox/labelLevel.png");
	level->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, node, Vec2(0, 0)));
	node->addChild(level);

	auto num = LabelAtlas::create(Value(GAMEDATA->getCurLevel()+1).asString(),"fonts/levelNumber.png",32,50,'0');
	num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, level, Vec2(10, 0)));
	num->setAnchorPoint(Vec2(0,0.5));
	level->addChild(num);

	auto di_kuang = Sprite::create("popbox/object_neikuang.png");
	di_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,BG_kuang,Vec2(0,20)));
	BG_kuang->addChild(di_kuang);
	
	auto target_bg = Sprite::create("popbox/targetScore_bg.png");
	di_kuang->addChild(target_bg);
	if (getTargetElementCount(GAMEDATA->getCurLevel()) == 0)
	{
		target_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, di_kuang, Vec2(0, 0)));
	}
	else
	{
		target_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, di_kuang, Vec2(0, 40)));
		auto targetEle = Sprite::create("popbox/targetElement_bg.png");
		targetEle->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, di_kuang, Vec2(0, -70)));
		di_kuang->addChild(targetEle);
		addTargetElement(targetEle);
	}
	auto targetNum = LabelAtlas::create(Value(GAMEDATA->getTargetScore(GAMEDATA->getCurLevel())).asString(), "fonts/targetNumber.png", 30, 40, '0');
	targetNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,target_bg,Vec2(0,0)));
	targetNum->setAnchorPoint(Vec2(0.5,0.5));
	targetNum->setScale(1.2);
	target_bg->addChild(targetNum,1);


	starGame = Button::create("button/start_game0.png","button/start_game1.png");
	starGame->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(0, 80)));
	BG_kuang->addChild(starGame);
	starGame->addClickEventListener(CC_CALLBACK_0(ObjectiveLayer::startGame, this));

	auto backButton = Button::create("popbox/cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, BG_kuang, Vec2(-30, -30)));
	BG_kuang->addChild(backButton);
	backButton->addClickEventListener(CC_CALLBACK_0(ObjectiveLayer::backButton, this));


}

int  ObjectiveLayer::getTargetElementCount(int level)
{
	int count = 0;
	if (REWARDDATA->getBlue(level) != 0)
	{
		count++;
	}
	if (REWARDDATA->getPurple(level) != 0)
	{
		count++;
	}
	if (REWARDDATA->getGreen(level) != 0)
	{
		count++;
	}
	if (REWARDDATA->getRad(level) != 0)
	{
		count++;
	}
	if (REWARDDATA->getYellor(level) != 0)
	{
		count++;
	}
	return count;
}

void ObjectiveLayer::addTargetElement(Node* node)
{
	int level = GAMEDATA->getCurLevel();
	int offY = 0;
	int num = getTargetElementCount(GAMEDATA->getCurLevel());
	Node * nbg = Node::create();
	nbg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, node, Vec2(-(num/2)*90, 0)));
	nbg->setAnchorPoint(Vec2(0.5, 0.5));
	node->addChild(nbg);
		if (REWARDDATA->getBlue(level) != 0)
		{
			auto ele = Sprite::create("infor/task_ele_0.png");
			ele->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, nbg, Vec2(offY * 70 + 70, 0)));
			nbg->addChild(ele, 1);
			offY++;
			auto targetNum = LabelAtlas::create(Value(REWARDDATA->getBlue(level)).asString(), "fonts/targetNumber.png", 30, 40, '0');
			targetNum->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, ele, Vec2(-20, 20)));
			targetNum->setAnchorPoint(Vec2(0, 0.5));
			ele->addChild(targetNum, 1);
			targetNum->setScale(0.5);
		}
		if (REWARDDATA->getPurple(level) != 0)
		{
			auto ele = Sprite::create("infor/task_ele_1.png");
			ele->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, nbg, Vec2(offY * 70 + 70, 0)));
			nbg->addChild(ele, 1);
			offY++;
			auto targetNum = LabelAtlas::create(Value(REWARDDATA->getPurple(level)).asString(), "fonts/targetNumber.png", 30, 40, '0');
			targetNum->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, ele, Vec2(-20, 20)));
			targetNum->setAnchorPoint(Vec2(0, 0.5));
			ele->addChild(targetNum, 1);
			targetNum->setScale(0.5);
		}
		if (REWARDDATA->getGreen(level) != 0)
		{
			auto ele = Sprite::create("infor/task_ele_2.png");
			ele->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, nbg, Vec2(offY * 70 + 70, 0)));
			nbg->addChild(ele, 1);
			offY++;
			auto targetNum = LabelAtlas::create(Value(REWARDDATA->getGreen(level)).asString(), "fonts/targetNumber.png", 30, 40, '0');
			targetNum->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, ele, Vec2(-20, 20)));
			targetNum->setAnchorPoint(Vec2(0, 0.5));
			targetNum->setScale(0.5);
			ele->addChild(targetNum, 1);
		}
		if (REWARDDATA->getRad(level) != 0)
		{
			auto ele = Sprite::create("infor/task_ele_3.png");
			ele->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, nbg, Vec2(offY * 70 + 70, 0)));
			nbg->addChild(ele, 1);
			offY++;
			auto targetNum = LabelAtlas::create(Value(REWARDDATA->getRad(level)).asString(), "fonts/targetNumber.png", 30, 40, '0');
			targetNum->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, ele, Vec2(-20, 20)));
			targetNum->setAnchorPoint(Vec2(0, 0.5));
			ele->addChild(targetNum, 1);
			targetNum->setScale(0.5);
		}
		if (REWARDDATA->getYellor(level) != 0)
		{
			auto ele = Sprite::create("infor/task_ele_4.png");
			ele->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, nbg, Vec2(offY * 70 + 70, 0)));
			nbg->addChild(ele, 1);
			offY++;
			auto targetNum = LabelAtlas::create(Value(REWARDDATA->getYellor(level)).asString(), "fonts/targetNumber.png", 30, 40, '0');
			targetNum->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, ele, Vec2(-20, 20)));
			targetNum->setAnchorPoint(Vec2(0, 0.5));
			ele->addChild(targetNum, 1);
			targetNum->setScale(0.5);
		}
}

void ObjectiveLayer::startGame()
{
	if (isFinish)
	{
		isFinish = false;
		if (GAMEDATA->getPowerNum() >= 2)
		{
			if (GAMEDATA->getPowerNum() == MAXPOWER)
			{
				UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERTIME", millisecondNow()); //满点体力下消耗体力时的毫秒时间
				UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERDAY", CommonFunction::getCurDay()); //满点体力下消耗体力的第几天
			}

			GAMEDATA->setPowerNum(GAMEDATA->getPowerNum() - 2); //扣两点体力
			//发送消息到UI层
			EventCustom _event(REFRESHUI);
			_eventDispatcher->dispatchEvent(&_event);

			Point powerPoint = CommonFunction::getVisibleAchor(Anchor::Center, starGame, Vec2(-50, 10));
			auto power = Sprite::create("popbox/power.png");
			power->setPosition(powerPoint);//CommonFunction::getVisibleAchor(Anchor::MidTop, starGame, Vec2(-200, 600))
			power->setScale(4.0);
			power->setOpacity(0);
			starGame->addChild(power);

			auto x2 = Sprite::create("popbox/X2.png");
			x2->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, power, Vec2(0, 0)));
			power->addChild(x2);

			ScaleTo* scaleTo = ScaleTo::create(0.5, 1);
			auto fin = FadeTo::create(0.5, 250);
			//FadeTo
			auto time = DelayTime::create(0.5f);
			Spawn* spawn = Spawn::create(fin, scaleTo, nullptr);
			auto call = CallFunc::create([=]{

				auto light1 = Sprite::create("infor/guang.png");
				light1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, power, Vec2(-10,-5)));
				light1->setScale(1.5);
				power->addChild(light1, -1);
				auto rotate_ac = RotateBy::create(20.0f, 360.0);
				if (light1)
					light1->runAction(RepeatForever::create(rotate_ac));
			});
			auto action = Sequence::create(spawn, call, time, CallFunc::create([&]{
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
			close();
			//没有体力
			EventCustom _event(ADDPOWER);
			_eventDispatcher->dispatchEvent(&_event);
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