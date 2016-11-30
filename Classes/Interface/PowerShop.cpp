
#include "PowerShop.h"
#include "TouchSwallowLayer.h"
#include "CommonFunction.h"
#include "Data/GameData.h"
#include "Data/Data.h"

bool	PowerShop:: init()
{
	if (!TopDownLayer::init())
	{
		return false;
	}
	//auto touch = TouchSwallowLayer::create();
	//addChild(touch);

	addUI();

	return true;
}

void	PowerShop::addUI()
{
	auto bg = Sprite::create("popbox/power_kuang.png");
	bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, m_popNode, Vec2(0, 0)));
	m_popNode->addChild(bg);

	auto hong = Sprite::create("popbox/power_shop.png");
	hong->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, bg, Vec2(0, -25)));
	bg->addChild(hong); 

	auto backButton = Button::create("popbox/cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, bg, Vec2(-20, -20)));
	bg->addChild(backButton);
	backButton->addClickEventListener([=](Ref*){
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		{
			AudioData::getInstance()->addButtonEffect(1);
		}
		//this->removeFromParent();
		close();
	});

	auto hp_bg = Sprite::create("popbox/hp_bg.png");
	hp_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(0,130)));
	bg->addChild(hp_bg);

	auto timerbg = Sprite::create("popbox/timerbg.png");
	timerbg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, hp_bg, Vec2(0, -30)));
	hp_bg->addChild(timerbg);
	//xueliang
	float p = (GAMEDATA->getPowerNum() * 100) / MAXPOWER;
	auto timer = Sprite::create("popbox/timer.png");
	auto progressTimer = ProgressTimer::create(timer);
	progressTimer->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, timerbg, Vec2(0, 0)));
	progressTimer->setType(cocos2d::ProgressTimer::Type::BAR);    // 设置进度条为水平  
	progressTimer->setBarChangeRate(Vec2(1, 0));          // 设置进度条变化速度，只在x轴变化 
	progressTimer->setMidpoint(Vec2(0, 0.5));               // 设置起点
	progressTimer->setPercentage(p);             // 设置初始化百分比的值  
	timerbg->addChild(progressTimer, 1);

	for (int i = 0; i < 3; i++)
	{
		auto power = Sprite::create(StringUtils::format("popbox/add_%d.png",i));
		power->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(i*160 - 160,-60)));
		bg->addChild(power);

		auto shang = Sprite::create(StringUtils::format("popbox/shang_%d.png",i));
		shang->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, power, Vec2(0, 0)));
		power->addChild(shang);

		auto p_button = Button::create(StringUtils::format("button/power_%d.png",i));
		p_button->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, power, Vec2(0, -55)));
		power->addChild(p_button);
		p_button->addClickEventListener([=](Ref*){
			if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			{
				AudioData::getInstance()->addButtonEffect(1);
			}
			if (i == 0)
			{
				log("1");
				GAMEDATA->setPowerNum(MAXPOWER); //花一块钱满血
				float np = (GAMEDATA->getPowerNum() * 100) / MAXPOWER;
				auto progressTo = ProgressTo::create(1,np);
				progressTimer->runAction(progressTo);
				refreshUI();
				//发送消息到UI层
				EventCustom _event(REFRESHUI);
				_eventDispatcher->dispatchEvent(&_event);
			}
			if (i == 1)
			{
				log("12");
				GAMEDATA->setPowerNum(min(GAMEDATA->getPowerNum()+10,MAXPOWER)); //花12coins钱满血
				float np = (GAMEDATA->getPowerNum() * 100) / MAXPOWER;
				auto progressTo = ProgressTo::create(1, np);
				progressTimer->runAction(progressTo);
				refreshUI();
				//发送消息到UI层
				EventCustom _event(REFRESHUI);
				_eventDispatcher->dispatchEvent(&_event);
			}
			if (i == 2)
			{
				log("20");
				GAMEDATA->setPowerNum(min(GAMEDATA->getPowerNum() + 5, MAXPOWER)); //花20loves钱满血
				float np = (GAMEDATA->getPowerNum() * 100) / MAXPOWER;
				auto progressTo = ProgressTo::create(1, np);
				progressTimer->runAction(progressTo);
				refreshUI();
				//发送消息到UI层
				EventCustom _event(REFRESHUI);
				_eventDispatcher->dispatchEvent(&_event);
			}
		});
	}



	auto timer_zhao = Sprite::create("popbox/timer_zhao.png");
	timer_zhao->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,timerbg,Vec2(0,0)));
	timerbg->addChild(timer_zhao,2);

	auto share_power = Button::create("button/share_power.png");
	share_power->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop,timer_zhao,Vec2(10,30)));
	timer_zhao->addChild(share_power);
	share_power->addClickEventListener([=](Ref*){
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		{
			AudioData::getInstance()->addButtonEffect(1);
		}
		log("share:1"); //加分享链接
		GAMEDATA->setPowerNum(min(GAMEDATA->getPowerNum() + 1, MAXPOWER)); //分享加1 
		float np = (GAMEDATA->getPowerNum() * 100) / MAXPOWER;
		auto progressTo = ProgressTo::create(1, np);
		progressTimer->runAction(progressTo);
		refreshUI();
		//发送消息到UI层
		EventCustom _event(REFRESHUI);
		_eventDispatcher->dispatchEvent(&_event);
	});

	nowNum = LabelAtlas::create(Value(GAMEDATA->getPowerNum()).asString(),"fonts/power_shopnum.png",28,32,'0');
	nowNum->setAnchorPoint(Vec2(1,0.5));
	nowNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,timer_zhao,Vec2(0,0)));
	timer_zhao->addChild(nowNum);

	auto xie = Sprite::create("popbox/timerxie.png");
	xie->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,nowNum,Vec2(10,0)));
	xie->setAnchorPoint(Vec2(0, 0.5));
	nowNum->addChild(xie);

	auto num = LabelAtlas::create(Value(MAXPOWER).asString(), "fonts/power_shopnum.png", 28, 32, '0');
	num->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,xie,Vec2(10,0)));
	xie->addChild(num);
	num->setAnchorPoint(Vec2(0,0.5));

}

void	PowerShop::	refreshUI()
{
	nowNum->setString(Value(GAMEDATA->getPowerNum()).asString());
}