
#include "LevelLayer.h"
#include "Data/GameData.h"
#include "CommonFunction.h"
#include "Scene/GameScene.h"
#include "Scene/StartScene.h"
#include "ObjectiveLayer.h"
#include "ShopLayer.h"
#include "SetLayer.h"
#include "PowerShop.h"
#include "Data/RewardData.h"

LevelLayer::LevelLayer() :mt(0),minutes(nullptr)
{
	auto _listenerReresh = EventListenerCustom::create(REFRESHUI, [=](EventCustom*event){
		refreshUI();
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerReresh, 1);
}
LevelLayer::~LevelLayer()
{
	_eventDispatcher->removeCustomEventListeners(REFRESHUI);

	if (GAMEDATA->getPowerNum() == MAXPOWER)
	{
		UserDefault::getInstance()->setIntegerForKey("LEAVE_TIME",0);
		UserDefault::getInstance()->setIntegerForKey("LEAVE_POWER",0);
	}
	else
	{
		if (UserDefault::getInstance()->getIntegerForKey("LEAVE_TIME",0) == 0)
		{
			UserDefault::getInstance()->setIntegerForKey("LEAVE_TIME", millisecondNow());	
		}
		UserDefault::getInstance()->setIntegerForKey("LEAVE_POWER", mt);
		log("wo jiao mt:%d", mt);
	}
}
bool LevelLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	GameData::getInstance()->praseJsonData();
	RewardData::getInstance()->praseJsonData();

	bool flag = UserDefault::getInstance()->getBoolForKey("IS_MUSIC", true);
	if (flag)
	{
		AudioData::getInstance()->addBgMusic(2);
	}
	else
	{
		AudioData::getInstance()->stopBgMusic();
	}

	//测试修改
	numBG = 0;
	minutsNum = 0;
	visibleSize = Director::getInstance()->getVisibleSize();

	addScrollView();
	addLevelButton();
	addUI();

	addEffectYun();


	if (GAMEDATA->getPowerNum() == MAXPOWER)
	{
		//log("addTimeLabel");
		UserDefault::getInstance()->setIntegerForKey("GAME_TIME",0);//游戏开始按钮减体力的时间写入
		UserDefault::getInstance()->setIntegerForKey("MIN_TIME",0);//
		UserDefault::getInstance()->setIntegerForKey("LEAVE_TIME", 0);//离开的时间记录
		UserDefault::getInstance()->setIntegerForKey("LEAVE_POWER", 0);//推出场景的时间
	}
	else
	{
		int t =( millisecondNow() - UserDefault::getInstance()->getIntegerForKey("LEAVE_TIME",0))/60000;
		log("long int:%d",t);
		int ht = UserDefault::getInstance()->getIntegerForKey("LEAVE_POWER",0)%10; //离开的时间10分钟以内
		log("int ht :%d",ht);
		if (UserDefault::getInstance()->getIntegerForKey("LEAVE_TIME",0) != 0)
		{
			int p = (t + ht) / 10;
			GAMEDATA->setPowerNum(min(GAMEDATA->getPowerNum() + p, MAXPOWER));
			refreshUI();
			UserDefault::getInstance()->setIntegerForKey("LEAVE_TIME", 0);//离开标记的时间取消
			UserDefault::getInstance()->setIntegerForKey("LEAVE_POWER", 0);	//离开之前走过的时间
			if (GAMEDATA->getPowerNum() == MAXPOWER)
			{
				UserDefault::getInstance()->setIntegerForKey("GAME_TIME", 0); //口血时间置0
				UserDefault::getInstance()->setIntegerForKey("MIN_TIME", 0);//
				UserDefault::getInstance()->setIntegerForKey("LEAVE_POWER", 0);//把游戏过去了几分钟置0
			}
		}
		if (GAMEDATA->getPowerNum() < MAXPOWER)
		{
			timeUI();
			this->scheduleUpdate();
		}
	}

	return true;
}

