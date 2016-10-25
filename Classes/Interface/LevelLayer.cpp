
#include "LevelLayer.h"
#include "Data/GameData.h"
#include "CommonFunction.h"
#include "Scene/GameScene.h"
#include "Scene/StartScene.h"
#include "ObjectiveLayer.h"
#include "ShopLayer.h"
#include "SetLayer.h"

//#define  GAMEDATA GameData::getInstance()
LevelLayer::LevelLayer()
{
	auto _listenerReresh = EventListenerCustom::create(REFRESHUI, [=](EventCustom*event){
		refreshUI();
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerReresh, 1);
}
LevelLayer::~LevelLayer()
{
	_eventDispatcher->removeCustomEventListeners(REFRESHUI);
}
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
	if (bg1)
	{
		bg1->setAnchorPoint(Vec2(0.5, 0));
		bg1->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Vec2(0, 0)));
		addChild(bg1);
	}

	addScrollView();
	addLevelButton();
	addUI();

	if (GAMEDATA->getPowerNum() == 500)
	{
		log("addTimeLabel");
		UserDefault::getInstance()->setIntegerForKey("GAME_TIME",0);
		UserDefault::getInstance()->setIntegerForKey("MIN_TIME",0);
	}
	else
	{
		//addlabel
		if ((mt % 10) > (500 - GAMEDATA->getPowerNum()))
		{
			GAMEDATA->setPowerNum(500);
			UserDefault::getInstance()->setIntegerForKey("GAME_TIME", 0);
			UserDefault::getInstance()->setIntegerForKey("MIN_TIME", 0);
		}
		else
		{
			timeUI();
			this->scheduleUpdate();
		}
	}

	return true;
}



