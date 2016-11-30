#include "SighedLayer.h"
#include "CommonFunction.h"
#include <iostream>
#include "TimeUtil.h"
#include "Data/Data.h"

SignedLayer::SignedLayer() :sighedOk_btn(nullptr), re_bg(nullptr)
{

}

SignedLayer::~SignedLayer()
{

}

bool SignedLayer::init()
{
	if (! TopDownLayer::init()){
		return false;
	}
	TimeUtil::init();
	addUI();
	return true;
}
void SignedLayer::addUI()
{
	auto tex = TextureCache::getInstance()->getTextureForKey("signed/reward/bg_img.png");
	if (!tex){
		return;
	}
	auto BG_kuang = Sprite::createWithTexture(tex);
	if (BG_kuang){
		m_popNode->addChild(BG_kuang, 100);
	}
	else{
		return;
	}

	auto title_img = Sprite::create("signed/reward/sigh_title.png");
	if (title_img){
		BG_kuang->addChild(title_img);
		title_img->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, BG_kuang, Vec2(0, -20)));
	}

	re_bg = Sprite::create("signed/reward/rew_img.png");
	if (re_bg){
		BG_kuang->addChild(re_bg);
		re_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, BG_kuang, Vec2(0, 12)));
	}

	auto close_btn = Button::create("popbox/cancel.png");
	close_btn->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, BG_kuang, Vec2(-20, -20)));
	BG_kuang->addChild(close_btn);
	close_btn->addClickEventListener(CC_CALLBACK_1(SignedLayer::backButCallBack, this));
	//签到按钮
	auto sighed_btn = Button::create("signed/reward/sign_btn_nor.png", "signed/reward/sign_btn_sel.png");
	if (sighed_btn){
		sighed_btn->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(0, sighed_btn->getContentSize().height)));
		BG_kuang->addChild(sighed_btn);
		sighed_btn->addClickEventListener(CC_CALLBACK_1(SignedLayer::signedButCallBack, this));
	}
	//签到完成按钮
		sighedOk_btn = TTButton::createWithImage("signed/reward/tomor_btn.png");
		if (sighedOk_btn){
			sighedOk_btn->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, BG_kuang, Vec2(-sighed_btn->getContentSize().width / 2, sighed_btn->getContentSize().height / 2)));
			BG_kuang->addChild(sighedOk_btn);
		}

		//奖励物品UI
		if (TimeUtil::isNewDay()){
			//未签到
			rewardsUI(TimeUtil::getTimeCount()+1);
			if (sighed_btn){
				sighed_btn->setVisible(true);
			}
			if (sighedOk_btn){
				sighedOk_btn->setVisible(false);
			}
		}
		else{
			//已经签到
			rewardsUI(TimeUtil::getTimeCount());
			if (sighed_btn){
				sighed_btn->setVisible(false);
			}
			if (sighedOk_btn){
				sighedOk_btn->setVisible(true);
			}
		}
}

void SignedLayer::signedButCallBack(Ref* sender)
{
	auto btn = static_cast<Button*>(sender);
	if (!btn){
		return;
	}
	//是否新的一天
	if (TimeUtil::isNewDay())
	{
		//是否签到
		if (!TimeUtil::getSignedToday())
		{
			//std::cout << "没签到..." << std::endl;
			TimeUtil::setSignedToday(true);
			TimeUtil::setFirstTime(TimeUtil::getCurTime());
			TimeUtil::addTimeCount();
			btn->setVisible(false);

			if (sighedOk_btn){
				sighedOk_btn->setVisible(true);
			}
			//todo 发放签到奖励
			switch (TimeUtil::getTimeCount()){
			case 1://bom
				GAMEDATA->setBoomb(GAMEDATA->getBoomb() + 1);
				break;
			case 2:
				GAMEDATA->setRefresh(GAMEDATA->getRefresh() + 1);
				break;
			case 3:
				GAMEDATA->setMoneyNum(GAMEDATA->getMoneyNum()+1);
				break;
			case 4:
				GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() + 1);
				break;
			case 5:
				GAMEDATA->setBoomb(GAMEDATA->getBoomb() + 1);
				GAMEDATA->setRefresh(GAMEDATA->getRefresh() + 1);
				break;
			case 6:
				GAMEDATA->setBoomb(GAMEDATA->getBoomb() + 1);
				GAMEDATA->setRefresh(GAMEDATA->getRefresh() + 1);
				GAMEDATA->setMoneyNum(GAMEDATA->getMoneyNum() + 1);
				break;
			case 7:
				GAMEDATA->setBoomb(GAMEDATA->getBoomb() + 1);
				GAMEDATA->setRefresh(GAMEDATA->getRefresh() + 1);
				GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() + 1);
				break;
			default:
				break;
			}
		}
	}else
	{
		//std::cout << "不是新的一天...,签到天数:"<< TimeUtil::getTimeCount()<< std::endl;
	}
}

void SignedLayer::backButCallBack(Ref* sender)
{
	close();
}