void LevelLayer::addEffectYun()
{
	auto yun1 = Sprite::create("effect/yun1.png");
	yun1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 300)));
	addChild(yun1, 3);
	auto yun2 = Sprite::create("effect/yun2.png");
	yun2->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0,300)));
	addChild(yun2, 3);
	auto yun3 = Sprite::create("effect/yun3.png");
	yun3->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 300)));
	addChild(yun3, 3);
	auto yun4 = Sprite::create("effect/yun4.png");
	yun4->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 300)));
	addChild(yun4, 3);

	auto moverRB = MoveTo::create(2.0, Vec2(Director::getInstance()->getVisibleSize().width, -100));
	auto moverLB = MoveTo::create(2.0, Vec2(0, 0));
	auto scaleTo = ScaleTo::create(2.0, 3.0);
	auto scaleTo2 = ScaleTo::create(2.0, 4.0);

	auto moverRT = MoveTo::create(2.0, Vec2(Director::getInstance()->getVisibleSize().width / 4, -100));
	auto scaleTo3 = ScaleTo::create(2.0, 3.0);

	auto moverLT = MoveTo::create(2.0, Vec2((2 * (Director::getInstance()->getVisibleSize().width / 4)), -100));
	auto scaleTo4 = ScaleTo::create(2.0, 3.0);

	auto spaw1 = Spawn::create(scaleTo, moverRB, nullptr);
	auto spaw2 = Spawn::create(scaleTo2, moverLB, nullptr);
	auto spaw3 = Spawn::create(scaleTo3, moverRT, nullptr);
	auto spaw4 = Spawn::create(scaleTo4, moverLT, nullptr);

	auto call1 = CallFunc::create([=](){
		//yun1->setVisible(false);
		this->removeChild(yun1);
		//log("aiyowoqu");
	});
	auto call2 = CallFunc::create([=](){
		this->removeChild(yun2);
		//log("aiyowoqu");
	});
	auto call3 = CallFunc::create([=](){
		this->removeChild(yun3);
		//log("aiyowoqu");
	});
	auto call4 = CallFunc::create([=](){
		this->removeChild(yun4);
		//log("aiyowoqu");
	});


	auto sqe1 = Sequence::create(spaw1, call1, nullptr);
	auto sqe2 = Sequence::create(spaw2, call2, nullptr);
	auto sqe3 = Sequence::create(spaw3, call3, nullptr);
	auto sqe4 = Sequence::create(spaw4, call4, nullptr);


	yun1->runAction(sqe1);
	yun2->runAction(sqe2);
	yun3->runAction(sqe3);
	yun4->runAction(sqe4);
}

