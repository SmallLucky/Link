#include "PowerLayer.h"
#include "CommonFunction.h"
#include "Data/GameData.h"
#include "Data/Data.h"
#include "PowerShop.h"

bool PowerLayer::init()
{
	if (!EasePop::init())
	{
		return false;
	}
	addUI();
	return true;
}

void PowerLayer::addUI()
{
	auto bg = Sprite::create("popbox/mid_kuang.png");
	if (bg){
		bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, m_popNode, Vec2(0, 0)));
		m_popNode->addChild(bg);
	}
	else{
		return;
	}

	auto powerLabel = Sprite::create("popbox/tili_title.png");
	if (powerLabel){
		powerLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, bg, Vec2(0, -10)));
		bg->addChild(powerLabel);
	}

	auto powerBG = Sprite::create("popbox/tili_cat.png");
	if (powerBG){
		powerBG->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg, Vec2(0, 18)));
		bg->addChild(powerBG);
	}
	//充值
	auto charge_btn = Button::create("button/charge_nor.png", "button/charge_sel.png");
	if (charge_btn){
		charge_btn->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg, Vec2(100, 60)));
		bg->addChild(charge_btn);
		charge_btn->addClickEventListener(CC_CALLBACK_1(PowerLayer::buyCbk, this));
	}
	//拒绝
	auto refuse_btn = Button::create("button/refused_0.png", "button/refused_1.png");
	if (refuse_btn){
		refuse_btn->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg, Vec2(-100, 60)));
		bg->addChild(refuse_btn);
		refuse_btn->addClickEventListener(CC_CALLBACK_1(PowerLayer::closeCbk, this));
	}
	//关闭
	auto close_btn= Button::create("popbox/cancel.png");
	if (close_btn){
		close_btn->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, bg, Vec2(-20, -20)));
		bg->addChild(close_btn);
		close_btn->addClickEventListener(CC_CALLBACK_1(PowerLayer::closeCbk, this));
	}
}

void PowerLayer::closeCbk(Ref*sender)
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
	{
		AudioData::getInstance()->addButtonEffect(1);
	}
	close();
}

void PowerLayer::buyCbk(Ref*sender)
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
	{
		AudioData::getInstance()->addButtonEffect(1);
	}

	auto layer = PowerShop::create();
	//layer->ignoreAnchorPointForPosition(true);
	layer->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,this,Vec2(0,0)));
	//layer->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(layer,10);

}

void PowerLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)
			{
				close();
			}
		}
	}
}