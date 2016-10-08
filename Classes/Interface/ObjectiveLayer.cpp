
#include "ObjectiveLayer.h"
#include "CommonFunction.h"
#include "Data/GameData.h"
#include "TouchSwallowLayer.h"
#include "Scene/GameScene.h"


#define GAMEDATA GameData::getInstance()
bool ObjectiveLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	GameData::getInstance()->praseJsonData();

	TouchSwallowLayer* touchSwall = TouchSwallowLayer::create();
	addChild(touchSwall);

	addUI();

	return true;
}

void ObjectiveLayer::addUI()
{

	auto cat = Sprite::create("popbox/cat_1.png");
	cat->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 330)));
	addChild(cat);

	auto BG_kuang = Sprite::create("popbox/popbox_big_bg.png");
	BG_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, cat, Vec2(-30, -310)));
	cat->addChild(BG_kuang);

	auto di = Sprite::create("popbox/mubiao_di.png");
	di->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,BG_kuang,Vec2(-20,230)));
	BG_kuang->addChild(di);

	auto num = LabelAtlas::create(Value(GAMEDATA->getCurLevel()+1).asString(),"fonts/infor_num_1.png",20,30,'0');
	num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,di,Vec2(10,0)));
	num->setAnchorPoint(Vec2(0,0.5));
	num->setScale(1.5);
	di->addChild(num);
	
	auto guan = Sprite::create("popbox/mubiao_guan.png");
	guan->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,num,Vec2(30,0)));
	guan->setScale(0.7);
	num->addChild(guan);

	auto target_bg = Sprite::create("popbox/target_bg.png");
	target_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(30, -10)));
	BG_kuang->addChild(target_bg);

	auto score_bg = Sprite::create("popbox/labelkuang_l.png");
	score_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, target_bg, Vec2(0, -20)));
	target_bg->addChild(score_bg);

	auto scoreNum = Label::createWithTTF(Value(GAMEDATA->getTargetScore(GAMEDATA->getCurLevel())).asString(), "fonts/Marker Felt.ttf", 60);
	scoreNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, score_bg, Vec2(0, 0)));
	score_bg->addChild(scoreNum);

	starGame = Button::create("popbox/start_game.png");
	starGame->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(30, 200)));
	BG_kuang->addChild(starGame);
	starGame->addClickEventListener(CC_CALLBACK_0(ObjectiveLayer::startGame, this));

	auto backButton = Button::create("popbox/button_cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(30, -10)));
	BG_kuang->addChild(backButton);
	backButton->addClickEventListener(CC_CALLBACK_0(ObjectiveLayer::backButton, this));


}

void ObjectiveLayer::startGame()
{
	GAMEDATA->setPowerNum(GAMEDATA->getPowerNum() - 3);

	auto power = Sprite::create("popbox/power.png");
	power->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, starGame,Vec2(-200, 600)));
	power->setScale(0.1);
	starGame->addChild(power);
	Point powerPoint = CommonFunction::getVisibleAchor(Anchor::Center, starGame, Vec2(40, 0));
	auto moveTo = MoveTo::create(1.0, powerPoint);
	ScaleTo* scaleTo = ScaleTo::create(1,1);
	Spawn* spawn = Spawn::create(moveTo, scaleTo,nullptr);
	auto action = Sequence::create(spawn, CallFunc::create([&]{
		GameScene* gameScene = GameScene::create();
		Director::getInstance()->replaceScene(gameScene);  //Ìø×ª
	}), nullptr);

	power->runAction(action);
}
void ObjectiveLayer::backButton()
{
	this->removeFromParent();
}