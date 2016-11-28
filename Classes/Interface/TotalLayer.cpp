
#include "TotalLayer.h"
#include "TouchSwallowLayer.h"
#include "CommonFunction.h"
#include "Data/GameData.h"
#include "Scene/LevelScene.h"
#include "Scene/StartScene.h"
#include "Data/Data.h"
#include "Scene/GameScene.h"
#include "Interface/ObjectiveLayer.h"

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

	auto comonon = Sprite::create("popbox/gift.png");
	comonon->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(0, 100)));
	BG_kuang->addChild(comonon);

	auto nextLevel = Button::create("button/next_level.png");
	nextLevel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,BG_kuang,Vec2(0,80)));
	BG_kuang->addChild(nextLevel);
	nextLevel->addClickEventListener(CC_CALLBACK_0(TotalLayer::GoOnCallBack, this));


	auto line = Sprite::create("popbox/line.png");
	line->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,BG_kuang,Vec2(0,150)));
	BG_kuang->addChild(line);

	addGift(line);

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
		log("////%d",num);
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
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
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
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
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
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
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
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
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
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
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
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
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
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
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
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
	{
		AudioData::getInstance()->addButtonEffect(1);
	}
	log("GoOnCallBack");
	int x = GAMEDATA->getCurLevel() + 1;
	if ( x < GAMEDATA->m_lock.size())
	{
		GAMEDATA->setCurLevel(x);
		GAMEDATA->setLock(GAMEDATA->getCurLevel());
		if (GAMEDATA->getPowerNum()>= 2)
		{
			GAMEDATA->setPowerNum(GAMEDATA->getPowerNum() -2);  //扣两点体力开始新游戏
			auto scene = GameScene::create();
			Director::getInstance()->replaceScene(TransitionFade::create(1,scene));
		}
		else
		{
			cout << "体力不够了！" << endl;
			LevelScene* levelScene = LevelScene::create();
			Director::getInstance()->replaceScene(TransitionCrossFade::create(1, levelScene));
		}


	}
	else{
		log("no level");
		StartScene* scene = StartScene::create();
		Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(0.8,scene));
	}

}

void TotalLayer::backCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
	{
		AudioData::getInstance()->addButtonEffect(1);
	}

	int x = GAMEDATA->getCurLevel() + 1;
	if (x < GAMEDATA->m_lock.size())
	{
		GAMEDATA->setCurLevel(x);
		GAMEDATA->setLock(GAMEDATA->getCurLevel());
	}
	log("backCallBack");
	//this->removeFromParent();
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