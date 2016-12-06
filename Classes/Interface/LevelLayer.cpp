
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
#include "GiftLayer.h"
#include "SighedLayer.h"
#include "PowerLayer.h"

LevelLayer::LevelLayer() :mt(0),minutes(nullptr)
{
	auto _listenerReresh = EventListenerCustom::create(REFRESHUI, [=](EventCustom*event){
		refreshUI();
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerReresh, 1);
	auto _listenerPower = EventListenerCustom::create(ADDPOWER, [=](EventCustom*event){
		auto layer = PowerLayer::create();
		addChild(layer);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerPower, 1);
}
LevelLayer::~LevelLayer()
{
	_eventDispatcher->removeCustomEventListeners(REFRESHUI);
	_eventDispatcher->removeCustomEventListeners(ADDPOWER);

	if (GAMEDATA->getPowerNum() == MAXPOWER)
	{
		UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERTIME", 0); //满点体力下消耗体力时的毫秒时间
		UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERDAY", CommonFunction::getCurDay()); //满点体力下消耗体力的第几天
		//移除时间label
		if (minutes)
		{
			minutes->removeFromParent();
			minutes = nullptr;
			//关闭定时器
			this->unscheduleUpdate();
		}
	}
	else
	{

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
	addTimeLabel();

	return true;
}

void  LevelLayer::addTimeLabel()
{

	if (GAMEDATA->getPowerNum() == MAXPOWER)
	{
		UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERTIME",0); //满点体力下消耗体力时的毫秒时间
		UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERDAY", CommonFunction::getCurDay()); //满点体力下消耗体力的第几天
		//移除时间label
		if (minutes)
		{
			minutes->removeFromParent();
			minutes = nullptr;
			//关闭定时器
			this->unscheduleUpdate();
		}
	}
	else
	{
			int t;
			int day = CommonFunction::getCurDay() - UserDefault::getInstance()->getIntegerForKey("CONSUMPTION_POWERDAY"); //
			if (day > 0) //判断是不是同一天 86400000
			{
				long int td = day * 86400000 - UserDefault::getInstance()->getIntegerForKey("CONSUMPTION_POWERTIME") + millisecondNow(); //相隔的天数*一天的毫秒数-走过的毫秒数加上今天的毫秒得到
				t = td / 60000; // 得到时间差 分钟
				log("day:%d",t);
			}
			else
			{
				t = (millisecondNow() - UserDefault::getInstance()->getIntegerForKey("CONSUMPTION_POWERTIME")) / 60000; //同一天，现在的毫秒减去记录的毫秒得到时间差 分钟
				log("long int:%d", t); //距离扣体力的分钟
			}
			int p = t / 10;
			if (p > 0 ) //表示时间差大于十分钟，需要修改体力
			{
				GAMEDATA->setPowerNum(min(GAMEDATA->getPowerNum() + p, MAXPOWER));
				refreshUI();
				log("addpower:%d",p);
				if (GAMEDATA->getPowerNum()  == MAXPOWER) //如果加完体力满点，那么
				{
					UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERTIME", 0); //满点体力下消耗体力时的毫秒时间
					UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERDAY", CommonFunction::getCurDay()); //满点体力下消耗体力的第几天
					//移除时间label
					if (minutes)
					{
						minutes->removeFromParent();
						minutes = nullptr;
						//关闭定时器
						this->unscheduleUpdate();
					}
				}
				else
				{
					timeUI();
					this->scheduleUpdate();
				}
			}
			else //没有超过十分钟 ，需要添加倒数时间label
			{
				timeUI();
				this->scheduleUpdate();
			}
	}
}

void  LevelLayer::timeUI() //
{
	long int t;
	int day = CommonFunction::getCurDay() - UserDefault::getInstance()->getIntegerForKey("CONSUMPTION_POWERDAY"); //
	if (day > 0) //判断是不是同一天 86400000
	{
		long int td = day * 86400000 - UserDefault::getInstance()->getIntegerForKey("CONSUMPTION_POWERTIME") + millisecondNow(); //相隔的天数*一天的毫秒数-走过的毫秒数加上今天的毫秒得到
		t = td / 1000; // 得到时间差 秒
	}
	else
	{
		t = (millisecondNow() - UserDefault::getInstance()->getIntegerForKey("CONSUMPTION_POWERTIME")) / 1000; //同一天，现在的毫秒减去记录的毫秒得到时间差 秒
		log("long int:%d", t); //距离扣体力的分钟
	}
	int p = t / 600; //过去几个十分钟
	if (p > 0 && p != 0) //表示时间差大于十分钟，需要修改体力
	{
		mt = (t - (p * 600))/60; //label需要创建的分钟
		st = t % 60; //label需要创建的秒
	}
	else //没有超过十分钟 ，需要添加倒数时间label
	{
		mt = t / 60; //label需要创建的分钟
		st = t % 60; //label需要创建的秒
	}

	minutes = LabelAtlas::create(Value(9 - (mt % 10)).asString(), "fonts/power_shopnum.png", 28, 32, '0');
	minutes->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, powerNum, Vec2(0, -40)));
	minutes->setAnchorPoint(Vec2(1, 0.5));
	powerNum->addChild(minutes);

	auto f = Label::create(":", "fonts/Marker Felt.ttf", 30);
	f->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, minutes, Vec2(5, 0)));
	f->setAnchorPoint(Vec2(0.5, 0.5));
	minutes->addChild(f);

	seconds = LabelAtlas::create(Value(59 - (st % 60)).asString(), "fonts/power_shopnum.png", 28, 32, '0');
	seconds->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, minutes, Vec2(10, 0)));
	seconds->setAnchorPoint(Vec2(0, 0.5));
	minutes->addChild(seconds);

}