void SignedLayer::rewardsUI(int days)
{
	switch (days){
	case 1:
	{
			 // std::cout << "第一天" << std::endl;
			  auto light = Sprite::create("signed/reward/llight.png");
			  if (light){
				  re_bg->addChild(light);
				  light->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, re_bg, Vec2(0, -20)));
			  }

			  auto pro_1 = Sprite::create("infor/props_1.png");
			  if (pro_1){
				  pro_1->setScale(1.2f);
				  light->addChild(pro_1);
				  pro_1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light, Vec2(0,0)));
			  }

			  auto x_img = Sprite::create("signed/main/x_img.png");
			  if (x_img){ 
				  pro_1->addChild(x_img);
				  x_img->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, pro_1, Vec2(10, 23)));
			  }

			  //炸弹数量
			  auto bomb_num = LabelAtlas::create("1", "signed/main/number_1.png", 26, 32, '0');
			  if (bomb_num){
				  bomb_num->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, x_img, Vec2(6, -15)));
				  x_img->addChild(bomb_num, 10);
			  }
	}
		break;
	case 2:
		//std::cout << "第二天" << std::endl;
		{
			auto light = Sprite::create("signed/reward/llight.png");
			if (light){
				re_bg->addChild(light);
				light->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, re_bg, Vec2(0, -20)));
			}

			auto pro_1 = Sprite::create("infor/props_0.png");
			if (pro_1){
				pro_1->setScale(1.2f);
				light->addChild(pro_1);
				pro_1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light, Vec2(0, 0)));
			}

			auto x_img = Sprite::create("signed/main/x_img.png");
			if (x_img){
				pro_1->addChild(x_img);
				x_img->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, pro_1, Vec2(10, 23)));
			}

			//数量
			auto bomb_num = LabelAtlas::create("1", "signed/main/number_1.png", 26, 32, '0');
			if (bomb_num){
				bomb_num->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, x_img, Vec2(6, -15)));
				x_img->addChild(bomb_num, 10);
			}
		}
		break;
	case 3:
		//std::cout << "第三天" << std::endl;
		{
			auto light = Sprite::create("signed/reward/llight.png");
			if (light){
				re_bg->addChild(light);
				light->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, re_bg, Vec2(0, -20)));
			}

			auto pro_1 = Sprite::create("popbox/gold_200.png");
			if (pro_1){
				light->addChild(pro_1);
				pro_1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light, Vec2(0, 0)));
			}
		}
		break;
	case 4:
		//std::cout << "第四天" << std::endl;
		{
			auto light = Sprite::create("signed/reward/llight.png");
			if (light){
				re_bg->addChild(light);
				light->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, re_bg, Vec2(0, -20)));
			}

			auto pro_1 = Sprite::create("popbox/love_10.png");
			if (pro_1){
				light->addChild(pro_1);
				pro_1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light, Vec2(155, 0)));
			}
		}
		break;
	case 5:
		//std::cout << "第五天" << std::endl;
		{
			auto light_1 = Sprite::create("signed/reward/llight.png");
			if (light_1){
				re_bg->addChild(light_1);
				light_1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, re_bg, Vec2(-80, -20)));
			}

			auto light_2 = Sprite::create("signed/reward/llight.png");
			if (light_2){
				re_bg->addChild(light_2);
				light_2->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, re_bg, Vec2(80, -20)));
			}

			auto pro_1 = Sprite::create("infor/props_0.png");
			if (pro_1){
				light_1->addChild(pro_1);
				pro_1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light_1, Vec2(0, 0)));
			}

			auto pro_2 = Sprite::create("infor/props_1.png");
			if (pro_2){
				light_2->addChild(pro_2);
				pro_2->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light_2, Vec2(0, 0)));
			}
		}
		break;
	case 6:
		//std::cout << "第6天" << std::endl;
		{
			auto light_1 = Sprite::create("signed/reward/llight.png");
			if (light_1){
				re_bg->addChild(light_1);
				light_1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, re_bg, Vec2(0, -20)));
			}

			auto light_2 = Sprite::create("signed/reward/llight.png");
			if (light_2){
				re_bg->addChild(light_2);
				light_2->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, re_bg, Vec2(80, -20)));
			}

			auto light_3 = Sprite::create("signed/reward/llight.png");
			if (light_3){
				re_bg->addChild(light_3);
				light_3->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, re_bg, Vec2(-80, -20)));
			}

			auto pro_1 = Sprite::create("popbox/gold_200.png");
			if (pro_1){
				light_1->addChild(pro_1);
				pro_1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light_1, Vec2(0, 0)));
			}

			auto pro_2 = Sprite::create("infor/props_0.png");
			if (pro_2){
				light_2->addChild(pro_2);
				pro_2->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light_2, Vec2(0, 0)));
			}

			auto pro_3 = Sprite::create("infor/props_1.png");
			if (pro_3){
				light_3->addChild(pro_3);
				pro_3->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light_3, Vec2(0, 0)));
			}
		}
		break;
	case 7:
		//std::cout << "第7天" << std::endl;
		{
			auto light_1 = Sprite::create("signed/reward/llight.png");
			if (light_1){
				re_bg->addChild(light_1);
				light_1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, re_bg, Vec2(0, -20)));
			}

			auto light_2 = Sprite::create("signed/reward/llight.png");
			if (light_2){
				re_bg->addChild(light_2);
				light_2->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, re_bg, Vec2(80, -20)));
			}

			auto light_3 = Sprite::create("signed/reward/llight.png");
			if (light_3){
				re_bg->addChild(light_3);
				light_3->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, re_bg, Vec2(-80, -20)));
			}

			auto pro_1 = Sprite::create("popbox/love_10.png");
			if (pro_1){
				light_1->addChild(pro_1);
				pro_1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light_1, Vec2(155, 0)));
			}

			auto pro_2 = Sprite::create("infor/props_0.png");
			if (pro_2){
				light_2->addChild(pro_2);
				pro_2->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light_2, Vec2(0, 0)));
			}

			auto pro_3 = Sprite::create("infor/props_1.png");
			if (pro_3){
				light_3->addChild(pro_3);
				pro_3->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, light_3, Vec2(0, 0)));
			}
		}
		break;
	default:
		break;
	}
}