
#include "ShopItem.h"
#include "CommonFunction.h"
#include "Data/Data.h"

ShopItem::ShopItem()
{

}
ShopItem::~ShopItem()
{

}
//CREATE_FUNC(ShopItem);
ShopItem* ShopItem::createShopItem(int diamNum, int giveNum, int moneyNum)
{
	ShopItem* item = new ShopItem();
	if (item && item->init(diamNum, giveNum, moneyNum))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;

}
bool ShopItem::init(int diamNum, int giveNum, int moneyNum)
{
	if (!Node::init())
	{
		return false;
	}
	auto bg_sp = Sprite::create("popbox/shop_itembg.png");
	bg_sp->setScale(0.9f);
	addChild(bg_sp);
	this->setContentSize(bg_sp->getContentSize());

	//购买按钮
	auto buy_btn = Button::create("popbox/shop_buttongold.png");
	buy_btn->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, bg_sp, Vec2(-80, 0)));
	buy_btn->addClickEventListener(CC_CALLBACK_1(ShopItem::btnClickEvent, this));
	bg_sp->addChild(buy_btn);
	buy_btn->setSwallowTouches(false);

	if (giveNum == 1) // propes
	{
		//钻石精灵
		auto diamond_sp = Sprite::create("popbox/props_1.png");
		diamond_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, bg_sp, Vec2(diamond_sp->getContentSize().width/2+15, 0)));
		bg_sp->addChild(diamond_sp);

		//钻石个数文本
		auto x = Sprite::create("popbox/X.png");
		x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,diamond_sp,Vec2(diamond_sp->getContentSize().width/2-10,0)));
		diamond_sp->addChild(x);

		auto number_diamond = LabelAtlas::create("128", "fonts/shop_num_1.png", 30, 30, '0');
		number_diamond->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,x,Vec2(5,0)));
		number_diamond->setAnchorPoint(Vec2(0,0.5));
		x->addChild(number_diamond);

		auto money = Sprite::create("popbox/small_gold.png");
		money->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid,buy_btn,Vec2(15,5)));
		money->setAnchorPoint(Vec2(0,0.5));
		buy_btn->addChild(money);

		auto money_number = LabelAtlas::create("23","fonts/shop_num_2.png",30,30,'0');
		money_number->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,money,Vec2(0,0)));
		money_number->setAnchorPoint(Vec2(0, 0.5));
		money->addChild(money_number);

	}
	if (giveNum == 2) //money
	{
		//钻石精灵
		auto gold_sp = Sprite::create("popbox/big_gold.png");
		gold_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, bg_sp, Vec2(gold_sp->getContentSize().width / 2 + 15, 0)));
		bg_sp->addChild(gold_sp);

		//钻石个数文本
		auto x = Sprite::create("popbox/X.png");
		x->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, gold_sp, Vec2(gold_sp->getContentSize().width / 2 - 10, 0)));
		gold_sp->addChild(x);

		auto number_diamond = LabelAtlas::create("128", "fonts/shop_num_1.png", 30, 30, '0');
		number_diamond->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, x, Vec2(5, 0)));
		number_diamond->setAnchorPoint(Vec2(0, 0.5));
		x->addChild(number_diamond);

		auto money = Sprite::create("popbox/m.png");
		money->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, buy_btn, Vec2(15, 5)));
		money->setAnchorPoint(Vec2(0, 0.5));
		buy_btn->addChild(money);

		auto money_number = LabelAtlas::create("23", "fonts/shop_num_2.png", 30, 30, '0');
		money_number->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, money, Vec2(0, 0)));
		money_number->setAnchorPoint(Vec2(0, 0.5));
		money->addChild(money_number);
	}
	return true;
}

void ShopItem::btnClickEvent(Ref* pSender)
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SHOP_MONEY);
	log("go mai!");
}


////赠送个数文本
//auto give_text = Text::create();
//give_text->setString(Value(giveNum).asString() + CommonFunction::WStrToUTF8(L"个"));
//give_text->setFontSize(24);
//give_text->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, bg_sp, Vec2(70, 0)));
//bg_sp->addChild(give_text);

////按钮上的文本
//auto buy_text = Text::create();
//buy_text->setString(CommonFunction::WStrToUTF8(L"兑换"));
//buy_text->setFontSize(32);
//buy_text->setColor(Color3B::BLACK);
//buy_text->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, buy_btn, Vec2(0, 0)));
//buy_btn->addChild(buy_text);

//auto money_text = Text::create();
//money_text->setString(CommonFunction::WStrToUTF8(L"￥") + Value(moneyNum).asString());
//money_text->setFontSize(32);
//money_text->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, diamond_sp, Vec2(-60, 0)));
//diamond_sp->addChild(money_text);

////打折
//auto count_text = Text::create();
////count_text->setString(Value(moneyNum).asString() + CommonFunction::WStrToUTF8(L"折"));
//count_text->setColor(Color3B::RED);
//count_text->setFontSize(24);
//count_text->setRotation(-26);
//count_text->setPosition(CommonFunction::getVisibleAchor(0, 0.5, bg_sp, Vec2(count_text->getContentSize().width / 2 + 20, 0)));
//bg_sp->addChild(count_text);

//if (moneyNum <= 8)
//{
//	count_text->setString(CommonFunction::WStrToUTF8(L"5折"));
//}
//else if (moneyNum <= 14)
//{
//	count_text->setString(CommonFunction::WStrToUTF8(L"4折"));
//}
//else if (moneyNum <= 18)
//{
//	count_text->setString(CommonFunction::WStrToUTF8(L"3折"));
//}