void  LevelLayer::setTimeUI() //
{
	long int t;
	int day = CommonFunction::getCurDay() - UserDefault::getInstance()->getIntegerForKey("CONSUMPTION_POWERDAY"); //
	if (day > 0) //判断是不是同一天 86400000
	{
		long int td = day * 86400000 - UserDefault::getInstance()->getIntegerForKey("CONSUMPTION_POWERTIME") + millisecondNow(); //相隔的天数*一天的毫秒数-走过的毫秒数加上今天的毫秒得到
		t = td / 1000; // 得到时间差 秒
	}
	else
	{
		t = (millisecondNow() - UserDefault::getInstance()->getIntegerForKey("CONSUMPTION_POWERTIME")) / 1000; //同一天，现在的毫秒减去记录的毫秒得到时间差 秒
		log("long int:%d", t); //距离扣体力的分钟
	}
	int p = t / 600; //过去几个十分钟
	if (p > 0 && p != 0) //表示时间差大于十分钟，需要修改体力
	{
		mt = (t - (p * 600)) / 60; //label需要创建的分钟
		st = t % 60; //label需要创建的秒
	}
	else //没有超过十分钟 ，需要添加倒数时间label
	{
		mt = t / 60; //label需要创建的分钟
		st = t % 60; //label需要创建的秒
	}

	if (minutes)
	{
		minutes->setString(Value(9 - (mt % 10)).asString());
		seconds->setString(Value(59 - (st % 60)).asString());
		if ((9 - (mt % 10)) == 0  && (59 - (st % 60) == 0))
		{
			cout << "时间满十分钟体力加一" << endl;
			GAMEDATA->setPowerNum(min(GAMEDATA->getPowerNum() + 1, MAXPOWER));
			refreshUI();
			UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERTIME", millisecondNow());
			if (GAMEDATA->getPowerNum() == MAXPOWER)
			{
				UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERTIME", 0); //满点体力下消耗体力时的毫秒时间
				UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERDAY", CommonFunction::getCurDay()); //满点体力下消耗体力的第几天
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

void LevelLayer::addEffectYun()
{
	auto yun1 = Sprite::create("effect/yun1.png");
	yun1->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, Vec2(-300, 50)));
	addChild(yun1, 3);
	auto yun2 = Sprite::create("effect/yun2.png");
	yun2->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, Vec2(-150, 50)));
	addChild(yun2, 3);
	auto yun3 = Sprite::create("effect/yun3.png");
	yun3->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, Vec2(0, 50)));
	addChild(yun3, 3);
	auto yun4 = Sprite::create("effect/yun4.png");
	yun4->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, Vec2(450, 50)));
	addChild(yun4, 3);

	auto moverRB = MoveTo::create(1.0, Vec2(Director::getInstance()->getVisibleSize().width, -100));
	auto moverLB = MoveTo::create(1.0, Vec2(0, 0));
	auto scaleTo = ScaleTo::create(1.0, 3.0);
	auto scaleTo2 = ScaleTo::create(1.0, 4.0);

	auto moverRT = MoveTo::create(1.0, Vec2(Director::getInstance()->getVisibleSize().width / 4, -100));
	auto scaleTo3 = ScaleTo::create(1.0, 3.0);

	auto moverLT = MoveTo::create(1.0, Vec2((2 * (Director::getInstance()->getVisibleSize().width / 4)), -100));
	auto scaleTo4 = ScaleTo::create(1.0, 3.0);

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
		scrollview->setInnerContainerSize(Size(visibleSize.width, 18*1136));
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
		scrollview->scrollToPercentVertical(p, 2.0, true);
		addChild(scrollview);
		for (int j = 0; j < 18; j++)
		{
			cout << "滚动背景求余**" << ":%d" << j % 9 << endl;
		}
		for (int i = 0; i < 18; i++)
		{
			string str = StringUtils::format("bg/levelbg/bg_00%d.png", i%9);
			auto tex = TextureCache::getInstance()->getTextureForKey(str);
			if (tex){
				auto bg = Sprite::createWithTexture(tex);
				if (bg)
				{
					//log("create bg..%d",i);
					bg->setAnchorPoint(Vec2(0.5, 0));
					bg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, scrollview, Vec2(0, i*bg->getContentSize().height)));//i*bg->getContentSize().height
					scrollview->addChild(bg);
				}
			}
		}

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
		Sprite* small_kuang;
			if (i == 0) //label_short
			{
				small_kuang = Sprite::create("popbox/label_short.png");
				small_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, Vec2((i * 200 - 200), -50)));
				addChild(small_kuang);
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
					powerNum->setScale(0.8);
					small_kuang->addChild(powerNum);
				}
			}
			if (i == 1)
			{
				small_kuang = Sprite::create("popbox/labelkuang_w.png");
				small_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, Vec2((i * 200 - 210), -50)));
				addChild(small_kuang);
				auto money = Sprite::create("popbox/big_gold.png");
				money->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, small_kuang, Vec2(20, 0)));
				small_kuang->addChild(money);
				auto addbutton = Button::create("popbox/add_button.png");
				addbutton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, small_kuang, Vec2(-10, 0)));
				small_kuang->addChild(addbutton);
				addbutton->addClickEventListener(CC_CALLBACK_0(LevelLayer::addCallBack, this));

				moneyNum = LabelAtlas::create(Value(GAMEDATA->getMoneyNum()).asString(), "fonts/power_shopnum.png", 28, 32, '0');
				moneyNum->setAnchorPoint(Vec2(0.5, 0.5));
				moneyNum->setScale(0.8);
				moneyNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, small_kuang, Vec2(10, 0)));
				small_kuang->addChild(moneyNum);
			}
			if (i == 2)
			{
				small_kuang = Sprite::create("popbox/labelkuang_w.png");
				small_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, Vec2((i * 200 - 200), -50)));
				addChild(small_kuang);
				auto love = Sprite::create("popbox/big_hart.png");
				love->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, small_kuang, Vec2(20, 0)));
				small_kuang->addChild(love);
				auto addbutton = Button::create("popbox/add_button.png");
				addbutton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, small_kuang, Vec2(-10, 0)));
				small_kuang->addChild(addbutton);
				addbutton->addClickEventListener(CC_CALLBACK_0(LevelLayer::addCallBack, this));

				loveNum = LabelAtlas::create(Value(GAMEDATA->getLoveNum()).asString(), "fonts/power_shopnum.png", 28, 32, '0');
				loveNum->setAnchorPoint(Vec2(0.5, 0.5));
				loveNum->setScale(0.8);
				loveNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, small_kuang, Vec2(10, 0)));
				small_kuang->addChild(loveNum);

			}
	}
	auto shopButton = Button::create("button/shop_nor.png", "button/shop_sel.png");
	shopButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom,Vec2(-100,100)));
	shopButton->setScale(0.6);
	addChild(shopButton);
	shopButton->addClickEventListener(CC_CALLBACK_0(LevelLayer::addCallBack,this));

	auto setButton = Button::create("button/set_nor.png", "button/set_sel.png");
	setButton->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom,Vec2(100,100)));
	setButton->setScale(0.6);
	addChild(setButton);
	setButton->addClickEventListener(CC_CALLBACK_0(LevelLayer::setCallBack, this));

	//豪华礼包，签到奖励
	auto giftsBtn = Button::create("signed/main/haohua_nor.png", "signed/main/haohua_sel.png");
	giftsBtn->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, Vec2(-giftsBtn->getContentSize().width / 2 - 15, 280)));
	addChild(giftsBtn);
	giftsBtn->addClickEventListener(CC_CALLBACK_1(LevelLayer::giftedCallBack, this));

	auto signendBtn = Button::create("signed/main/qiandao_nor.png", "signed/main/qiandao_sel.png");
	signendBtn->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, Vec2(-signendBtn->getContentSize().width / 2 - 15, 130)));
	addChild(signendBtn);
	signendBtn->addClickEventListener(CC_CALLBACK_1(LevelLayer::signedCallBack, this));
}

