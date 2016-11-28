
#include "AddCount.h"
#include "TouchSwallowLayer.h"
#include "CommonFunction.h"

bool AddCount::init()
{
	if (!TopDownLayer::init())
	{
		return false;
	}
	//auto touchlayer = TouchSwallowLayer::create();
	//addChild(touchlayer);

	addUI();
	return true;
}
void AddCount::addUI()
{
	auto bg = Sprite::create("infor/nocount_bg.png");
	bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, m_popNode,Vec2(0, 0)));
	m_popNode->addChild(bg);
	
	auto nocountlabel = Sprite::create("infor/nocount_label.png");
	nocountlabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,bg,Vec2(50,-20)));
	bg->addChild(nocountlabel);

	auto breakButton = Button::create("popbox/cancel.png");
	breakButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop,bg,Vec2(-80,-20)));
	breakButton->addClickEventListener([=](Ref*){
		EventCustom _event(ISGAMEOVERTRUE);
		_eventDispatcher->dispatchEvent(&_event);
		this->removeFromParent();
	});
	bg->addChild(breakButton);

	auto addCountBut = Button::create("button/addCountBut.png");
	addCountBut->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(60,-40)));
	bg->addChild(addCountBut);

	addCountBut->addClickEventListener(CC_CALLBACK_0(AddCount::addCountButton, this));
}

void	AddCount::addCountButton()
{
	EventCustom _event(ISGAMEOVERFALSE);
	_eventDispatcher->dispatchEvent(&_event);
	this->removeFromParent();
}

