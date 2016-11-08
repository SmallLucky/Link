
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
ShopItem* ShopItem::createShopItem(int type, int moneyNum)
{
	ShopItem* item = new ShopItem();
	if (item && item->init(type, moneyNum))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;

}
bool ShopItem::init(int type, int moneyNum)
{
	if (!Node::init())
	{
		return false;
	}
	auto bg_sp = Sprite::create("popbox/shop_itembg.png");
	addChild(bg_sp);
	//this->setContentSize(bg_sp->getContentSize());

	if (bg_sp)
	{
		if (type == 1) // propes
		{
			string str = StringUtils::format("popbox/gold_%d.png", moneyNum);
			auto coins = Sprite::create(str);
			coins->setAnchorPoint(Vec2(0, 0.5));
			coins->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, bg_sp, Vec2(25, -8)));
			bg_sp->addChild(coins);

			auto moneynum = Sprite::create(StringUtils::format("popbox/x%d.png",moneyNum));
			moneynum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg_sp, Vec2(0, 0)));
			bg_sp->addChild(moneynum);
			
			auto shop_button = Button::create("button/shop_money.png");
			shop_button->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,bg_sp,Vec2(-100,0)));
			bg_sp->addChild(shop_button);
			shop_button->addClickEventListener([=](Ref*){
				if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SHOP_MONEY);
				log("go mai %d",moneyNum);
			});
		}
		if (type == 2)
		{
			auto props = Sprite::create(StringUtils::format("popbox/shopprops_%d.png",moneyNum));
			props->setAnchorPoint(Vec2(0,0.5));
			props->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid,bg_sp,Vec2(30,0)));
			bg_sp->addChild(props);

			auto pro_num0 = Button::create("button/shop_props0.png");
			pro_num0->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg_sp,Vec2(60,0)));
			bg_sp->addChild(pro_num0);
			pro_num0->addClickEventListener([=](Ref*){
				log("props:%d",3);
			});

			auto pro_num1 = Button::create("button/shop_props1.png");
			pro_num1->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,bg_sp,Vec2(-80,0)));
			bg_sp->addChild(pro_num1);
			pro_num1->addClickEventListener([=](Ref*){
				log("props1:%d", 6);
			});
		}
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