void LevelLayer::signedCallBack(Ref* sender)
{
	auto giftLayer = SignedLayer::create();
	if (giftLayer){
		addChild(giftLayer);
	}
}

void LevelLayer::giftedCallBack(Ref* sender)
{
	auto giftLayer = GiftLayer::create();
	if (giftLayer){
		addChild(giftLayer);
	}
}


//添加关卡
void LevelLayer::addLevelButton()
{


	for (int i = 0; i <GAMEDATA->m_lock.size(); i++)
	{
		Button* testButton;
		if ((i+1) % 10 == 0 && i != 0)
		{
			testButton = Button::create("button/level_normal_10.png", "button/level_pass_10.png", "button/level_disable.png");
			
			
			if (GameData::getInstance()->m_lock[i] == 0)
			{
				auto light1 = Sprite::create("infor/guang.png");
				light1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, testButton, Vec2(-5, 10)));
				light1->setScale(1.5);
				testButton->addChild(light1, -1);
				rotateAction(light1);
			}
		}
		else
		{
			testButton = Button::create("button/level_normal.png", "button/level_pass.png", "button/level_disable.png");
		}
		testButton->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom, scrollview, Vec2(GAMEDATA->getOffsetX(i),  GAMEDATA->getOffsetY(i))));
		testButton->setScale(0.75);
		scrollview->addChild(testButton);
		testButton->setTag(i);
		auto level_num = LabelAtlas::create(Value(i +1).asString(),"fonts/power_shopnum.png",28,32,'0');
		level_num->setAnchorPoint(Vec2(0.5,0.5));
		level_num->setScale(0.7);
		level_num->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, testButton, Vec2(0,10)));
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
		UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERTIME", 0); //满点体力下消耗体力时的毫秒时间
		UserDefault::getInstance()->setIntegerForKey("CONSUMPTION_POWERDAY", CommonFunction::getCurDay()); //满点体力下消耗体力的第几天
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

void LevelLayer::tanLevelEffect(Node* node, float time)
{
	auto f_node = dynamic_cast<Sprite*>(node);
	if (!f_node){
		return;
	}
	auto delay = DelayTime::create(time);
	auto call = CallFunc::create([=]{
		node->setVisible(false);
	});
	auto fun = CallFunc::create([=]{
		node->setVisible(true);
	});
	auto seq = Sequence::create(delay,call,delay,fun,nullptr);
	node->runAction(RepeatForever::create(seq));
}

void LevelLayer::rotateAction(Node* node)
{
	if (!node){
		return;
	}

	auto r_node = dynamic_cast<Sprite*>(node);
	auto rotate_ac = RotateBy::create(20.0f, 360.0);
	if (r_node){
		r_node->runAction(RepeatForever::create(rotate_ac));
	}
}