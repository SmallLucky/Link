
#include "SetLayer.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "CommonFunction.h"


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
	auto cat = Sprite::create("popbox/cat_1.png");
	cat->setAnchorPoint(Vec2(0.5,0.5));
	cat->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,200)));
	addChild(cat);

	Sprite* bg = Sprite::create("popbox/popbox_bg.png");
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	bg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,cat,Vec2(-40,-40)));
	cat->addChild(bg);

	auto setLabel = Sprite::create("popbox/set_label.png");
	setLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(40,100)));
	bg->addChild(setLabel);
	
	auto setBG = Sprite::create("popbox/set_bg.png");
	setBG->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg, Vec2(30,-50)));
	bg->addChild(setBG);

	auto musicCBox = CheckBox::create("popbox/music_on.png", "popbox/sound_off.png", TextureResType::LOCAL);//创建,参数：未选中图片名，选中图片名，文字，状态（0-未选中，1-选中）
	musicCBox->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,setBG,Vec2(-150,0)));
	setBG->addChild(musicCBox,1);

	auto musicLabel = Sprite::create("popbox/music_label.png");
	musicLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, musicCBox, Vec2(0, -45)));
	musicCBox->addChild(musicLabel);

	auto soundCBox = CheckBox::create("popbox/sound_on.png", "popbox/sound_off.png", TextureResType::LOCAL);//创建,参数：未选中图片名，选中图片名，文字，状态（0-未选中，1-选中）
	soundCBox->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, setBG, Vec2(0, 0)));
	setBG->addChild(soundCBox);

	auto soundLabel = Sprite::create("popbox/sound_label.png");
	soundLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, soundCBox, Vec2(0, -45)));
	soundCBox->addChild(soundLabel);

	CheckBox * vibrationCBox = CheckBox::create("popbox/vibration_on.png", "popbox/vibration_off.png", TextureResType::LOCAL);//创建,参数：未选中图片名，选中图片名，文字，状态（0-未选中，1-选中）
	vibrationCBox->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, setBG, Vec2(150, 0)));
	setBG->addChild(vibrationCBox);

	auto vibrationLabel = Sprite::create("popbox/vibration_label.png");
	vibrationLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, vibrationCBox, Vec2(0, -45)));
	vibrationCBox->addChild(vibrationLabel);

	musicCBox->addEventListener(CC_CALLBACK_2(SetLayer::musicCBoxCallBack, this));
	soundCBox->addEventListener(CC_CALLBACK_2(SetLayer::soundCBoxCallBack, this));
	vibrationCBox->addEventListener(CC_CALLBACK_2(SetLayer::vibrationCBoxCallBack, this));

	auto backSet = Button::create("popbox/button_cancel.png");
	backSet->addClickEventListener(CC_CALLBACK_0(SetLayer::backButtonCallBack, this));
	backSet->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg, Vec2(25, -20)));
	bg->addChild(backSet);
}

void	SetLayer::musicCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type)
{
	log("music");
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("LOCAL");
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("PLIST");
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
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("PLIST");
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
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("PLIST");
		break;
	default:
		break;
	}
}

void	SetLayer::backButtonCallBack()
{
	this->removeFromParent();
}

