
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
	auto BG_kuang = Sprite::create("popbox/changKuang.png");
	BG_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 0)));
	addChild(BG_kuang);

	auto hong = Sprite::create("popbox/shopHong.png");
	hong->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, BG_kuang, Vec2(0, -25)));
	BG_kuang->addChild(hong);

	//props
	auto props_bg = Sprite::create("popbox/shop_propskuang.png");
	props_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,BG_kuang,Vec2(0,70)));
	BG_kuang->addChild(props_bg);

	//props_bg->setVisible(false);

	auto pat = Sprite::create("popbox/pet.png");
	pat->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,props_bg,Vec2(0,-150)));
	props_bg->addChild(pat);
	
	auto propsscrollView = ScrollView::create();
	propsscrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	propsscrollView->setAnchorPoint(Vec2(0.5, 0));
	propsscrollView->setContentSize(Size(props_bg->getContentSize().width, props_bg->getContentSize().height));
	propsscrollView->setInertiaScrollEnabled(true);
	propsscrollView->setBounceEnabled(true); //¿ªÆô»Øµ¯
	propsscrollView->setInnerContainerSize(Size(props_bg->getContentSize().width, 2 * 165));
	propsscrollView->setBackGroundColor(Color3B::GRAY);
	propsscrollView->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, props_bg, Vec2(0, 0)));
	props_bg->addChild(propsscrollView);

	auto propsscrv_width = propsscrollView->getContentSize().width;
	for (int i = 0; i <2; i++)
	{
		auto item = ShopItem::createShopItem(2,i);
		item->setTag(i);
		int item_height = item->getContentSize().height;
		propsscrollView->addChild(item);
		item->setAnchorPoint(Vec2(0.5,0.5));
		item->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, propsscrollView, Vec2(0, -100 - (i*150))));//item_height/4(0 - i * (item_height)-item_height / 2) + 4 * 90)
	}

	auto money_bg = Sprite::create("popbox/shop_moneykuang.png");
	money_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang,Vec2(0,-65)));
	BG_kuang->addChild(money_bg);
	money_bg->setVisible(false);

	auto moneyscrollView = ScrollView::create();
	moneyscrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	moneyscrollView->setAnchorPoint(Vec2(0.5, 0));
	moneyscrollView->setContentSize(Size(money_bg->getContentSize().width, money_bg->getContentSize().height-10));
	moneyscrollView->setInertiaScrollEnabled(true);
	moneyscrollView->setBounceEnabled(true);
	moneyscrollView->setInnerContainerSize(Size(money_bg->getContentSize().width, GAMEDATA->m_coins.size() * 162));
	moneyscrollView->setBackGroundColor(Color3B::BLACK);
	moneyscrollView->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, money_bg, Vec2(0, 5)));
	money_bg->addChild(moneyscrollView);

	for (int i = 0; i < GAMEDATA->m_coins.size(); i++)
	{
		auto item = ShopItem::createShopItem(1, GAMEDATA->getShopCoins(i));
		item->setTag(i);
		int item_height = item->getContentSize().height;
		item->setAnchorPoint(Vec2(0.5, 0.5));
		item->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, moneyscrollView, Vec2(0,-70 - (i*150))));//item_height/4
		moneyscrollView->addChild(item);
	}

	Button* proButton;
	Button* monButton;
	Button* gifButton;
	for (int i = 0; i < 3; i++)
	{
		auto buttonBG = Sprite::create("popbox/buttonBG.png");
		buttonBG->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, BG_kuang, Vec2(i * 150 - 150, -120)));
		BG_kuang->addChild(buttonBG);

		string str = StringUtils::format("button/shop_button%d.png", i);
		string str_1 = StringUtils::format("button/ban_%d.png", i);

		if (i == 0)
		{
			proButton = Button::create(str,str_1,str_1);
			proButton->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, buttonBG, Vec2(0, 0)));
			buttonBG->addChild(proButton, 1);
			proButton->setBright(false);
		}
		if (i == 1)
		{
			monButton = Button::create(str, str_1, str_1);
			monButton->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, buttonBG, Vec2(0, 0)));
			buttonBG->addChild(monButton, 1);
		}
		if (i == 2)
		{
			gifButton = Button::create(str, str, str);
			gifButton->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, buttonBG, Vec2(0, 0)));
			buttonBG->addChild(gifButton, 1);
		}
	}
	if (proButton)
	{
		proButton->addClickEventListener([=](Ref*){
			proButton->setBright(false);
			monButton->setBright(true);
			gifButton->setBright(true);
			props_bg->setVisible(true);
			money_bg->setVisible(false);
		});
	}
	if (monButton)
	{
		monButton->addClickEventListener([=](Ref*){
			monButton->setBright(false);
			proButton->setBright(true);
			gifButton->setBright(true);
			props_bg->setVisible(false);
			money_bg->setVisible(true);
		});
	}
	if (gifButton)
	{
		gifButton->addClickEventListener([=](Ref*){
			//gifButton->setBright(false);
			//monButton->setBright(true);
			//proButton->setBright(true);
		});
	}

	auto backButton = Button::create("popbox/cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, BG_kuang, Vec2(-20, -20)));
	BG_kuang->addChild(backButton);
	backButton->addClickEventListener(CC_CALLBACK_0(ShopLayer::backButCallBack, this));

}

void ShopLayer::backButCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
	this->removeFromParent();
}