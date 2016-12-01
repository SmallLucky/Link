
#include "ShopLayer.h"
#include "CommonFunction.h"
#include "TouchSwallowLayer.h"
#include "Unit/ShopItem.h"
#include "CommonFunction.h"
#include "Data/Data.h"
#include "GiftLayer.h"

ShopLayer::ShopLayer()
{
	auto _listenerIcons = EventListenerCustom::create(BYICONS, [=](EventCustom*event){
		monButton->setBright(false);
		proButton->setBright(true);
		gifButton->setBright(true);
		props_bg->setVisible(false);
		money_bg->setVisible(true);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerIcons, 1);
	auto _listenerLoves = EventListenerCustom::create(BYLOVES, [=](EventCustom*event){
		monButton->setBright(false);
		proButton->setBright(true);
		gifButton->setBright(true);
		props_bg->setVisible(false);
		money_bg->setVisible(true);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerLoves, 1);
}
ShopLayer::~ShopLayer()
{
	_eventDispatcher->removeCustomEventListeners(BYICONS);
	_eventDispatcher->removeCustomEventListeners(BYLOVES);
}

bool ShopLayer:: init()
{
	if (!TopDownLayer::init())
	{
		return false;
	}

	addUI();
	return true;
}

void ShopLayer::addUI()
{
	auto BG_kuang = Sprite::create("popbox/changKuang.png");
	BG_kuang->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, m_popNode, Vec2(0, 0)));
	m_popNode->addChild(BG_kuang);

	auto hong = Sprite::create("popbox/shopHong.png");
	hong->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, BG_kuang, Vec2(0, -25)));
	BG_kuang->addChild(hong);

	//props
	props_bg = Sprite::create("popbox/gift_kuang.png");
	props_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,BG_kuang,Vec2(0,20)));
	BG_kuang->addChild(props_bg);

	//props_bg->setVisible(false);

	auto pat = Sprite::create("popbox/pet.png");
	pat->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,props_bg,Vec2(0,-110)));
	props_bg->addChild(pat);

	auto scrollView = ScrollView::create();
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setAnchorPoint(Vec2(0.5, 0));
	scrollView->setContentSize(Size(props_bg->getContentSize().width, props_bg->getContentSize().height - 10));
	scrollView->setInertiaScrollEnabled(true);
	scrollView->setBounceEnabled(true);
	//scrollView->setInnerContainerSize(Size(BG_kuang->getContentSize().width, 3 * 160));
	scrollView->setBackGroundColor(Color3B::GRAY);
	scrollView->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, props_bg, Vec2(0, 10)));
	scrollView->setScrollBarOpacity(0);
	//scrollView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//scrollView->setBackGroundColorOpacity(150);
	props_bg->addChild(scrollView);

	auto light_sp = Sprite::create("signed/main/light_5.png");
	if (light_sp){
		scrollView->addChild(light_sp,-1);
		light_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, scrollView, Vec2(0, 50)));
	}
	auto scrv_width = scrollView->getContentSize().width;

	for (int i = 0; i < 3; i++)
	{
		int item_height = 0;
		if (i == 0){
			auto item = GiftItem::createItem(20, 20, 18.8);
			if (item){
				item->setTag(i);
				item_height = item->getContentSize().height;
				scrollView->addChild(item);
				item->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, scrollView, Vec2(0, i * (item_height - 14) - 160)));
			}
		}
		else if (i == 1){
			auto item = GiftItem::createItem(6, 6, 6.0);
			if (item){
				item->setTag(i);
				item_height = item->getContentSize().height;
				scrollView->addChild(item);
				item->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, scrollView, Vec2(0, i * (item_height - 14) - 160)));
			}
		}
		else if (i == 2){
			auto item = GiftItem::createItem(10, 10, 8.8);
			if (item){
				item->setTag(i);
				item_height = item->getContentSize().height;
				scrollView->addChild(item);
				item->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, scrollView, Vec2(0, i * (item_height - 14) - 160)));
			}
		}
	}

	money_bg = Sprite::create("popbox/shop_moneykuang.png");
	money_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang,Vec2(0,-65)));
	BG_kuang->addChild(money_bg);
	money_bg->setVisible(false);

	auto moneyscrollView = ScrollView::create();
	moneyscrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	moneyscrollView->setAnchorPoint(Vec2(0.5, 0));
	moneyscrollView->setContentSize(Size(money_bg->getContentSize().width, money_bg->getContentSize().height-10));
	moneyscrollView->setInertiaScrollEnabled(true);
	moneyscrollView->setBounceEnabled(true);
	moneyscrollView->setInnerContainerSize(Size(money_bg->getContentSize().width, GAMEDATA->m_coins.size() * 160));
	moneyscrollView->setBackGroundColor(Color3B::BLACK);
	moneyscrollView->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, money_bg, Vec2(0, 5)));
	moneyscrollView->setScrollBarOpacity(0); //设置滚动条透明度
	money_bg->addChild(moneyscrollView);

	for (int i = 0; i < GAMEDATA->m_coins.size(); i++)
	{
		auto item = ShopItem::createShopItem(1, GAMEDATA->getCoinsMoney(i), GAMEDATA->getCoinsMoneyJiao(i),GAMEDATA->getShopCoins(i));
		item->setTag(i);
		int item_height = item->getContentSize().height;
		item->setAnchorPoint(Vec2(0.5,1));
		item->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, moneyscrollView, Vec2(0, - (i*150)+80)));//item_height/4
		moneyscrollView->addChild(item);
	}

	love_bg = Sprite::create("popbox/shop_moneykuang.png");
	love_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(0, -65)));
	BG_kuang->addChild(love_bg);
	love_bg->setVisible(false);

	auto lovescrollView = ScrollView::create();
	lovescrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	lovescrollView->setAnchorPoint(Vec2(0.5, 0));
	lovescrollView->setContentSize(Size(love_bg->getContentSize().width, love_bg->getContentSize().height - 10));
	lovescrollView->setInertiaScrollEnabled(true);
	lovescrollView->setBounceEnabled(true);
	lovescrollView->setInnerContainerSize(Size(love_bg->getContentSize().width, GAMEDATA->m_coins.size() * 160));
	lovescrollView->setBackGroundColor(Color3B::BLACK);
	lovescrollView->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, love_bg, Vec2(0, 5)));
	lovescrollView->setScrollBarOpacity(0); //设置滚动条透明度
	love_bg->addChild(lovescrollView);

	for (int i = 0; i < GAMEDATA->m_coins.size(); i++)
	{
		auto item = ShopItem::createShopItem(3, GAMEDATA->getLovesMoney(i), GAMEDATA->getLovesMoneyJiao(i), GAMEDATA->getLovesNum(i));
		item->setTag(i);
		int item_height = item->getContentSize().height;
		item->setAnchorPoint(Vec2(0.5, 1));
		item->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, lovescrollView, Vec2(0, -(i * 150) + 80)));//item_height/4
		lovescrollView->addChild(item);
	}

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
			gifButton = Button::create(str, str_1, str_1);
			gifButton->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, buttonBG, Vec2(0, 0)));
			buttonBG->addChild(gifButton, 1);
		}
	}
	if (proButton)
	{
		proButton->addClickEventListener([=](Ref*){
			if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			{
				AudioData::getInstance()->addButtonEffect(2);
			}
			proButton->setBright(false);
			monButton->setBright(true);
			gifButton->setBright(true);

			props_bg->setVisible(true);
			money_bg->setVisible(false);
			love_bg->setVisible(false);
		});
	}
	if (monButton)
	{
		monButton->addClickEventListener([=](Ref*){
			if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			{
				AudioData::getInstance()->addButtonEffect(2);
			}
			monButton->setBright(false);
			proButton->setBright(true);
			gifButton->setBright(true);

			props_bg->setVisible(false);
			money_bg->setVisible(true);
			love_bg->setVisible(false);
		});
	}
	if (gifButton)
	{
		gifButton->addClickEventListener([=](Ref*){
			if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			{
				AudioData::getInstance()->addButtonEffect(2);
			}
			gifButton->setBright(false);
			monButton->setBright(true);
			proButton->setBright(true);

			props_bg->setVisible(false);
			money_bg->setVisible(false);
			love_bg->setVisible(true);
		});
	}

	auto backButton = Button::create("popbox/cancel.png");
	backButton->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, BG_kuang, Vec2(-20, -20)));
	BG_kuang->addChild(backButton);
	backButton->addClickEventListener(CC_CALLBACK_0(ShopLayer::backButCallBack, this));

}

void ShopLayer::backButCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
	{
		AudioData::getInstance()->addButtonEffect(1);
	}
	/*this->removeFromParent();*/
	close();
}