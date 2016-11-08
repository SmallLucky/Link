
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
	auto bg = Sprite::create("popbox/mid_kuang.png");
	bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,0)));
	addChild(bg);

	auto quitLabel = Sprite::create("popbox/quit_label.png");
	quitLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,bg,Vec2(0,-10)));
	bg->addChild(quitLabel);

	auto quitBG = Sprite::create("popbox/quick_cat.png");
	quitBG->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(0,30)));
	bg->addChild(quitBG);

	auto goonbut = Button::create("button/gono_game.png");
	goonbut->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg, Vec2(100, 60)));
	bg->addChild(goonbut);
	goonbut->addClickEventListener(CC_CALLBACK_0(QuitLayer::backGame, this));

	auto quitButton = Button::create("button/break_game.png");
	quitButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, bg, Vec2(-100, 60)));
	bg->addChild(quitButton);
	quitButton->addClickEventListener(CC_CALLBACK_0(QuitLayer::QuitGame, this));

	auto backButton = Button::create("popbox/cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, bg, Vec2(-20, -20)));
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