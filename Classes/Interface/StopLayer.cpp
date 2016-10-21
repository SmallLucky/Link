
#include "StopLayer.h"
#include "CommonFunction.h"
#include "TouchSwallowLayer.h"
#include "Scene/GameScene.h"

bool StopLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	TouchSwallowLayer* swallLayer = TouchSwallowLayer::create();
	addChild(swallLayer);
	addUI();
	return true;
}

void StopLayer::addUI()
{
	auto bg = Sprite::create("popbox/popbox_bg.png");
	bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(-10,0)));
	addChild(bg);

	auto stopLabel = Sprite::create("popbox/stop_label.png");
	stopLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(40,100)));
	bg->addChild(stopLabel);

	auto stopBG = Sprite::create("popbox/stop_bg.png");
	stopBG->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg, Vec2(30, -60)));
	bg->addChild(stopBG);

	auto gonoButton = Button::create("popbox/gono_button.png");
	gonoButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg, Vec2(-80, 120)));
	bg->addChild(gonoButton);
	gonoButton->addClickEventListener([=](Ref*){
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT"))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
		}
		this->removeFromParent();
	});

	auto newGame = Button::create("popbox/newleve_button.png");
	newGame->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,bg,Vec2(130,120)));
	bg->addChild(newGame);
	newGame->addClickEventListener(CC_CALLBACK_0(StopLayer::newGameCallBack,this));

}

void	StopLayer::newGameCallBack()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("StopLayer::newGameCallBack()");
#endif
	log("new");
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT"))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
	}
	GameScene* gameScene = GameScene::create();
	Director::getInstance()->replaceScene(gameScene);  //��ת
}