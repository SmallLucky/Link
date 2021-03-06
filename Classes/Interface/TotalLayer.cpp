
#include "TotalLayer.h"
#include "TouchSwallowLayer.h"
#include "CommonFunction.h"
#include "Data/GameData.h"
#include "Scene/LevelScene.h"
#include "Scene/StartScene.h"
#include "Data/Data.h"
#include "Scene/GameScene.h"
#include "Interface/ObjectiveLayer.h"
#include "PowerLayer.h"

bool TotalLayer:: init()
{
	if (!EasePop::init())
	{
		return false;
	}

	bool flag = UserDefault::getInstance()->getBoolForKey("IS_MUSIC", true);
	if (flag)
	{
		AudioData::getInstance()->addBgMusic(4);
	}
	else
	{
		AudioData::getInstance()->stopBgMusic();
	}

	GAMEDATA->praseJsonData();
	//auto touchSwall = TouchSwallowLayer::create();
	//addChild(touchSwall);
	isButFinish = true;
	addUI();

	return true;
}

void TotalLayer::addUI()
{
	auto BG_kuang = Sprite::create("popbox/dikuang.png");
	BG_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,this, Vec2(0, 0)));
	m_popNode->addChild(BG_kuang);

	auto pet = Sprite::create("popbox/pet_3.png");
	pet->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, BG_kuang,Vec2(0, 100)));
	BG_kuang->addChild(pet);

	auto hong = Sprite::create("popbox/sucess_hong.png");
	hong->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, BG_kuang, Vec2(0, -40)));
	BG_kuang->addChild(hong);

	auto levelnumbg = Sprite::create("popbox/success_levenumbg.png");
	levelnumbg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, hong, Vec2(0, 0)));
	hong->addChild(levelnumbg);

	auto level = LabelAtlas::create(Value(GAMEDATA->getCurLevel()+1).asString(), "fonts/success_levelnum.png", 19, 24, '0');
	level->setAnchorPoint(Vec2(0.5, 0.5));
	level->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, levelnumbg, Vec2(0, 0)));
	levelnumbg->addChild(level);

	nextLevel = Button::create("button/next_level0.png", "button/next_level1.png");
	nextLevel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,BG_kuang,Vec2(0,80)));
	BG_kuang->addChild(nextLevel);
	nextLevel->addClickEventListener(CC_CALLBACK_0(TotalLayer::GoOnCallBack, this));

	//log("*********:%d", REWARDDATA->getIsFristPlay(GAMEDATA->getCurLevel()));
	if (REWARDDATA->getIsFristPlay(GAMEDATA->getCurLevel()) == 1)
	{
		auto comonon = Sprite::create("popbox/gift.png");
		comonon->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(0, 100)));
		BG_kuang->addChild(comonon);

		auto line = Sprite::create("popbox/line.png");
		line->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(0, 150)));
		BG_kuang->addChild(line);

		addGift(line);
	}
	else
	{
		auto comonon = Sprite::create("popbox/petSay.png");
		comonon->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(0, 100)));
		BG_kuang->addChild(comonon);
		auto petbg = Sprite::create("popbox/petSay_kuang.png");
		petbg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,comonon,Vec2(0,-110)));
		comonon->addChild(petbg);

		auto label = Sprite::create("popbox/petSayLabel.png");
		label->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,petbg,Vec2(0,0)));
		petbg->addChild(label);

		//奖励金币 , 爱心
		GAMEDATA->setPowerNum(min(GAMEDATA->getPowerNum() + 1, MAXPOWER)); // 过关奖励
	}
	auto backButton = Button::create("popbox/cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, BG_kuang, Vec2(-20, -20)));
	BG_kuang->addChild(backButton);
	backButton->addClickEventListener(CC_CALLBACK_0(TotalLayer::backCallBack, this));

}

