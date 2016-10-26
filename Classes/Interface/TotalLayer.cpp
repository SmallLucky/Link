
#include "TotalLayer.h"
#include "TouchSwallowLayer.h"
#include "CommonFunction.h"
#include "Data/GameData.h"
#include "Scene/LevelScene.h"
#include "Scene/StartScene.h"

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
	auto sp = Sprite::create("bg/bg_1.png");
	sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,0)));
	addChild(sp);
	auto cat = Sprite::create("popbox/cat_2.png");
	cat->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,330)));
	addChild(cat);
	auto BG_kuang = Sprite::create("popbox/popbox_big_bg.png");
	BG_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, cat, Vec2(-30, -310)));
	cat->addChild(BG_kuang);

	auto star1 = Sprite::create("popbox/star_1.png");
	star1->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,cat,Vec2(-170,-20)));
	cat->addChild(star1);
	auto star2 = Sprite::create("popbox/star_2.png");
	star2->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, cat, Vec2(-30, 30)));
	cat->addChild(star2);
	auto star3 = Sprite::create("popbox/star_1.png");
	star3->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, cat, Vec2(180, 0)));
	star3->setScale(0.6);
	cat->addChild(star3);

	auto successful = Sprite::create("popbox/successful.png");
	successful->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang,Vec2(30,240)));
	BG_kuang->addChild(successful);

	auto di = Sprite::create("popbox/total_di.png");
	di->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, successful, Vec2(-40, -10)));
	successful->addChild(di);

	auto num = LabelAtlas::create(Value(GAMEDATA->getCurLevel() + 1).asString(), "fonts/infor_num_1.png", 20, 30, '0');
	num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, di, Vec2(10, 0)));
	num->setAnchorPoint(Vec2(0, 0.5));
	di->addChild(num);

	auto guan = Sprite::create("popbox/total_guan.png");
	guan->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, num, Vec2(30, 0)));
	num->addChild(guan);

	auto success_bg = Sprite::create("popbox/successful_bg.png");
	success_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,BG_kuang,Vec2(30,-10)));
	BG_kuang->addChild(success_bg);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			auto label_kuang = Sprite::create("popbox/labelkuang_h.png");
			label_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,success_bg,Vec2(90 + (i*180 - 180) ,(j*90 - 90) +30)));
			success_bg->addChild(label_kuang);
			auto gift = Sprite::create("popbox/star_1.png");
			gift->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid,label_kuang,Vec2(30,0)));
			label_kuang->addChild(gift);
		}
	}

	auto goOnButton = Button::create("popbox/next_level.png");
	goOnButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(30, 160)));
	BG_kuang->addChild(goOnButton);
	goOnButton->addClickEventListener(CC_CALLBACK_0(TotalLayer::GoOnCallBack,this));

	auto backButton = Button::create("popbox/button_cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(30, -10)));
	BG_kuang->addChild(backButton);
	backButton->addClickEventListener(CC_CALLBACK_0(TotalLayer::backCallBack, this));

	//auto level = Label::createWithTTF(Value(GAMEDATA->getCurLevel()+1).asString(), "fonts/Marker Felt.ttf", 60);
	//level->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(0, 100)));
	//BG_kuang->addChild(level);

	////star score
	//auto scoreLabel = Label::createWithTTF("Score:", "fonts/Marker Felt.ttf", 60);
	//scoreLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(-100, 0)));
	//BG_kuang->addChild(scoreLabel);

	//auto scoreNum = Label::createWithTTF(Value(GAMEDATA->getCurSocre()).asString(), "fonts/Marker Felt.ttf", 60);
	//scoreNum->setAnchorPoint(Vec2(0, 0.5));
	//scoreNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, scoreLabel, Vec2(70, 0)));
	//scoreLabel->addChild(scoreNum);
}

void TotalLayer::GoOnCallBack()
{
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
	log("backCallBack");
	this->removeFromParent();
	LevelScene* levelScene = LevelScene::create();
	Director::getInstance()->replaceScene(levelScene);
}