void	LevelLayer::addScrollView()
{
	Texture2D* t = TextureCache::getInstance()->getTextureForKey("bg/bg_1.png");

	scrollview = ScrollView::create();
	if (scrollview)
	{
		scrollview->setAnchorPoint(Vec2(0.5, 0));
		scrollview->setContentSize(Director::getInstance()->getVisibleSize());
		scrollview->setInnerContainerSize(Size(visibleSize.width, 9*1136));
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
		//scrollview->
		//int l = visibleSize.height;
		//log("*****%.2f", (float)((10375 - 160) * 100 / (10375)));
		int index = 0;
		for (int i = 0; i < GAMEDATA->m_lock.size(); i++)
		{
			if (GAMEDATA->m_lock[i] == 1)
			{
				index = i - 1;
				break;
			}
		}
		float p = (float)((200 + 10375 - GAMEDATA->getOffsetY(index)) * 100 / (10375));
		//scrollview->jumpToPercentVertical(p);
		scrollview->scrollToPercentVertical(p, 5.0, true);
		addChild(scrollview);
		for (int i = 0; i < 9; i++)
		{
		string str = StringUtils::format("bg/levelbg/bg_00%d.png",i);
			auto bg = Sprite::create(str);
			//auto bg = Sprite::createWithTexture(t);
			if (bg)
			{
				bg->setAnchorPoint(Vec2(0.5, 0));
				bg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, scrollview, Vec2(0, i*bg->getContentSize().height)));//i*bg->getContentSize().height
				scrollview->addChild(bg);
			}
		}

			//auto light = Sprite::create("popbox/");
			//light->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, scrollview, Vec2(GAMEDATA->getOffsetX(index), GAMEDATA->getOffsetY(index))));
			//scrollview->addChild(light);
		//ParticleSystem * efft = ParticleSystemQuad::create("effect/particle_1.plist");
		////ParticleSystem* m_emitter1 = ParticleSystemQuad::create("effect/particle_1.plist");
		//efft->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, scrollview, Vec2(GAMEDATA->getOffsetX(index), GAMEDATA->getOffsetY(index))));
		//scrollview->addChild(efft);

		auto light = Sprite::create("effect/light_1.png");
		light->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom, scrollview, Vec2(GAMEDATA->getOffsetX(index), GAMEDATA->getOffsetY(index))));
		scrollview->addChild(light);
		light->setScale(0);

		auto s1 = ScaleTo::create(1.2, 1.5);
		auto s2 = ScaleTo::create(0.02, 0);

		auto out = FadeOut::create(1.5);
		auto in = FadeIn::create(0.1);
		auto spwa = Spawn::create(s1,out,nullptr);
		auto spwa2 = Spawn::create(s2, in, nullptr);

		light->runAction(RepeatForever::create(Sequence::create(spwa, spwa2, nullptr)));

		auto light2 = Sprite::create("effect/light_2.png");
		light2->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom, scrollview, Vec2(GAMEDATA->getOffsetX(index), GAMEDATA->getOffsetY(index))));
		scrollview->addChild(light2);
		light2->setScale(0);

		auto ss1 = ScaleTo::create(1.2, 1.5);
		auto ss2 = ScaleTo::create(0.02, 0);

		auto oout = FadeOut::create(1.5);
		auto iin = FadeIn::create(0.1);
		auto sspwa = Spawn::create(ss1, oout, nullptr);
		auto sspwa2 = Spawn::create(ss2, iin, nullptr);

		light2->runAction(RepeatForever::create(Sequence::create(sspwa, sspwa2, nullptr)));

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

				auto addbutton = Button::create("popbox/add_button.png");
				addbutton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, small_kuang, Vec2(-10, 0)));
				small_kuang->addChild(addbutton);
				addbutton->addClickEventListener([=](Ref*){
					if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
						AudioData::getInstance()->addButtonEffect(1);
					PowerShop* layer = PowerShop::create();
					addChild(layer);
				});

				powerNum = LabelAtlas::create(Value(GAMEDATA->getPowerNum()).asString(), "fonts/power_shopnum.png", 28, 32, '0');
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

				moneyNum = LabelAtlas::create(Value(GAMEDATA->getMoneyNum()).asString(), "fonts/power_shopnum.png", 28, 32, '0');
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

				loveNum = LabelAtlas::create(Value(GAMEDATA->getLoveNum()).asString(), "fonts/power_shopnum.png", 28, 32, '0');
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
	long int tt = t / 1000; // 秒
	
	mt = tt / 60;
	st = tt % 60;
	//log("tt:%ld   %d:%d", tt,mt,st);
	
	minutsNum = mt - UserDefault::getInstance()->getIntegerForKey("MIN_TIME",0);
	minutes = LabelAtlas::create(Value(9 - (minutsNum%10)).asString(), "fonts/power_shopnum.png", 28, 32, '0');
	minutes->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,powerNum,Vec2(0,-40)));
	minutes->setAnchorPoint(Vec2(1,0.5));
	powerNum->addChild(minutes);
	auto f = Label::create(":","fonts/Marker Felt.ttf",30);
	f->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, minutes, Vec2(5, 0)));
	f->setAnchorPoint(Vec2(0.5, 0.5));
	minutes->addChild(f);
	seconds = LabelAtlas::create(Value(59 - (st%60)).asString(), "fonts/power_shopnum.png", 28, 32, '0');
	seconds->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid , minutes,Vec2(10,0)));
	seconds->setAnchorPoint(Vec2(0, 0.5));
	minutes->addChild(seconds);

}
void  LevelLayer::setTimeUI() //
{
	long int t = millisecondNow() - UserDefault::getInstance()->getIntegerForKey("GAME_TIME"); //得到系统时间毫秒
	long int tt = t / 1000; // 秒
	mt = tt / 60;
	st = tt % 60;
	//log("tt:%ld   %d:%d", tt, mt, st);

	minutsNum = mt - UserDefault::getInstance()->getIntegerForKey("MIN_TIME", 0);
	//log("****time:%ld ,millisecondNow:%ld  minutsNum:%ld ", t, millisecondNow(), minutsNum);
	if (minutes)
	{
		minutes->setString(Value(9-(minutsNum%10)).asString());
		seconds->setString(Value(59 -(st%60)).asString());
	}
	if (minutsNum % 10 == 0 && minutsNum != 0)
	{
		GAMEDATA->setPowerNum(min(GAMEDATA->getPowerNum() + 1, MAXPOWER));
		UserDefault::getInstance()->setIntegerForKey("MIN_TIME", minutsNum);
		refreshUI(); //]
		if (GAMEDATA->getPowerNum() == MAXPOWER)
		{
			UserDefault::getInstance()->setIntegerForKey("GAME_TIME", 0);
			UserDefault::getInstance()->setIntegerForKey("MIN_TIME", 0);
			//移除时间label
			if (minutes)
			{
				minutes->removeFromParent();
				minutes = nullptr;
				//关闭定时器
				this->unscheduleUpdate();
			}
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
	gettimeofday(&now, nullptr);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}
//添加关卡
void LevelLayer::addLevelButton()
{


	for (int i = 0; i <GAMEDATA->m_lock.size(); i++)
	{
		auto testButton = Button::create("button/level_normal.png", "button/level_pass.png", "button/level_disable.png");
		testButton->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom, scrollview, Vec2(GAMEDATA->getOffsetX(i),  GAMEDATA->getOffsetY(i))));
		testButton->setScale(0.7);
		scrollview->addChild(testButton);
		testButton->setTag(i);
		auto level_num = LabelAtlas::create(Value(i +1).asString(),"fonts/power_shopnum.png",28,32,'0');
		level_num->setAnchorPoint(Vec2(0.5,0.5));
		level_num->setScale(0.7);
		level_num->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, testButton, Vec2(0,0)));
		testButton->addChild(level_num);

		if (GameData::getInstance()->m_lock[i] == 1)
		{
			testButton->setBright(false);
			testButton->setTouchEnabled(false);
		}
		//testButton->addClickEventListener(CC_CALLBACK_0(LevelLayer::test_buttonCallBack, this));
		testButton->addClickEventListener([=](Ref* sender){
			if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
				AudioData::getInstance()->addButtonEffect(1);
			GameData::getInstance()->setCurLevel(i);
			log("getCurLevel:%i", GameData::getInstance()->getCurLevel());
			//GAMEDATA->setCurLevel(GAMEDATA->getCurLevel() + 1);
			ObjectiveLayer* layer = ObjectiveLayer::create();
			addChild(layer);
		});

		//if (GameData::getInstance()->m_lock[i] == 1 && GameData::getInstance()->m_lock[i - 1] == 0)
		//{
		//	ParticleSystem * efft = ParticleSystemQuad::create("effect/particle_1.plist");
		//	//ParticleSystem* m_emitter1 = ParticleSystemQuad::create("effect/particle_1.plist");
		//	efft->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, scrollview, Vec2(GAMEDATA->getOffsetX(i - 1), GAMEDATA->getOffsetY(i - 1))));
		//	scrollview->addChild(efft);
		//}
	}

}

void LevelLayer:: back_buttonCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addButtonEffect(1);
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
	if (GAMEDATA->getPowerNum() == MAXPOWER)
	{
		UserDefault::getInstance()->setIntegerForKey("GAME_TIME", 0);//游戏开始按钮减体力的时间写入
		UserDefault::getInstance()->setIntegerForKey("MIN_TIME", 0);//
		UserDefault::getInstance()->setIntegerForKey("LEAVE_TIME", 0);//离开的时间记录
		UserDefault::getInstance()->setIntegerForKey("LEAVE_POWER", 0);//推出场景的时间
		//移除时间label
		if (minutes)
		{
			minutes->removeFromParent();
			minutes = nullptr;
			//关闭定时器
			this->unscheduleUpdate();
		}
	}
}

void LevelLayer::addCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addButtonEffect(1);
	ShopLayer* shop = ShopLayer::create();
	addChild(shop);
}

void LevelLayer::setCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addButtonEffect(1);
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