void TotalLayer::addGift(Sprite* s)
{
	bool props = true;
	srand(time(nullptr));
	int ele = abs(rand() % 2);
	int num = (GAMEDATA->getCurLevel() + 1) % 10;
	if (num == 0)
	{
		//log("////%d",num);
		for (int i = 0; i < 4; i++)
		{
			auto giftBg = Sprite::create("popbox/success_propsbg.png");
			giftBg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, s, Vec2(170 - (i * 115), 90)));
			s->addChild(giftBg);

			if (i == 0)
			{
				auto power = Sprite::create("popbox/power.png");
				power->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				power->setScale(1.5);
				giftBg->addChild(power);
				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(-20, 20)));
				giftBg->addChild(x);

				auto num = LabelAtlas::create(Value(1).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
				num->setAnchorPoint(Vec2(0, 0.5));
				x->addChild(num);
			}
			if (i == 1)
			{
				auto love = Sprite::create("popbox/big_hart.png");
				love->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				love->setScale(1.5);
				giftBg->addChild(love);
				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(-20, 20)));
				giftBg->addChild(x);
				auto num = LabelAtlas::create(Value(1).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
				num->setAnchorPoint(Vec2(0, 0.5));
				x->addChild(num);
			}
			if (i == 2)
			{
				auto coins = Sprite::create("popbox/big_gold.png");
				coins->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				coins->setScale(1.5);
				giftBg->addChild(coins);

				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(-20, 20)));
				giftBg->addChild(x);
				auto num = LabelAtlas::create(Value(10).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
				num->setAnchorPoint(Vec2(0, 0.5));
				x->addChild(num);
			}			
			if (i == 3 && props)
			{
				props = false;
				log("////**%d",ele);
				string str = StringUtils::format("infor/props_%d.png", ele);
				auto props = Sprite::create(str);
				props->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				giftBg->addChild(props);

				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(-20, 20)));
				giftBg->addChild(x);

				if (ele == 0 )
				{
					auto num = LabelAtlas::create(Value(1).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
					num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
					num->setAnchorPoint(Vec2(0, 0.5));
					x->addChild(num);
					//
					GAMEDATA->setRefresh(GAMEDATA->getRefresh() + 1);

				}
				if (ele == 1)
				{
					auto num = LabelAtlas::create(Value(1).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
					num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
					num->setAnchorPoint(Vec2(0, 0.5));
					x->addChild(num);
					//
					GAMEDATA->setBoomb(GAMEDATA->getBoomb() + 1);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			auto giftBg = Sprite::create("popbox/success_propsbg.png");
			giftBg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, s, Vec2(100 - (i * 120), 90)));
			s->addChild(giftBg);

			if (i == 0)
			{
				auto power = Sprite::create("popbox/power.png");
				power->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				power->setScale(1.5);
				giftBg->addChild(power);
				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(-20, 20)));
				giftBg->addChild(x);

				auto num = LabelAtlas::create(Value(1).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
				num->setAnchorPoint(Vec2(0, 0.5));
				x->addChild(num);
			}
			if (i == 1)
			{
				auto love = Sprite::create("popbox/big_hart.png");
				love->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				love->setScale(1.5);
				giftBg->addChild(love);
				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(-20, 20)));
				giftBg->addChild(x);
				auto num = LabelAtlas::create(Value(1).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
				num->setAnchorPoint(Vec2(0, 0.5));
				x->addChild(num);
			}
			if (i == 2)
			{
				auto coins = Sprite::create("popbox/big_gold.png");
				coins->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				coins->setScale(1.5);
				giftBg->addChild(coins);

				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(-20, 20)));
				giftBg->addChild(x);
				auto num = LabelAtlas::create(Value(10).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
				num->setAnchorPoint(Vec2(0, 0.5));
				x->addChild(num);
			}
		}
	}
	addReward();
}

void TotalLayer::GoOnCallBack()
{
	if (isButFinish)
	{
		isButFinish = false;
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		{
			AudioData::getInstance()->addButtonEffect(1);
		}
		REWARDDATA->setIsFristPlay(GAMEDATA->getCurLevel()); //修改是否是第一次玩这个关卡
		int x = GAMEDATA->getCurLevel() + 1;
		if (x < GAMEDATA->m_lock.size())
		{
			GAMEDATA->setCurLevel(x);
			GAMEDATA->setLock(GAMEDATA->getCurLevel());
			if (GAMEDATA->getPowerNum() >= 2)
			{
				GAMEDATA->setPowerNum(GAMEDATA->getPowerNum() - 2);  //扣两点体力开始新游戏
				Point powerPoint = CommonFunction::getVisibleAchor(Anchor::Center, nextLevel, Vec2(-45, 10));
				auto power = Sprite::create("popbox/power.png");
				power->setPosition(powerPoint);//CommonFunction::getVisibleAchor(Anchor::MidTop, starGame, Vec2(-200, 600))
				power->setScale(4.0);
				power->setOpacity(0);
				nextLevel->addChild(power);

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
					GameScene* gameScene = GameScene::create();
					Director::getInstance()->replaceScene(TransitionFade::create(1, gameScene)); //跳转TransitionSlideInT::create(2, scene)
				}), nullptr);

				power->runAction(action);
			}
			else
			{
				cout << "体力不够了！" << endl;
				auto layer = PowerLayer::create();
				layer->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, this, Vec2(0, 0)));
				addChild(layer, 10);
				//LevelScene* levelScene = LevelScene::create();
				//Director::getInstance()->replaceScene(TransitionCrossFade::create(1, levelScene));
			}


		}
		else{
			log("no level");
			StartScene* scene = StartScene::create();
			Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(0.8, scene));
		}
	}
}

void TotalLayer::backCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
	{
		AudioData::getInstance()->addButtonEffect(1);
	}
	REWARDDATA->setIsFristPlay(GAMEDATA->getCurLevel()); //修改是否是第一次玩这个关卡
	int x = GAMEDATA->getCurLevel() + 1;
	if (x < GAMEDATA->m_lock.size())
	{
		GAMEDATA->setCurLevel(x);
		GAMEDATA->setLock(GAMEDATA->getCurLevel());
	}
	LevelScene* levelScene = LevelScene::create();
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1,levelScene));
}

void	TotalLayer:: addReward()
{
	//奖励金币 , 爱心
	GAMEDATA->setPowerNum(min(GAMEDATA->getPowerNum() + 1, MAXPOWER)); // 过关奖励
	GAMEDATA->setMoneyNum(GAMEDATA->getMoneyNum() + 10);
	GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() +1);

}