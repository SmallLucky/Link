#include "GiftLayer.h"
#include "CommonFunction.h"
#include <iostream>

GiftLayer::GiftLayer()
{

}

GiftLayer::~GiftLayer()
{

}

bool GiftLayer::init()
{
	if (! TopDownLayer::init()){
		return false;
	}

	addUI();

	return true;
}
void GiftLayer::addUI()
{
	auto tex = TextureCache::getInstance()->getTextureForKey("signed/main/kuang_img.png");
	if (!tex){
		return;
	}
	auto BG_kuang = Sprite::createWithTexture(tex);
	if (BG_kuang){
		m_popNode->addChild(BG_kuang, 100);
	}else{
		return;
	}

	auto close_btn = Button::create("popbox/cancel.png");
	if (close_btn){
		close_btn->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, BG_kuang, Vec2(-20, -20)));
		BG_kuang->addChild(close_btn);
		close_btn->addClickEventListener(CC_CALLBACK_1(GiftLayer::backButCallBack, this));
	}

	auto title_img = Sprite::create("signed/main/haohua_img.png");
	if (title_img){
		BG_kuang->addChild(title_img);
		title_img->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, BG_kuang, Vec2(0, -20)));
	}

	auto light_sp = Sprite::create("signed/main/light_5.png");
	if (light_sp){
		BG_kuang->addChild(light_sp);
		light_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(0, 50)));
	}

	auto scrollView = ScrollView::create();
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setAnchorPoint(Vec2(0.5, 0));
	scrollView->setContentSize(Size(BG_kuang->getContentSize().width, BG_kuang->getContentSize().height-160));
	scrollView->setInertiaScrollEnabled(true);
	scrollView->setBounceEnabled(true);
	//scrollView->setInnerContainerSize(Size(BG_kuang->getContentSize().width, 3 * 160));
	scrollView->setBackGroundColor(Color3B::GRAY);
	scrollView->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(0, 45)));
	scrollView->setScrollBarOpacity(0);
	//scrollView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//scrollView->setBackGroundColorOpacity(150);
	BG_kuang->addChild(scrollView);

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
}

void GiftLayer::backButCallBack(Ref* sender)
{
	close();
}

//-----------------GiftItem-------------------//

GiftItem::GiftItem()
{

}

GiftItem::~GiftItem()
{

}

GiftItem*GiftItem::createItem(int changeNum, int bombNum, float moneyNum)
{
	GiftItem* item = new GiftItem();
	if (item && item->init(changeNum, bombNum, moneyNum))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

bool GiftItem::init(int changeNum, int bombNum, float moneyNum)
{
	if (!Node::init()){
		return false;
	}

	addUI(changeNum, bombNum, moneyNum);
	return true;
}

void GiftItem::addUI(int changeNum, int bombNum, float moneyNum)
{
	Sprite* bg_sp = nullptr;
	if (changeNum == 10){
		bg_sp = Sprite::create("signed/reward/diban_img.png");
		if (bg_sp){
			auto sel_img = Sprite::create("signed/reward/sale_img.png");
			if (sel_img){
				bg_sp->addChild(sel_img,10);
				float width = sel_img->getContentSize().width;
				sel_img->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftTop, bg_sp, Vec2(width / 2, -width / 2 + 20)));
			}
		}
	}
	else{
		bg_sp = Sprite::create("popbox/shop_itembg.png");
	}

	if (!bg_sp){
		return;
	}

	addChild(bg_sp);
	this->setContentSize(bg_sp->getContentSize());

	auto change_sp = Sprite::create("signed/main/shopprops_0.png");
	if (!change_sp){
		return;
	}
	bg_sp->addChild(change_sp);
	change_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, bg_sp, Vec2(100, 6)));

	auto add_sp = Sprite::create("signed/reward/add_btn.png");
	if (add_sp && change_sp){
		change_sp->addChild(add_sp);
		add_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, change_sp, Vec2(20, 0)));
	}

	auto x_img = Sprite::create("signed/main/x_img.png");
	if (x_img){
		change_sp->addChild(x_img);
		x_img->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, change_sp, Vec2(-20, 23)));
	}

	//换色数量
	auto chan_num = LabelAtlas::create(Value(changeNum).asString(), "signed/main/number_1.png", 26, 32, '0');
	if (chan_num){
		chan_num->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, x_img, Vec2(5, -15)));
		x_img->addChild(chan_num, 10);
	}


	auto bomb_sp = Sprite::create("signed/main/shopprops_1.png");
	if (bomb_sp){
		bg_sp->addChild(bomb_sp);
		bomb_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg_sp, Vec2(0, 6)));
	}

	auto x_img_1 = Sprite::create("signed/main/x_img.png");
	if (x_img_1){
		bomb_sp->addChild(x_img_1);
		x_img_1->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, bomb_sp, Vec2(-20, 23)));
	}

	//炸弹数量
	auto bomb_num = LabelAtlas::create(Value(bombNum).asString(), "signed/main/number_1.png", 26, 32, '0');
	if (bomb_num){
		bomb_num->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, x_img_1, Vec2(5, -15)));
		//chan_num->setScale(0.6);
		x_img_1->addChild(bomb_num, 10);
	}

	auto buy_btn = Button::create("button/shop_by0.png","button/shop_by1.png");
	if (buy_btn){
		buy_btn->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, bg_sp, Vec2(-100, 0)));
		bg_sp->addChild(buy_btn);
		buy_btn->setSwallowTouches(false);
		buy_btn->addClickEventListener(CC_CALLBACK_1(GiftItem::btnClickEvent, this));
	}

	auto rmb_sp = Sprite::create("signed/main/rmb_img.png");
	if (rmb_sp){
		buy_btn->addChild(rmb_sp);
		rmb_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,buy_btn,Vec2(-30,5)));
	}

	auto noney_num = LabelAtlas::create(Value((int)moneyNum).asString(), "fonts/shop_num_2.png", 30, 30, '0');
	if (noney_num){
		noney_num->setScale(0.8f);
		noney_num->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, buy_btn, Vec2(-20, -8)));
		buy_btn->addChild(noney_num, 10);
	}

	if (!noney_num){
		return;
	}

	auto dian = Sprite::create("signed/main/point.png");
	dian->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, noney_num, Vec2(0, 0)));
	noney_num->addChild(dian);

	int m = (int)moneyNum;
	if (m == 8)
	{
		auto _num = LabelAtlas::create("8", "fonts/shop_num_2.png", 30, 30, '0');
		
		if (_num){
			_num->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, dian, Vec2(0, 0)));
			dian->addChild(_num, 10);
		}
	}
	else if (m == 6){
		auto _num = LabelAtlas::create("0", "fonts/shop_num_2.png", 30, 30, '0');
		if (_num){
			_num->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, dian, Vec2(0, 0)));
			dian->addChild(_num, 10);
		}
	}
	else if (m == 18){
		auto _num = LabelAtlas::create("8", "fonts/shop_num_2.png", 30, 30, '0');
		if (_num){
			_num->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, dian, Vec2(0, 0)));
			dian->addChild(_num, 10);
		}
	}


}

void GiftItem::btnClickEvent(Ref* pSender)
{
	auto btn = dynamic_cast<Button*>(pSender);
	if (!btn){
		return;
	}

	int tag = this->getTag();
	switch (tag){
	case 0:
		std::cout << "购买18.0元" << std::endl;
		break;
	case 1:
		std::cout << "购买6.0元" << std::endl;
		break;
	case 2:
		std::cout << "购买8.8元" << std::endl;
		break;
	default:
		break;
	}
}