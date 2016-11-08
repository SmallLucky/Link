
#include "SetLayer.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "CommonFunction.h"
#include "Data/Data.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;


bool SetLayer:: init()
{
	if (!Layer::init())
	{
		return false;
	}

	_swallowLayer = TouchSwallowLayer::create();
	addChild(_swallowLayer);
	
	addUI();
	 
	return true;
}

void	SetLayer::addUI()
{
	//auto cat = Sprite::create("popbox/cat_1.png");
	//cat->setAnchorPoint(Vec2(0.5,0.5));
	//cat->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,200)));
	//addChild(cat);

	Sprite* bg = Sprite::create("popbox/mid_kuang.png");
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,0)));
	addChild(bg);

	auto hong = Sprite::create("popbox/mid_hong.png");
	hong->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,bg,Vec2(0,-30)));
	bg->addChild(hong);

	auto setLabel = Sprite::create("popbox/setLabel.png");
	setLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, hong, Vec2(0, 10)));
	hong->addChild(setLabel);

	CheckBox* musicCBox = CheckBox::create("popbox/music_on.png", "popbox/music_off.png", TextureResType::LOCAL); 
	if (UserDefault::getInstance()->getBoolForKey("IS_MUSIC",true))
	{
		//创建,参数：未选中图片名，选中图片名，文字，状态（0-未选中，1-选中）
		log("false");
		musicCBox->setSelectedState(false);
	}
	else
	{
		log("true");
		musicCBox->setSelectedState(true);
	}
	
	musicCBox->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(-120,0)));
	bg->addChild(musicCBox, 1);

	auto musicLabel = Sprite::create("popbox/music_label.png");
	musicLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, musicCBox, Vec2(0, -30)));
	musicCBox->addChild(musicLabel);

	auto soundCBox = CheckBox::create("popbox/sound_on.png", "popbox/sound_off.png", TextureResType::LOCAL);//创建,参数：未选中图片名，选中图片名，文字，状态（0-未选中，1-选中）
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
	{
		log("false");
		soundCBox->setSelectedState(false);
	}
	else
	{
		soundCBox->setSelectedState(true);
	}
	soundCBox->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg, Vec2(0, 0)));
	bg->addChild(soundCBox);

	auto soundLabel = Sprite::create("popbox/sound_label.png");
	soundLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, soundCBox, Vec2(0, -30)));
	soundCBox->addChild(soundLabel);

	CheckBox * vibrationCBox = CheckBox::create("popbox/vibration_on.png", "popbox/vibration_off.png", TextureResType::LOCAL);//创建,参数：未选中图片名，选中图片名，文字，状态（0-未选中，1-选中）
	if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
	{
		log("false");
		vibrationCBox->setSelectedState(false);
	}
	else
	{
		vibrationCBox->setSelectedState(true);
	}
	vibrationCBox->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg, Vec2(120, 0)));
	bg->addChild(vibrationCBox);

	auto vibrationLabel = Sprite::create("popbox/vibration_label.png");
	vibrationLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, vibrationCBox, Vec2(0, -30)));
	vibrationCBox->addChild(vibrationLabel);

	musicCBox->addEventListener(CC_CALLBACK_2(SetLayer::musicCBoxCallBack, this));
	soundCBox->addEventListener(CC_CALLBACK_2(SetLayer::soundCBoxCallBack, this));
	vibrationCBox->addEventListener(CC_CALLBACK_2(SetLayer::vibrationCBoxCallBack, this));

	auto backSet = Button::create("popbox/cancel.png");
	backSet->addClickEventListener(CC_CALLBACK_0(SetLayer::backButtonCallBack, this));
	backSet->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, bg, Vec2(-20, -20)));
	bg->addChild(backSet);
}

void	SetLayer::musicCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type)
{
	log("music");
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("LOCAL");
		UserDefault::getInstance()->setBoolForKey("IS_MUSIC",false);
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("PLIST");
		UserDefault::getInstance()->setBoolForKey("IS_MUSIC", true);
		SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM, true);
		break;
	default:
		break;
	}
}

void	SetLayer::soundCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type)
{
	log("sound");
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("LOCAL");
		UserDefault::getInstance()->setBoolForKey("IS_EFFECT",false);
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("PLIST");
		UserDefault::getInstance()->setBoolForKey("IS_EFFECT",true);
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
		break;
	default:
		break;
	}
}

void	SetLayer::vibrationCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type)
{
	log("vibration");
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("LOCAL");
		UserDefault::getInstance()->setBoolForKey("IS_VIBRATE",false);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("PLIST");
		UserDefault::getInstance()->setBoolForKey("IS_VIBRATE", true);
		break;
	default:
		break;
	}
}

void	SetLayer::backButtonCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT"))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
	}
	this->removeFromParent();
}

