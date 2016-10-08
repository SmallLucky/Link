
#include "LevelLayer.h"
#include "Data/GameData.h"
#include "CommonFunction.h"
#include "Scene/GameScene.h"
#include "Scene/StartScene.h"
#include "ObjectiveLayer.h"
#include "ShopLayer.h"
#include "SetLayer.h"

//#define  GAMEDATA GameData::getInstance()
bool LevelLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	GameData::getInstance()->praseJsonData();
	//测试修改
	numBG = 0;
	
	visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* bg1 = Sprite::create("bg/bg_1.png");
	bg1->setAnchorPoint(Vec2(0.5,0));
	bg1->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,Vec2(0,0)));
	addChild(bg1);

	addScrollView();
	addLevelButton();
	addUI();
	return true;
}
void	LevelLayer::addScrollView()
{

	scrollview = ScrollView::create();
	scrollview->setAnchorPoint(Vec2(0.5, 0));
	scrollview->setContentSize(Director::getInstance()->getVisibleSize());
	scrollview->setInnerContainerSize(Size(visibleSize.width, visibleSize.height * 3));
	scrollview->setDirection(ScrollView::Direction::VERTICAL);
	scrollview->setBackGroundColor(Color3B::GRAY);
	scrollview->setBackGroundColorOpacity(0);
	scrollview->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	scrollview->setPosition(Vec2(visibleSize.width / 2, 0));
	scrollview->setInertiaScrollEnabled(true);
	scrollview->setBounceEnabled(true);
	scrollview->setScrollBarPositionFromCorner(Vec2(10, 0));
	scrollview->jumpToBottom();
	addChild(scrollview);
	for (int i = 0; i < 3; i++)
	{
		auto bg = Sprite::create("bg/bg_1.png");
		bg->setAnchorPoint(Vec2(0.5, 0));
		bg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, scrollview, Vec2(0, i*bg->getContentSize().height)));
		scrollview->addChild(bg);
	}
}
void	LevelLayer::addUI()
{

	for (int i = 0; i < 3; i++)
	{
		auto small_kuang = Sprite::create("popbox/labelkuang_w.png");
		small_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, Vec2((i*200 -200), -50)));
		addChild(small_kuang);
		if (i == 0)
		{
			auto power = Sprite::create("popbox/power.png");
			power->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, small_kuang, Vec2(20, 0)));
			small_kuang->addChild(power);

			auto powerNum = LabelAtlas::create(Value(GAMEDATA->getPowerNum()).asString(),"fonts/level_fonts.png",30,30,'0');
			powerNum->setAnchorPoint(Vec2(0.5, 0.5));
			powerNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,small_kuang,Vec2(10,0)));
			small_kuang->addChild(powerNum);

		}
		if (i == 1)
		{
			auto money = Sprite::create("popbox/big_gold.png");
			money->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, small_kuang, Vec2(20, 0)));
			small_kuang->addChild(money);
			auto addbutton = Button::create("popbox/add_button.png");
			addbutton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, small_kuang, Vec2(-10, 0)));
			small_kuang->addChild(addbutton);
			addbutton->addClickEventListener(CC_CALLBACK_0(LevelLayer::addCallBack,this));

			auto moneyNum = LabelAtlas::create(Value(GAMEDATA->getMoneyNum()).asString(), "fonts/level_fonts.png", 30, 30, '0');
			moneyNum->setAnchorPoint(Vec2(0.5, 0.5));
			moneyNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, small_kuang, Vec2(10, 0)));
			small_kuang->addChild(moneyNum);
		}
		if (i == 2)
		{
			auto love = Sprite::create("popbox/big_hart.png");
			love->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, small_kuang, Vec2(20, 0)));
			small_kuang->addChild(love);
			auto addbutton = Button::create("popbox/add_button.png");
			addbutton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, small_kuang, Vec2(-10, 0)));
			small_kuang->addChild(addbutton);
			addbutton->addClickEventListener(CC_CALLBACK_0(LevelLayer::addCallBack, this));

			auto loveNum = LabelAtlas::create(Value(GAMEDATA->getLoveNum()).asString(), "fonts/level_fonts.png", 30, 30, '0');
			loveNum->setAnchorPoint(Vec2(0.5, 0.5));
			loveNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, small_kuang, Vec2(10, 0)));
			small_kuang->addChild(loveNum);
			
		}
	}

	auto shopButton = Button::create("popbox/shop_button.png");
	shopButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom,Vec2(-100,100)));
	addChild(shopButton);
	shopButton->addClickEventListener(CC_CALLBACK_0(LevelLayer::addCallBack,this));

	auto setButton = Button::create("popbox/button_set.png");
	setButton->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom,Vec2(100,100)));
	addChild(setButton);
	setButton->addClickEventListener(CC_CALLBACK_0(LevelLayer::setCallBack, this));


}

