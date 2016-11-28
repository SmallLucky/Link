
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

	auto againButton = Button::create("button/oneAgain.png");
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
	log("again game");
	//再玩一次减少体力
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addButtonEffect(1);
	if (GAMEDATA->getPowerNum()>= 2)
	{
		GAMEDATA->setPowerNum(GAMEDATA->getPowerNum() - 2);
		auto gameScene = GameScene::create();
		Director::getInstance()->replaceScene(TransitionFade::create(0.7,gameScene));
	}
	else
	{
		//提示体力不足，去购买！~！！缺界面
	}

}