void	LevelLayer::addScrollView()
{

	scrollview = ScrollView::create();
	if (scrollview)
	{
		scrollview->setAnchorPoint(Vec2(0.5, 0));
		scrollview->setContentSize(Director::getInstance()->getVisibleSize());
		scrollview->setInnerContainerSize(Size(visibleSize.width, visibleSize.height * 3));
		scrollview->setDirection(ScrollView::Direction::VERTICAL);
		scrollview->setBackGroundColor(Color3B::GRAY);
		scrollview->setBackGroundColorOpacity(0);
		scrollview->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
		scrollview->setPosition(Vec2(visibleSize.width / 2, 0));
		scrollview->setInertiaScrollEnabled(true); //设置惯性滚动启动
		scrollview->setBounceEnabled(false); // 设置回弹效果为flase
		scrollview->setScrollBarPositionFromCorner(Vec2(10, 0));
		scrollview->setScrollBarOpacity(0); //设置滚动条透明度
		scrollview->jumpToBottom();
		int l = visibleSize.height;
		log("*****%.2f", (float)((l * 3 - 160)*100 / (l * 3)));
		int index = 0;
		for (int i = 0; i < GAMEDATA->m_lock.size(); i++)
		{
			if (GAMEDATA->m_lock[i] == 1)
			{
				index = i - 1;
				break;
			}
		}
		float p = (float)((l * 3 - GAMEDATA->getOffsetY(index)) * 100 / (l * 3));
		//scrollview->jumpToPercentVertical(p);
		if (p < 70)
		{
			scrollview->scrollToPercentVertical(p, 5.0, true); //显示百分比
		}
		else
		{
			scrollview->jumpToPercentVertical(p);
		}
		/*scrollview->set*/
			//LinearGravity
		addChild(scrollview);
		for (int i = 0; i < 3; i++)
		{
			auto bg = Sprite::create("bg/bg_1.png");
			if (bg)
			{
				bg->setAnchorPoint(Vec2(0.5, 0));
				bg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, scrollview, Vec2(0, i*bg->getContentSize().height)));
				scrollview->addChild(bg);
			}
		}
	}

}
void	LevelLayer::addUI()
{

	for (int i = 0; i < 3; i++)
	{
		auto small_kuang = Sprite::create("popbox/labelkuang_w.png");
		if (small_kuang)
		{
			small_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, Vec2((i * 200 - 200), -50)));
			addChild(small_kuang);
			if (i == 0)
			{
				auto power = Sprite::create("popbox/power.png");
				if (power)
				{
					power->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, small_kuang, Vec2(20, 0)));
					small_kuang->addChild(power);
				}
				powerNum = LabelAtlas::create(Value(GAMEDATA->getPowerNum()).asString(), "fonts/level_fonts.png", 30, 30, '0');
				if (powerNum)
				{
					powerNum->setAnchorPoint(Vec2(0.5, 0.5));
					powerNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, small_kuang, Vec2(10, 0)));
					small_kuang->addChild(powerNum);
				}
			}
			if (i == 1)
			{
				auto money = Sprite::create("popbox/big_gold.png");
				money->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, small_kuang, Vec2(20, 0)));
				small_kuang->addChild(money);
				auto addbutton = Button::create("popbox/add_button.png");
				addbutton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, small_kuang, Vec2(-10, 0)));
				small_kuang->addChild(addbutton);
				addbutton->addClickEventListener(CC_CALLBACK_0(LevelLayer::addCallBack, this));

				moneyNum = LabelAtlas::create(Value(GAMEDATA->getMoneyNum()).asString(), "fonts/level_fonts.png", 30, 30, '0');
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

				loveNum = LabelAtlas::create(Value(GAMEDATA->getLoveNum()).asString(), "fonts/level_fonts.png", 30, 30, '0');
				loveNum->setAnchorPoint(Vec2(0.5, 0.5));
				loveNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, small_kuang, Vec2(10, 0)));
				small_kuang->addChild(loveNum);

			}
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
void  LevelLayer::timeUI() //
{
	long int t = millisecondNow() - UserDefault::getInstance()->getIntegerForKey("GAME_TIME"); //得到系统时间毫秒
	//long int nowt = millisecondNow(); //现在的时间 
	//nowt - t;
	long int tt = t / 1000; // 秒
	
	mt = tt / 60;
	st = tt % 60;
	log("tt:%ld   %d:%d", tt,mt,st);
	
	minutsNum = mt - UserDefault::getInstance()->getIntegerForKey("MIN_TIME",0);
	//log("****time:%ld ,millisecondNow:%ld  minutsNum:%ld ", t, millisecondNow(), minutsNum);
	//GAMEDATA->setPowerNum(GAMEDATA->getPowerNum() + minutsNum / 10);
	//refreshUI(); //
	minutes = LabelAtlas::create(Value(9 - minutsNum).asString(), "fonts/level_fonts.png", 30, 30, '0');
	minutes->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,powerNum,Vec2(0,-40)));
	minutes->setAnchorPoint(Vec2(1,0.5));
	powerNum->addChild(minutes);
	auto f = Label::create(":","fonts/Marker Felt.ttf",30);
	f->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, minutes, Vec2(5, 0)));
	f->setAnchorPoint(Vec2(0.5, 0.5));
	minutes->addChild(f);
	seconds = LabelAtlas::create(Value(59 - st).asString(), "fonts/level_fonts.png", 30, 30, '0');
	seconds->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid , minutes,Vec2(10,0)));
	seconds->setAnchorPoint(Vec2(0, 0.5));
	minutes->addChild(seconds);

}
void  LevelLayer::setTimeUI() //
{
	long int t = millisecondNow() - UserDefault::getInstance()->getIntegerForKey("GAME_TIME"); //得到系统时间毫秒
	//long int nowt = millisecondNow(); //现在的时间 
	//nowt - t;
	long int tt = t / 1000; // 秒

	mt = tt / 60;
	st = tt % 60;
	log("tt:%ld   %d:%d", tt, mt, st);

	minutsNum = mt - UserDefault::getInstance()->getIntegerForKey("MIN_TIME", 0);
	log("****time:%ld ,millisecondNow:%ld  minutsNum:%ld ", t, millisecondNow(), minutsNum);
	if (minutes)
	{
		minutes->setString(Value(9-minutsNum).asString());
		seconds->setString(Value(59 - st).asString());
	}
	if (minutsNum % 10 == 0 && minutsNum != 0)
	{
		GAMEDATA->setPowerNum(GAMEDATA->getPowerNum() + 1);
		UserDefault::getInstance()->setIntegerForKey("MIN_TIME", minutsNum);
		refreshUI(); //]
		if (GAMEDATA->getPowerNum() == 500)
		{
			UserDefault::getInstance()->setIntegerForKey("GAME_TIME", 0);
			UserDefault::getInstance()->setIntegerForKey("MIN_TIME", 0);
			//移除时间label
			minutes->removeFromParent();
			//关闭定时器
			this->unscheduleUpdate();

		}
	}
}
void  LevelLayer::update(float dt) // 
{
	setTimeUI();
}
long LevelLayer::millisecondNow()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
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
			if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT"))
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
			}
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
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT"))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
	}
	StartScene* startScene = StartScene::create();
	Director::getInstance()->replaceScene(startScene);
}
 //刷新数值label  
void LevelLayer::refreshUI()
{
	log("refreshUI");
	powerNum->setString(Value(GAMEDATA->getPowerNum()).asString());
	loveNum->setString(Value(GAMEDATA->getLoveNum()).asString());
	moneyNum->setString(Value(GAMEDATA->getMoneyNum()).asString());
}

void LevelLayer::addCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT"))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
	}
	ShopLayer* shop = ShopLayer::create();
	addChild(shop);
}

void LevelLayer::setCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT"))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
	}
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