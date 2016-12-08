
#include "LoserLayer.h"
#include "TouchSwallowLayer.h"
#include "Data/GameData.h"
#include "CommonFunction.h"
#include "Scene/GameScene.h"
#include "Scene/LevelScene.h"

bool LoserLayer:: init()
{
	if (!EasePop::init())
	{
		return false;
	}
	bool flag = UserDefault::getInstance()->getBoolForKey("IS_MUSIC", true);
	if (flag)
	{
		AudioData::getInstance()->addBgMusic(5);
	}
	else
	{
		AudioData::getInstance()->stopBgMusic();
	}
	//auto tuchSwall = TouchSwallowLayer::create();
	//addChild(tuchSwall);
	isButFinish = true;
	GAMEDATA->praseJsonData();

	addUI();
	return true;
}

void LoserLayer::addUI()
{

	auto BG_kuang = Sprite::create("popbox/dikuang.png");
	BG_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,this, Vec2(0, 0)));
	m_popNode->addChild(BG_kuang);

	auto hong = Sprite::create("popbox/loser_hong.png");
	hong->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,BG_kuang,Vec2(0,-40)));
	BG_kuang->addChild(hong);

	auto pet = Sprite::create("popbox/pet_2.png");
	pet->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,hong,Vec2(0,110)));
	hong->addChild(pet);

	auto label = Sprite::create("popbox/loser_label.png");
	label->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,BG_kuang,Vec2(0,-40)));
	BG_kuang->addChild(label);

	auto comonon = Sprite::create("popbox/commonCard.png");
	comonon->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,label,Vec2(0,50)));
	label->addChild(comonon);

	auto levelnumbg = Sprite::create("popbox/loserlevel_kuang.png");
	levelnumbg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,hong,Vec2(0,0)));
	hong->addChild(levelnumbg);

	auto level = LabelAtlas::create(Value(GAMEDATA->getCurLevel()+1).asString(),"fonts/loser_num.png",19,27,'0');
	level->setAnchorPoint(Vec2(0.5,0.5));
	level->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,levelnumbg,Vec2(0,0)));
	levelnumbg->addChild(level);

	againButton = Button::create("button/oneAgain0.png", "button/oneAgain1.png");
	againButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(0,80)));
	BG_kuang->addChild(againButton);
	againButton->addClickEventListener(CC_CALLBACK_0(LoserLayer::againCallBack, this));

	auto quitButton = Button::create("popbox/cancel.png");
	quitButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, BG_kuang, Vec2(-20, -20)));
	BG_kuang->addChild(quitButton);
	quitButton->addClickEventListener(CC_CALLBACK_0(LoserLayer::quitCallBack, this));
}

void LoserLayer::quitCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addButtonEffect(1);
	auto gameScene = LevelScene::create();
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7,gameScene));
}

void LoserLayer::againCallBack()
{
	if (isButFinish)
	{
		isButFinish = false;
		//再玩一次减少体力
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			AudioData::getInstance()->addButtonEffect(1);
		if (GAMEDATA->getPowerNum() >= 2)
		{
			GAMEDATA->setPowerNum(GAMEDATA->getPowerNum() - 2);
			Point powerPoint = CommonFunction::getVisibleAchor(Anchor::Center, againButton, Vec2(-60, 10));
			auto power = Sprite::create("popbox/power.png");
			power->setPosition(powerPoint);//CommonFunction::getVisibleAchor(Anchor::MidTop, starGame, Vec2(-200, 600))
			power->setScale(4.0);
			power->setOpacity(0);
			againButton->addChild(power);

			auto x2 = Sprite::create("popbox/X2.png");
			x2->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, power, Vec2(0, 0)));
			power->addChild(x2);

			ScaleTo* scaleTo = ScaleTo::create(0.5, 0.8);
			auto fin = FadeTo::create(0.5, 250);
			//FadeTo
			auto time = DelayTime::create(0.5f);
			Spawn* spawn = Spawn::create(fin, scaleTo, nullptr);
			auto call = CallFunc::create([=]{

				auto light1 = Sprite::create("infor/guang.png");
				light1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, power, Vec2(-10, -5)));
				power->addChild(light1, -1);
				auto rotate_ac = RotateBy::create(20.0f, 360.0);
				if (light1)
					light1->runAction(RepeatForever::create(rotate_ac));
			});
			auto action = Sequence::create(spawn, call, time, CallFunc::create([&]{
				if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
					AudioData::getInstance()->addOtherEffect(1);
				//this->removeFromParent();
				auto gameScene = GameScene::create();
				Director::getInstance()->replaceScene(TransitionFade::create(0.7, gameScene)); //跳转TransitionSlideInT::create(2, scene)
			}), nullptr);

			power->runAction(action);


		}
		else
		{
			//提示体力不足，去购买！~！！缺界面
			auto layer = PowerLayer::create();
			layer->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, this, Vec2(0, 0)));
			addChild(layer, 10);
		}
	}
}