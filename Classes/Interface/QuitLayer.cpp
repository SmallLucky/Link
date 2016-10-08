
#include "QuitLayer.h"
#include "CommonFunction.h"
#include "Data/GameData.h"

bool QuitLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto touchSwallow = TouchSwallowLayer::create();
	addChild(touchSwallow);

	addUI();
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(QuitLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
	return true;
}

void QuitLayer::addUI()
{
	auto bg = Sprite::create("popbox/popbox_bg.png");
	bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(-40,10)));
	addChild(bg);

	auto quitLabel = Sprite::create("popbox/quit_label.png");
	quitLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(25,90)));
	bg->addChild(quitLabel);
	auto quitBG = Sprite::create("popbox/quit_bg.png");
	quitBG->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(20,-40)));
	bg->addChild(quitBG);

	for (int i = 0; i < 3; i++)
	{
		auto star = Sprite::create("popbox/big_hart.png");
		star->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, quitBG, Vec2(i * 120 - 120, 10)));
		quitBG->addChild(star);
	}

	auto lostLabel = Sprite::create("popbox/losthart_label.png");
	lostLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,quitBG,Vec2(0,20)));
	quitBG->addChild(lostLabel);

	auto quitButton = Button::create("popbox/quit_button.png");
	quitButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg, Vec2(-80, 120)));
	bg->addChild(quitButton);
	quitButton->addClickEventListener(CC_CALLBACK_0(QuitLayer::QuitGame, this));

	auto backButton = Button::create("popbox/leave_button.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg, Vec2(120, 120)));
	bg->addChild(backButton);
	backButton->addClickEventListener(CC_CALLBACK_0(QuitLayer::backGame, this));
}

void QuitLayer::QuitGame()
{
	log("quit");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	GAMEDATA->setLoveNum(3);
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void QuitLayer::backGame()
{
	log("leave");
	this->removeFromParent();
}

void QuitLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //·µ»Ø
			{
				this->removeFromParent();
			}
		}
	}
}