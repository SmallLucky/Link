
#include "SupplementsLayer.h"

#include "CommonFunction.h"
#include "ShopLayer.h"

SupplementsLayer::SupplementsLayer()
{

}
SupplementsLayer::~SupplementsLayer()
{

}
//CREATE_FUNC(ShopItem);
SupplementsLayer* SupplementsLayer::createInit(int type)
{
	SupplementsLayer* item = new SupplementsLayer();
	if (item && item->init(type))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

bool SupplementsLayer::init(int type)
{
	if (!DownTopLayer::init())
	{
		return false;
	}
	AddUI(type);

	return true;
}

void	SupplementsLayer::AddUI(int type)
{
	auto bg = Sprite::create("popbox/mid_kuang.png");
	bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,m_popNode,Vec2(0,0)));
	m_popNode->addChild(bg);

	auto breakButton = Button::create("popbox/cancel.png");
	breakButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop,bg,Vec2(-20,-20)));
	bg->addChild(breakButton);
	breakButton->addClickEventListener(CC_CALLBACK_0(SupplementsLayer::breakCallBack, this));

	auto heng = Sprite::create("popbox/heng.png");
	heng->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,bg,Vec2(0,-15)));
	bg->addChild(heng);

	auto cat = Sprite::create("infor/cat_supplements.png");
	cat->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(0,20)));
	bg->addChild(cat);

	auto guang = Sprite::create("infor/guang.png");
	guang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, cat, Vec2(90, 25)));
	cat->addChild(guang);

	auto spp = Sprite::create(StringUtils::format("infor/props_%d.png", type));
	spp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,guang,Vec2(0,0)));
	spp->setScale(0.7);
	guang->addChild(spp);

	auto refusedBut = Button::create("button/refused_0.png", "button/refused_1.png");
	if (refusedBut)
	{
		refusedBut->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom, bg, Vec2(130, 65)));
		bg->addChild(refusedBut, 1);
		refusedBut->addClickEventListener(CC_CALLBACK_0(SupplementsLayer::breakCallBack, this));
	}
	auto supplementsBut = Button::create("button/supplements_0.png", "button/supplements_1.png");
	supplementsBut->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom,bg,Vec2(-130,65)));
	bg->addChild(supplementsBut);
	supplementsBut->addClickEventListener(CC_CALLBACK_0(SupplementsLayer::supplementsCallBack,this));
}

void	SupplementsLayer::supplementsCallBack()
{
	auto layer = ShopLayer::create();
	layer->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,this,Vec2(0,0)));
	this->addChild(layer,10);
}

void	SupplementsLayer::breakCallBack()
{
	close();
}