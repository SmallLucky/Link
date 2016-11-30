
#include "StopLayer.h"
#include "CommonFunction.h"
#include "TouchSwallowLayer.h"
#include "Scene/GameScene.h"
#include "Scene/LevelScene.h"

bool StopLayer::init()
{
	if (!EasePop::init())
	{
		return false;
	}
	//TouchSwallowLayer* swallLayer = TouchSwallowLayer::create();
	//addChild(swallLayer);
	addUI();
	return true;
}

void StopLayer::addUI()
{
	auto bg = Sprite::create("popbox/stop_kuang.png");
	bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,this,Vec2(0,0)));
	m_popNode->addChild(bg);

	auto hong = Sprite::create("popbox/mid_hong.png");
	hong->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, bg, Vec2(0, -30)));
	bg->addChild(hong);

	auto stopLabel = Sprite::create("popbox/stopLabel.png");
	stopLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, hong, Vec2(0, 10)));
	hong->addChild(stopLabel);

	CheckBox* musicCBox = CheckBox::create("popbox/music_on.png", "popbox/music_off.png", TextureResType::LOCAL);
	if (UserDefault::getInstance()->getBoolForKey("IS_MUSIC", true))
	{
		//创建,参数：未选中图片名，选中图片名，文字，状态（0-未选中，1-选中）
		musicCBox->setSelectedState(false);
	}
	else
	{
		musicCBox->setSelectedState(true);
	}

	musicCBox->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg, Vec2(-120, 15)));
	bg->addChild(musicCBox, 1);

	auto soundCBox = CheckBox::create("popbox/sound_on.png", "popbox/sound_off.png", TextureResType::LOCAL);//创建,参数：未选中图片名，选中图片名，文字，状态（0-未选中，1-选中）
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
	{
		soundCBox->setSelectedState(false);
	}
	else
	{
		soundCBox->setSelectedState(true);
	}
	soundCBox->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg, Vec2(0, 15)));
	bg->addChild(soundCBox);

	CheckBox * vibrationCBox = CheckBox::create("popbox/vibration_on.png", "popbox/vibration_off.png", TextureResType::LOCAL);//创建,参数：未选中图片名，选中图片名，文字，状态（0-未选中，1-选中）
	if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE", true))
	{
		log("false");
		vibrationCBox->setSelectedState(false);
	}
	else
	{
		vibrationCBox->setSelectedState(true);
	}
	vibrationCBox->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg, Vec2(120, 15)));
	bg->addChild(vibrationCBox);

	musicCBox->addEventListener(CC_CALLBACK_2(StopLayer::musicCBoxCallBack, this));
	soundCBox->addEventListener(CC_CALLBACK_2(StopLayer::soundCBoxCallBack, this));
	vibrationCBox->addEventListener(CC_CALLBACK_2(StopLayer::vibrationCBoxCallBack, this));

	auto newGame = Button::create("button/newleve_button_0.png", "button/newleve_button_1.png");
	newGame->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg, Vec2(-100, 70)));
	bg->addChild(newGame);
	newGame->addClickEventListener([=](Ref*){
		GameScene* newgame = GameScene::create();
		Director::getInstance()->replaceScene(newgame);
	});
	auto brackButton = Button::create("popbox/cancel.png");
	brackButton->addClickEventListener([=](Ref*){
		//this->removeFromParent();
		close();
	});
	brackButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, bg, Vec2(-20, -20)));
	bg->addChild(brackButton);

	auto brackGame = Button::create("button/brack_button_0.png", "button/brack_button_1.png");
	brackGame->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg, Vec2(100, 70)));
	bg->addChild(brackGame);
	brackGame->addClickEventListener(CC_CALLBACK_0(StopLayer::newGameCallBack, this));

}

void	StopLayer::musicCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type)
{
	log("music");
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("LOCAL");
		UserDefault::getInstance()->setBoolForKey("IS_MUSIC", false);
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("PLIST");
		UserDefault::getInstance()->setBoolForKey("IS_MUSIC", true);
		AudioData::getInstance()->addBgMusic(3);
		break;
	default:
		break;
	}
}

void	StopLayer::soundCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type)
{
	log("sound");
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("LOCAL");
		UserDefault::getInstance()->setBoolForKey("IS_EFFECT", false);
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("PLIST");
		UserDefault::getInstance()->setBoolForKey("IS_EFFECT", true);
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
		break;
	default:
		break;
	}
}

void	StopLayer::vibrationCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type)
{
	log("vibration");
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		log("LOCAL");
		UserDefault::getInstance()->setBoolForKey("IS_VIBRATE", false);
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		log("PLIST");
		UserDefault::getInstance()->setBoolForKey("IS_VIBRATE", true);
		break;
	default:
		break;
	}
}


void	StopLayer::newGameCallBack()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("StopLayer::newGameCallBack()");
#endif
	LevelScene* levelScene = LevelScene::create();
	Director::getInstance()->replaceScene(levelScene);  //跳转
}