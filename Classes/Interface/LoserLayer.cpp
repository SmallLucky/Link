
#include "LoserLayer.h"
#include "TouchSwallowLayer.h"
#include "Data/GameData.h"
#include "CommonFunction.h"
#include "Scene/GameScene.h"
#include "Scene/LevelScene.h"

bool LoserLayer:: init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto tuchSwall = TouchSwallowLayer::create();
	addChild(tuchSwall);

	GAMEDATA->praseJsonData();

	addUI();
	return true;
}

void LoserLayer::addUI()
{
	auto cat = Sprite::create("popbox/cat_3.png");
	cat->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,330)));
	addChild(cat);

	auto BG_kuang = Sprite::create("popbox/popbox_big_bg.png");
	BG_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, cat, Vec2(-30, -310)));
	cat->addChild(BG_kuang);

	auto hart1 = Sprite::create("popbox/hart_broken.png");
	hart1->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, cat, Vec2(-170, -20)));
	cat->addChild(hart1);
	auto hart2 = Sprite::create("popbox/hart_broken.png");
	hart2->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, cat, Vec2(-30, 30)));
	hart2->setScale(0.5);
	cat->addChild(hart2);
	auto hart3 = Sprite::create("popbox/hart_broken.png");
	hart3->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, cat, Vec2(180, -5)));
	hart3->setScale(0.7);
	cat->addChild(hart3);

	auto failure = Sprite::create("popbox/failure.png");
	failure->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(30, 240)));
	BG_kuang->addChild(failure);

	auto di = Sprite::create("popbox/failure_di.png");
	di->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, failure, Vec2(-40, -10)));
	failure->addChild(di);

	auto num = LabelAtlas::create(Value(GAMEDATA->getCurLevel() + 1).asString(), "fonts/loser_fonts.png", 19.5, 30, '0');
	num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, di, Vec2(10, -5)));
	num->setAnchorPoint(Vec2(0, 0.5));
	di->addChild(num);

	auto guan = Sprite::create("popbox/failure_guan.png");
	guan->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, num, Vec2(30, 5)));
	num->addChild(guan);

	auto failure_bg = Sprite::create("popbox/loser_bg.png");
	failure_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(30, -10)));
	BG_kuang->addChild(failure_bg);

	auto onceAgain = Sprite::create("popbox/nextone_canbe.png");
	onceAgain->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,failure_bg,Vec2(0,0)));
	failure_bg->addChild(onceAgain);

	auto againButton = Button::create("popbox/once_again.png");
	againButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(30, 180)));
	BG_kuang->addChild(againButton);
	againButton->addClickEventListener(CC_CALLBACK_0(LoserLayer::againCallBack, this));

	auto quitButton = Button::create("popbox/button_cancel.png");
	quitButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(30, -10)));
	BG_kuang->addChild(quitButton);
	quitButton->addClickEventListener(CC_CALLBACK_0(LoserLayer::quitCallBack, this));


	//auto score = Label::createWithTTF(Value(GAMEDATA->getTargetScore(GAMEDATA->getCurLevel()) - GAMEDATA->getCurSocre()).asString(), "fonts/Marker Felt.ttf", 60);
	//score->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,BG_kuang,Vec2(0,50)));
	//BG_kuang->addChild(score);

	//auto againButton = Button::create("button/set.png");
	//againButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,BG_kuang,Vec2(0,0)));
	//BG_kuang->addChild(againButton);
	//againButton->addClickEventListener(CC_CALLBACK_0(LoserLayer::againCallBack,this));

	//auto quitButton = Button::create("button/Cancel.png");
	//quitButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,BG_kuang,Vec2(0,0)));
	//BG_kuang->addChild(quitButton);
	//quitButton->addClickEventListener(CC_CALLBACK_0(LoserLayer::quitCallBack,this));
}

void LoserLayer::quitCallBack()
{
	auto gameScene = LevelScene::create();
	Director::getInstance()->replaceScene(gameScene);
}

void LoserLayer::againCallBack()
{
	log("again game");
	auto gameScene = GameScene::create();
	Director::getInstance()->replaceScene(gameScene);
}