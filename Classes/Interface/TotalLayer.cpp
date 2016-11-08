
#include "TotalLayer.h"
#include "TouchSwallowLayer.h"
#include "CommonFunction.h"
#include "Data/GameData.h"
#include "Scene/LevelScene.h"
#include "Scene/StartScene.h"
#include "Data/Data.h"

bool TotalLayer:: init()
{
	if (!Layer::init())
	{
		return false;
	}
	GAMEDATA->praseJsonData();
	auto touchSwall = TouchSwallowLayer::create();
	addChild(touchSwall);

	addUI();

	return true;
}

void TotalLayer::addUI()
{
	//auto sp = Sprite::create("bg/bg_1.png");
	//sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,0)));
	//addChild(sp);

	auto BG_kuang = Sprite::create("popbox/dikuang.png");
	BG_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 0)));
	addChild(BG_kuang);

	auto pet = Sprite::create("popbox/pet_3.png");
	pet->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, BG_kuang,Vec2(0, 100)));
	BG_kuang->addChild(pet);

	auto hong = Sprite::create("popbox/sucess_hong.png");
	hong->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, BG_kuang, Vec2(0, -40)));
	BG_kuang->addChild(hong);

	auto levelnumbg = Sprite::create("popbox/success_levenumbg.png");
	levelnumbg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, hong, Vec2(0, 0)));
	hong->addChild(levelnumbg);

	auto level = LabelAtlas::create(Value(GAMEDATA->getCurLevel()+1).asString(), "fonts/success_levelnum.png", 17, 24, '0');
	level->setAnchorPoint(Vec2(0.5, 0.5));
	level->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, levelnumbg, Vec2(0, 0)));
	levelnumbg->addChild(level);

	auto comonon = Sprite::create("popbox/gift.png");
	comonon->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(0, 100)));
	BG_kuang->addChild(comonon);

	auto nextLevel = Button::create("button/next_level.png");
	nextLevel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,BG_kuang,Vec2(0,80)));
	BG_kuang->addChild(nextLevel);
	nextLevel->addClickEventListener(CC_CALLBACK_0(TotalLayer::backCallBack, this));


	auto line = Sprite::create("popbox/line.png");
	line->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,BG_kuang,Vec2(0,150)));
	BG_kuang->addChild(line);

	bool ispower = true;
	bool islove = true;
	bool iscoins = true;
	bool isrefresh = true;
	bool isboom = true;
	for (int i = 0; i < 4; i++) //0123
	{
		auto giftBg = Sprite::create("popbox/success_propsbg.png");
		giftBg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,line,Vec2(170 - (i*115),90)));
		line->addChild(giftBg);
		for (int j = 0; j < 5; j++)
		{
			if (REWARDDATA->getPower(GAMEDATA->getCurLevel()) != 0  && ispower)
			{
				auto power = Sprite::create("popbox/power.png");
				power->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,giftBg,Vec2(0,0)));
				power->setScale(1.5);
				giftBg->addChild(power);
				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom,giftBg,Vec2(0,0)));
				giftBg->addChild(x);

				auto num = LabelAtlas::create(Value(REWARDDATA->getPower(GAMEDATA->getCurLevel())).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,x,Vec2(0,0)));
				num->setAnchorPoint(Vec2(0,0.5));
				x->addChild(num);
				ispower = false;
				break;
			}
			else if (REWARDDATA->getRefresh(GAMEDATA->getCurLevel()) != 0 && isrefresh)
			{
				auto refresh = Sprite::create("infor/props_0.png");
				refresh->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				giftBg->addChild(refresh);

				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
				giftBg->addChild(x);

				auto num = LabelAtlas::create(Value(REWARDDATA->getRefresh(GAMEDATA->getCurLevel())).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
				num->setAnchorPoint(Vec2(0, 0.5));
				x->addChild(num);
				isrefresh = false;
				break;
			}
			else if(REWARDDATA->getBoom(GAMEDATA->getCurLevel()) != 0 && isboom)
			{
				auto boom = Sprite::create("infor/props_1.png");
				boom->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				giftBg->addChild(boom);

				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
				giftBg->addChild(x);

				auto num = LabelAtlas::create(Value(REWARDDATA->getBoom(GAMEDATA->getCurLevel())).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
				num->setAnchorPoint(Vec2(0, 0.5));
				x->addChild(num);
				isboom = false;
				break;
			}
			else if(REWARDDATA->getCoins(GAMEDATA->getCurLevel()) != 0 && iscoins)
			{
				auto coins = Sprite::create("popbox/big_gold.png");
				coins->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				coins->setScale(1.5);
				giftBg->addChild(coins);

				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
				giftBg->addChild(x);
				auto num = LabelAtlas::create(Value(REWARDDATA->getCoins(GAMEDATA->getCurLevel())).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
				num->setAnchorPoint(Vec2(0, 0.5));
				x->addChild(num);
				iscoins = false;
				break;
			}
			else if(REWARDDATA->getLove(GAMEDATA->getCurLevel()) != 0 && islove)
			{
				auto love = Sprite::create("infor/big_hart.png");
				love->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, giftBg, Vec2(0, 0)));
				love->setScale(1.5);
				giftBg->addChild(love);
				auto x = Sprite::create("popbox/X.png");
				x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, giftBg, Vec2(0, 0)));
				giftBg->addChild(x);
				auto num = LabelAtlas::create(Value(REWARDDATA->getLove(GAMEDATA->getCurLevel())).asString(), "fonts/succes_propsnum.png", 27, 34, '0');
				num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(0, 0)));
				num->setAnchorPoint(Vec2(0, 0.5));
				x->addChild(num);
				islove = false;
				break;
			}
		}
	}

	auto backButton = Button::create("popbox/cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, BG_kuang, Vec2(-20, -20)));
	BG_kuang->addChild(backButton);
	backButton->addClickEventListener(CC_CALLBACK_0(TotalLayer::backCallBack, this));

}

void TotalLayer::GoOnCallBack()
{
	addReward();
	log("GoOnCallBack");
	int x = GAMEDATA->getCurLevel() + 1;
	if ( x < GAMEDATA->m_lock.size())
	{
		GAMEDATA->setCurLevel(x);
		GAMEDATA->setLock(GAMEDATA->getCurLevel());
		GAMEDATA->setPowerNum(min(GAMEDATA->getPowerNum() + 1,500)); // ¹ý¹Ø½±Àø
		LevelScene* levelScene = LevelScene::create();
		Director::getInstance()->replaceScene(levelScene);
	}
	else{
		log("no level");
		StartScene* scene = StartScene::create();
		Director::getInstance()->replaceScene(scene);
	}

}

void TotalLayer::backCallBack()
{
	addReward();
	log("backCallBack");
	this->removeFromParent();
	LevelScene* levelScene = LevelScene::create();
	Director::getInstance()->replaceScene(levelScene);
}

void	TotalLayer:: addReward()
{

}