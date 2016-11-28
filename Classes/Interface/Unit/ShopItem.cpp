
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
ShopItem* ShopItem::createShopItem(int type, int moneyNum, int num)
{
	ShopItem* item = new ShopItem();
	if (item && item->init(type, moneyNum,num))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;

}
bool ShopItem::init(int type, int moneyNum,int num)
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
			string str = StringUtils::format("popbox/gold_%d.png", num);
			auto coins = Sprite::create(str);
			coins->setAnchorPoint(Vec2(0, 0.5));
			coins->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, bg_sp, Vec2(25, -8)));
			bg_sp->addChild(coins);

			auto moneynum = Sprite::create(StringUtils::format("popbox/icons_x%d.png", num));
			moneynum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg_sp, Vec2(0, 0)));
			bg_sp->addChild(moneynum);
			
			auto shop_button = Button::create("button/shop_by0.png", "button/shop_by1.png", "button/shop_by1.png");
			if (shop_button)
			{
				shop_button->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, bg_sp, Vec2(-80, 0)));
				bg_sp->addChild(shop_button, 1);
				shop_button->addClickEventListener([=](Ref*){
					if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
						AudioData::getInstance()->addButtonEffect(3);
					log("go mai %d", moneyNum);
				});

				auto symbol = Sprite::create("popbox/m.png");
				symbol->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, shop_button, Vec2(30, 5)));
				shop_button->addChild(symbol);

				auto money = LabelAtlas::create(Value(moneyNum).asString(), "fonts/game_propsnum.png", 20, 24, '0');
				money->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, symbol, Vec2(25, 0)));
				money->setAnchorPoint(Vec2(0, 0.5));
				money->setScale(0.6);
				symbol->addChild(money);

				auto dian = Sprite::create("popbox/dian.png");
				dian->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, money, Vec2(0, 0)));//money->getContentSize().width
				money->addChild(dian);

			}
		}
		if (type == 2)
		{
			auto props = Sprite::create(StringUtils::format("popbox/shopprops_%d.png", num));
			props->setAnchorPoint(Vec2(0,0.5));
			props->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid,bg_sp,Vec2(30,0)));
			bg_sp->addChild(props);

			auto pro_num0 = Button::create("button/shop_props0.png","button/props_shop0.png");
			pro_num0->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg_sp,Vec2(60,0)));
			bg_sp->addChild(pro_num0);
			pro_num0->addClickEventListener([=](Ref*){
				/*addRefreshPropsCallBarck();*/
				if (num == 0)
				{
					addRefreshPropsCallBarck(3);
				}
				if (num == 1)
				{
					addBoomPropsCallBarck(3);
				}
				
			});

			auto pro_num1 = Button::create("button/shop_props1.png","button/props_shop1.png");
			pro_num1->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,bg_sp,Vec2(-80,0)));
			bg_sp->addChild(pro_num1);
			pro_num1->addClickEventListener([=](Ref*){
				if (num == 0)
				{
					addRefreshPropsCallBarck(12);
				}
				if (num == 1)
				{
					addBoomPropsCallBarck(12);
				}
			});
		}
		if (type == 3)
		{
			string str = StringUtils::format("popbox/love_%d.png", num);
			auto loves = Sprite::create(str);
			loves->setAnchorPoint(Vec2(0.5, 0.5));
			loves->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg_sp, Vec2(0, 8)));
			bg_sp->addChild(loves);

			auto moneynum = Sprite::create(StringUtils::format("popbox/love_x%d.png", num));
			moneynum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg_sp, Vec2(0, 0)));
			bg_sp->addChild(moneynum);

			auto shop_button = Button::create("button/shop_by0.png","button/shop_by1.png" , "button/shop_by1.png");
			if (shop_button)
			{
				shop_button->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, bg_sp, Vec2(-80, 0)));
				bg_sp->addChild(shop_button, 1);
				shop_button->addClickEventListener([=](Ref*){
					if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
						AudioData::getInstance()->addButtonEffect(3);
					log("go mai %d", moneyNum);
				});

				auto symbol = Sprite::create("popbox/m.png");
				symbol->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid,shop_button,Vec2(30,5)));
				shop_button->addChild(symbol);

				auto money = LabelAtlas::create(Value(moneyNum).asString(), "fonts/game_propsnum.png", 20, 24, '0');
				money->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid,symbol,Vec2(25,0)));
				money->setAnchorPoint(Vec2(0,0.5));
				money->setScale(0.6);
				symbol->addChild(money);

				auto dian = Sprite::create("popbox/dian.png");
				dian->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, money, Vec2(0, 0)));//money->getContentSize().width
				money->addChild(dian);

			}

		}
	}
	return true;
}

void ShopItem::addRefreshPropsCallBarck(int num) //道具按钮旁边添加道具回调方法
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addButtonEffect(3);
	//用金币购买jia爱心
	bool isBy = false;
	if (GAMEDATA->getLoveNum() >= (num*2))
	{
		GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() - (num*2)); //爱心大于等于2
		isBy = true;
	}
	else
	{
		isBy = false; //数量不够，弹出购买弹框
		cout << "爱心不够" << endl;
		//发送消息到UI层
		EventCustom _event(BYLOVES);
		_eventDispatcher->dispatchEvent(&_event);
	}
	if (GAMEDATA->getMoneyNum() >= (num*60))
	{
		if (isBy)
		{
			GAMEDATA->setMoneyNum(GAMEDATA->getMoneyNum() - (num*60)); //爱心扣成功，扣除金币
			GAMEDATA->setRefresh(GAMEDATA->getRefresh() + num); //两者都扣除后，添加道具
		}
	}
	else
	{
		isBy = false;
		cout << "金币不够" << endl;
		//发送消息到UI层
		EventCustom _event(BYICONS);
		_eventDispatcher->dispatchEvent(&_event);
	} 
}
void ShopItem::addBoomPropsCallBarck(int num)
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addButtonEffect(3);
	//用金币购买jia爱心
	bool isBy = false;
	if (GAMEDATA->getLoveNum() >= (num * 2))
	{
		GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() - (num * 2)); //爱心大于等于2
		isBy = true;
	}
	else
	{
		isBy = false; //数量不够，弹出购买弹框
		cout << "爱心不够" << endl;
		EventCustom _event(BYLOVES);
		_eventDispatcher->dispatchEvent(&_event);
	}
	if (GAMEDATA->getMoneyNum() >= (num * 60))
	{
		if (isBy)
		{
			GAMEDATA->setMoneyNum(GAMEDATA->getMoneyNum() - (num * 60)); //爱心扣成功，扣除金币
			GAMEDATA->setBoomb(GAMEDATA->getBoomb() + num); //两者都扣除后，添加道具
		}
	}
	else
	{
		isBy = false;
		cout << "金币不够" << endl;
		EventCustom _event(BYICONS);
		_eventDispatcher->dispatchEvent(&_event);
	}
}


void ShopItem::btnClickEvent(Ref* pSender)
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addButtonEffect(3);
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