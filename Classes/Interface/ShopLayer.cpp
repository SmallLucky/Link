
#include "ShopLayer.h"
#include "CommonFunction.h"
#include "TouchSwallowLayer.h"
#include "Unit/ShopItem.h"
#include "CommonFunction.h"
#include "Data/Data.h"
bool ShopLayer:: init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto touchSwallow = TouchSwallowLayer::create();
	addChild(touchSwallow);

	addUI();
	return true;
}

void ShopLayer::addUI()
{
	auto cat = Sprite::create("popbox/cat_4.png");
	cat->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 330)));
	addChild(cat);
	auto BG_kuang = Sprite::create("popbox/popbox_big_bg.png");
	BG_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, cat, Vec2(-30, -310)));
	cat->addChild(BG_kuang);

	auto shopBGleft = Sprite::create("popbox/shop_bg_left.png");
	shopBGleft->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,BG_kuang,Vec2(30,-80)));
	BG_kuang->addChild(shopBGleft);

	auto shopBGRight = Sprite::create("popbox/shop_bg_right.png");
	shopBGRight->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(30, -80)));
	BG_kuang->addChild(shopBGRight);
	shopBGRight->setVisible(false);

	auto propsscrollView = ScrollView::create();
	propsscrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	propsscrollView->setAnchorPoint(Vec2(0.5, 0.5));
	propsscrollView->setContentSize(Size(shopBGleft->getContentSize().width, shopBGleft->getContentSize().height - 100));
	propsscrollView->setInertiaScrollEnabled(true);
	propsscrollView->setBounceEnabled(true);
	propsscrollView->setInnerContainerSize(Size(shopBGleft->getContentSize().width, 8 * 99));
	propsscrollView->setBackGroundColor(Color3B::GRAY);
	propsscrollView->setPosition(CommonFunction::getVisibleAchor(Center, shopBGleft, Vec2(0, -20)));
	shopBGleft->addChild(propsscrollView);
	auto propsscrv_width = propsscrollView->getContentSize().width;
	for (int i = 0; i <8; i++)
	{
		auto item = ShopItem::createShopItem((i + 3),1, (i + 3));
		item->setTag(i);
		int item_height = item->getContentSize().height;
		propsscrollView->addChild(item);
		item->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, propsscrollView, Vec2(0, (0 - i * (item_height)-item_height / 2) + 4 * 90)));//item_height/4
	}

	auto moneyscrollView = ScrollView::create();
	moneyscrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	moneyscrollView->setAnchorPoint(Vec2(0.5, 0.5));
	moneyscrollView->setContentSize(Size(shopBGRight->getContentSize().width, shopBGRight->getContentSize().height - 100));
	moneyscrollView->setInertiaScrollEnabled(true);
	moneyscrollView->setBounceEnabled(true);
	moneyscrollView->setInnerContainerSize(Size(shopBGRight->getContentSize().width, 8 * 99));
	//moneyscrollView->setBackGroundColor(Color3B::BLACK);
	moneyscrollView->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, shopBGRight, Vec2(0, -20)));
	//auto test = Sprite::create("popbox/big_gold.png");
	//test->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, moneyscrollView, Vec2(0, 4*99)));
	//moneyscrollView->addChild(test);
	shopBGRight->addChild(moneyscrollView);
	for (int i = 0; i <8; i++)
	{
		auto item = ShopItem::createShopItem((i + 4), 2, (i + 4));
		item->setTag(i);
		int item_height = item->getContentSize().height;
		moneyscrollView->addChild(item);
		item->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, moneyscrollView, Vec2(0, (0 - i * (item_height)-item_height / 2) + 4*90)));//item_height/4
	}



	auto propsButton = Button::create("popbox/shopprops_button.png");
	propsButton->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(-80, 160)));
	BG_kuang->addChild(propsButton);
	propsButton->addClickEventListener([=](Ref*){
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
		shopBGRight->setVisible(false);
		shopBGleft->setVisible(true);
	});

	auto moneyButton = Button::create("popbox/shopmoney_button.png");
	moneyButton->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(30, 160)));
	BG_kuang->addChild(moneyButton);
	moneyButton->addClickEventListener([=](Ref*){
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
		shopBGleft->setVisible(false);
		shopBGRight->setVisible(true);
	});

	auto backButton = Button::create("popbox/button_cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(30, -10)));
	BG_kuang->addChild(backButton);
	backButton->addClickEventListener(CC_CALLBACK_0(ShopLayer::backButCallBack, this));

}

void ShopLayer::backButCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
	this->removeFromParent();
}