//添加关卡
void LevelLayer::addLevelButton()
{


	for (int i = 0; i <GAMEDATA->m_lock.size(); i++)
	{
		auto testButton = Button::create("button/level_normal.png", "button/level_pass.png", "button/level_disable.png");
		testButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, scrollview, Vec2(GAMEDATA->getOffsetX(i),  GAMEDATA->getOffsetY(i))));
		scrollview->addChild(testButton);
		testButton->setTag(i);
		auto level_num = LabelAtlas::create(Value(i +1).asString(),"fonts/level_fonts.png",30,30,'0');
		level_num->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, testButton, Vec2(-15,0)));
		testButton->addChild(level_num);

		if (GameData::getInstance()->m_lock[i] == 1)
		{
			testButton->setBright(false);
			testButton->setTouchEnabled(false);
		}
		//testButton->addClickEventListener(CC_CALLBACK_0(LevelLayer::test_buttonCallBack, this));
		testButton->addClickEventListener([=](Ref* sender){
			GameData::getInstance()->setCurLevel(i);
			log("getCurLevel:%i", GameData::getInstance()->getCurLevel());
			//GAMEDATA->setCurLevel(GAMEDATA->getCurLevel() + 1);
			ObjectiveLayer* layer = ObjectiveLayer::create();
			addChild(layer);
		});
	}

}

void LevelLayer:: back_buttonCallBack()
{
	log("test_battonCallBack");
	StartScene* startScene = StartScene::create();
	Director::getInstance()->replaceScene(startScene);
}

void LevelLayer::refreshUI()
{
	log("refreshUI");
}

void LevelLayer::addCallBack()
{
	ShopLayer* shop = ShopLayer::create();
	addChild(shop);
}

void LevelLayer::setCallBack()
{
	SetLayer* setLayer = SetLayer::create();
	addChild(setLayer);
}



//		//	log("....");
//		//	Point nowPoint = convertToWorldSpace(scrollview->getInnerContainerPosition());//将坐标转换为世界坐标
//		//	log("point old:%f,now:%f", oldPoint.y, nowPoint.y);
//		//	float l = nowPoint.y - oldPoint.y;
//		//	float x = l / 10;
//		//	log("x:%f", x);
//		//	MoveBy* moveby = MoveBy::create(0.5, Vec2(0, x));
//		//	bgLayer->runAction(moveby);
//		//	oldPoint = nowPoint;
//		//	int y = 0;
//		//	if (bgLayer->getPositionY()<0)
//		//	{
//		//		y = (0 - bgLayer->getPositionY()) / HEIGHT + 1;
//		//		log("<<<<<<<<%f , y: %i", bg1->getPositionY(), y);
//		//		if (numBG != y || numBG > y)
//		//		{
//		//			Sprite* bg = Sprite::create("bg/bg1.png");
//		//			log("point*****************:%f,%f", WIDTH / 2, HEIGHT / 2 * y);
//		//			bg->setAnchorPoint(Vec2(0, 0));
//		//			bg->setPosition(0, HEIGHT*y);
//		//			bgLayer->addChild(bg);
//		//			numBG = y;
//		//		}
//		//	}