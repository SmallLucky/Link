#include "InformationLayer.h" //信息层

#include "Data/Data.h"
#include "Data/RoundData.h"
#include "CommonFunction.h"

InformationLayer::InformationLayer()
{

}
InformationLayer::~InformationLayer()
{

}
bool InformationLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	REWARDDATA->praseJsonData();

	addUI();
	initCountLabel(); //初始化时间标签
	initScoreLabel(); //初始化分数标签


	initTargetScore();
	addTargetElement(); //目标元素
	return true;
	
}

void	InformationLayer::addUI()
{
	//Size s = Director::getInstance()->getVisibleSize();
	info_bg = Sprite::create("infor/Inforbg.png");
	info_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,Vec2(0,0)));
	info_bg->setAnchorPoint(Vec2(0.5,1));
	addChild(info_bg);

	count_bg = Sprite::create("infor/count_bg.png");
	count_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid,info_bg,Vec2(100,50)));
	count_bg->setAnchorPoint(Vec2(0.5,1));
	info_bg->addChild(count_bg);
	
	targetScore_bg = Sprite::create("infor/target_bg.png");
	targetScore_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,info_bg,Vec2(-100,50)));
	targetScore_bg->setAnchorPoint(Vec2(0.5, 1));
	info_bg->addChild(targetScore_bg);

	score_bg = Sprite::create("infor/score_label.png");
	score_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, info_bg, Vec2(-140, -15)));
	score_bg->setAnchorPoint(Vec2(1,0.5));
	info_bg->addChild(score_bg);

	auto countlabel = Sprite::create("infor/count_label.png");
	countlabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,count_bg,Vec2(0,0)));
	countlabel->setAnchorPoint(Vec2(0.5,0));
	count_bg->addChild(countlabel);

	auto targetlabel = Sprite::create("infor/target_label.png");
	targetlabel->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop, targetScore_bg,Vec2(0,0)));
	targetlabel->setAnchorPoint(Vec2(0.5, 0));
	targetScore_bg->addChild(targetlabel);

	auto petbg = Sprite::create("infor/pet_bg.png");
	petbg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,info_bg,Vec2(0,0)));
	info_bg->addChild(petbg);

	auto pet = Sprite::create("infor/pet_1.png");
	pet->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,petbg,Vec2(0,0)));
	petbg->addChild(pet);

	auto levelbg = Sprite::create("infor/level_bg.png");
	levelbg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,petbg,Vec2(0,25)));
	levelbg->setAnchorPoint(Vec2(0.5,1));
	petbg->addChild(levelbg,-1);

	auto levellabel = Sprite::create("infor/level_label.png");
	levellabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,levelbg,Vec2(0,-5)));
	levellabel->setAnchorPoint(Vec2(1,0.5));
	levelbg->addChild(levellabel);

	auto levelnum = LabelAtlas::create(Value(GAMEDATA->getCurLevel()+1).asString(), "fonts/game_levelnum.png",18,21,'0');
	levelnum->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,levellabel,Vec2(5,0)));
	levelnum->setAnchorPoint(Vec2(0,0.5));
	levellabel->addChild(levelnum);

}

//添加一个标签
void InformationLayer::createLabel(Label* &label, char* text, Color4B color, Point pos, float size, Point anchor /* = Point(0.5, 0.5) */)
{
	label = Label::createWithTTF(text, INFORMATION_FONT, INFORMATION_FONTSIZE); //创建标签
	label->setTextColor(color); //设置标签颜色
	label->enableShadow(Color4B::BLACK, INFORMATION_SHADOW_OFFSET); //添加字体阴影
	label->setPosition(pos); //设置标签位置
	label->setAnchorPoint(anchor); //设置标签锚点
	setSize(label, size); //设置标签尺寸
	addChild(label);
}

void  InformationLayer::initTargetScore()
{
	targatLabel = LabelAtlas::create(Value(GAMEDATA->getTargetScore(GAMEDATA->getCurLevel())).asString(), "fonts/game_targetnum.png", 26, 35, '0');
	targatLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,targetScore_bg,Vec2(0,0)));
	targatLabel->setAnchorPoint(Vec2(0.5, 0.5));
	targetScore_bg->addChild(targatLabel);
}
/*
//初始化时间标签
void InformationLayer::initTimeLabel()
{
	char text[32];
	sprintf(text, "TIME:0.0s");

	createLabel(timeLabel, text, TIME_NORMAL_COLOR, TIME_POSITION, TIME_SIZE, TIME_ANCHOR); //添加时间标签，之后通过showTime函数修改时间

}

//显示时间
void InformationLayer::showTime(float t)
{
	//if (t > LITTLE_TIME_FLAG)
	//{
	//	timeLabel->setTextColor(TIME_NORMAL_COLOR); //时间充裕时设置颜色为一般颜色
	//	
	//}
	//else if (t <= 0)
	//{
	//	t = 0; //时间用尽后显示时间为0
	//}
	//else
	//{
	//	timeLabel->setTextColor(TIME_LITTLE_COLOR); //时间紧迫时设置颜色为特殊颜色
	//
	//}

	//char text[32];
	//sprintf(text, "TIME:%.1fs", t);

	//if (timeLabel)
	//{
	//	timeLabel->setString(text); //修改时间标签的文本为对应时间
	//}
	
}
*/
//初始化得分标签
void InformationLayer::initScoreLabel()
{
	scoreLabel = LabelAtlas::create(Value(GAMEDATA->getCurSocre()).asString(), "fonts/game_scorenum.png", 17, 19, '0');
	scoreLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, score_bg, Vec2(10,0)));
	//scoreLabel->setScale(1.2);
	scoreLabel->setAnchorPoint(Vec2(0,0.5));
	score_bg->addChild(scoreLabel);
	scalecEffect(scoreLabel);
}

//显示得分
void InformationLayer::showScore(int s)
{
	scoreLabel->setString(Value(s).asString());

}

//显示得分增加
void InformationLayer::scoreUp(int delta, int time)
{
	auto addscore = LabelAtlas::create(Value(delta).asString(), "fonts/infor_num_1.png", 20, 30, '0');
	addscore->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,-30)));
	addscore->setScale(2.5);
	addChild(addscore);
	Action* a = Sequence::create(FadeOut::create(time), RemoveSelf::create(), nullptr); //设置动画，标签在一定时间内淡出，然后消失
	addscore->runAction(a);
} 

//初始化步数标签
void InformationLayer::initCountLabel()
{
	countLabel = LabelAtlas::create(Value(GAMEDATA->getCount(GAMEDATA->getCurLevel())).asString(), "fonts/game_countnum.png", 38, 48, '0');
	countLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, count_bg, Vec2(0, 0)));
	//countLabel->setScale(1.2);
	countLabel->setAnchorPoint(Vec2(0.5,0.5));
	count_bg->addChild(countLabel);
}

//显示步数
void InformationLayer::showCount(int c)
{
	if (c <= 0)
	{
		c = 0;
	}
	countLabel->setString(Value(c).asString());
	//effect
	auto effect = clone(c);
	effect->setScale(0.9f);
	if (effect && count_bg){
		effect->setAnchorPoint(Vec2(0.5, 0.5));
		count_bg->addChild(effect);
		effect->setPosition(countLabel->getPosition() + Vec2(0, 0));
		auto easeSacle = EaseExponentialOut::create(ScaleTo::create(0.30f, 1.5f));
		auto spawn = Spawn::create(easeSacle, FadeOut::create(0.30f), nullptr);
		auto calf = CallFunc::create([effect](){effect->removeFromParent(); });
		effect->runAction(Sequence::create(spawn, calf, nullptr));
	}
}

LabelAtlas* InformationLayer::clone(int c)
{
	auto effect = LabelAtlas::create(Value(c).asString(), "fonts/game_countnum.png", 38, 48, '0');
	if (effect){
		return effect;
	}
	return nullptr;
}

void	InformationLayer::addTargetElement()
{
	int index = GAMEDATA->getCurLevel();
	int offY = 0;
	if (REWARDDATA->getBlue(index) != 0)
	{
		Btaskbg = Sprite::create("infor/task_bg.png");
		Btaskbg->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom, info_bg, Vec2(150 + (offY * 100), -75)));
		info_bg->addChild(Btaskbg);

		auto Bele = Sprite::create("infor/task_ele_0.png");
		Bele->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Btaskbg, Vec2(0, 0)));
		Btaskbg->addChild(Bele);
		effectAction(Bele,2.5);
		offY++;

		targetElementBNum = LabelAtlas::create(Value(REWARDDATA->getBlue(index)).asString(), "fonts/task_number.png", 22, 26, '0');
		targetElementBNum->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Btaskbg, Vec2(0, 0)));
		targetElementBNum->setAnchorPoint(Vec2(0.5,0.5));
		Btaskbg->addChild(targetElementBNum);
	}
	if (REWARDDATA->getPurple(index) != 0)
	{
		Ptaskbg = Sprite::create("infor/task_bg.png");
		Ptaskbg->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom, info_bg, Vec2(150 + (offY * 100), -75)));
		info_bg->addChild(Ptaskbg);

		auto Pele = Sprite::create("infor/task_ele_1.png");
		Pele->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Ptaskbg, Vec2(0, 0)));
		Ptaskbg->addChild(Pele);
		effectAction(Pele, 2.5);
		offY++;

		targetElementPNum = LabelAtlas::create(Value(REWARDDATA->getPurple(index)).asString(), "fonts/task_number.png", 22, 26, '0');
		targetElementPNum->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Ptaskbg, Vec2(0, 0)));
		targetElementPNum->setAnchorPoint(Vec2(0.5, 0.5));
		Ptaskbg->addChild(targetElementPNum);
	}
	if (REWARDDATA->getGreen(index) != 0)
	{
		Gtaskbg = Sprite::create("infor/task_bg.png");
		Gtaskbg->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom, info_bg, Vec2(150 + (offY * 100), -75)));
		info_bg->addChild(Gtaskbg);

		auto Gele = Sprite::create("infor/task_ele_2.png");
		Gele->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Gtaskbg, Vec2(0, 0)));
		Gtaskbg->addChild(Gele);
		effectAction(Gele, 2.5);
		offY++;

		targetElementGNum = LabelAtlas::create(Value(REWARDDATA->getGreen(index)).asString(), "fonts/task_number.png", 22, 26, '0');
		targetElementGNum->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Gtaskbg, Vec2(0, 0)));
		targetElementGNum->setAnchorPoint(Vec2(0.5, 0.5));
		Gtaskbg->addChild(targetElementGNum);
	}
	if (REWARDDATA->getRad(index) != 0)
	{
		Rtaskbg = Sprite::create("infor/task_bg.png");
		Rtaskbg->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom, info_bg, Vec2(150 + (offY * 100), -75)));
		info_bg->addChild(Rtaskbg);

		auto Rele = Sprite::create("infor/task_ele_3.png");
		Rele->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Rtaskbg, Vec2(0, 0)));
		Rtaskbg->addChild(Rele);
		effectAction(Rele, 2.5);
		offY++;

		targetElementRNum = LabelAtlas::create(Value(REWARDDATA->getRad(index)).asString(), "fonts/task_number.png", 22, 26, '0');
		targetElementRNum->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Rtaskbg, Vec2(0, 0)));
		targetElementRNum->setAnchorPoint(Vec2(0.5, 0.5));
		Rtaskbg->addChild(targetElementRNum);
	}
	if (REWARDDATA->getYellor(index) != 0)
	{
		Ytaskbg = Sprite::create("infor/task_bg.png");
		Ytaskbg->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom, info_bg, Vec2(150 + (offY * 100), -75)));
		info_bg->addChild(Ytaskbg);

		auto Yele = Sprite::create("infor/task_ele_4.png");
		Yele->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Ytaskbg, Vec2(0, 0)));
		Ytaskbg->addChild(Yele);
		effectAction(Yele, 2.5);
		offY++;

		targetElementYNum = LabelAtlas::create(Value(REWARDDATA->getYellor(index)).asString(), "fonts/task_number.png", 22, 26, '0');
		targetElementYNum->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Ytaskbg, Vec2(0, 0)));
		targetElementYNum->setAnchorPoint(Vec2(0.5, 0.5));
		Ytaskbg->addChild(targetElementYNum);
	}
}
void	InformationLayer::showTargetElementNum(int ele, int n)
{
	/*int index = GAMEDATA->getCurLevel();*/
	switch (ele)
	{
	case 0:
		if (n > 0 )// REWARDDATA->getBlue(index)
		{
			if (targetElementBNum)
				targetElementBNum->setString(Value(n).asString());
		}
		else
		{
			if (targetElementBNum)
			{
				targetElementBNum->removeFromParent();
				targetElementBNum = nullptr;
				//targetElementBNum->setString(Value(0).asString());
				auto finish = Sprite::create("infor/task_finish.png");
				finish->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Btaskbg, Vec2(0, 0)));
				Btaskbg->addChild(finish, 1);
				finish->setScale(4.0);
				finishEffect(finish);
			}
		}
		break;
	case 1:
		if (n > 0) // REWARDDATA->getPurple(index)
		{
			if (targetElementPNum)
				targetElementPNum->setString(Value(n).asString());
		}
		else
		{
			if (targetElementPNum)
			{
				targetElementPNum->removeFromParent();
				targetElementPNum = nullptr;
				//targetElementBNum->setString(Value(0).asString());
				auto finish = Sprite::create("infor/task_finish.png");
				finish->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Ptaskbg, Vec2(0, 0)));
				Ptaskbg->addChild(finish, 1);
				finish->setScale(4.0);
				finishEffect(finish);
			}
		}
		break;
	case 2:
		if (n > 0 ) //REWARDDATA->getGreen(index)
		{
			if (targetElementGNum)
				targetElementGNum->setString(Value(n).asString());
		}
		else
		{
			if (targetElementGNum)
			{
				targetElementGNum->removeFromParent();
				//targetElementBNum->setString(Value(0).asString());
				targetElementGNum = nullptr;
				auto finish = Sprite::create("infor/task_finish.png");
				finish->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Gtaskbg, Vec2(0, 0)));
				Gtaskbg->addChild(finish, 1);
				finish->setScale(4.0);
				finishEffect(finish);
			}

		}
		break;
	case 3:
		if (n > 0 ) //REWARDDATA->getRad(index)
		{
			if (targetElementRNum)
				targetElementRNum->setString(Value(n).asString());
		}
		else
		{
			if (targetElementRNum)
			{
				targetElementRNum->removeFromParent();
				targetElementRNum = nullptr;
				//targetElementBNum->setString(Value(0).asString());
				auto finish = Sprite::create("infor/task_finish.png");
				finish->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Rtaskbg, Vec2(0, 0)));
				Rtaskbg->addChild(finish, 1);
				finish->setScale(4.0);
				finishEffect(finish);
			}
		}
		break;
	case 4:
		if (n > 0 ) //REWARDDATA->getYellor(index)
		{
			if (targetElementYNum)
				targetElementYNum->setString(Value(n).asString());
		}
		else
		{
			if (targetElementYNum)
			{
				targetElementYNum->removeFromParent();
				targetElementYNum = nullptr;
				//targetElementBNum->setString(Value(0).asString());
				auto finish = Sprite::create("infor/task_finish.png");
				finish->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, Ytaskbg, Vec2(0, 0)));
				Ytaskbg->addChild(finish, 1);
				finish->setScale(4.0);
				finishEffect(finish);
			}
		}
		break;
	default:
		break;
	}
}

void InformationLayer::effectAction(Node* node, float delay)
{
	auto f_node = dynamic_cast<Sprite*>(node);
	if (!f_node){
		return;
	}

	auto delayTime = DelayTime::create(delay);
	auto rotate_1 = RotateTo::create(0.10f, -15);
	auto rotate_2 = RotateTo::create(0.10f, 15);
	auto rotate_3 = RotateTo::create(0.10f, 0);
	auto seq = Sequence::create(delayTime, rotate_1, rotate_2, rotate_3, nullptr);
	auto repeat = RepeatForever::create(seq);
	f_node->runAction(repeat);
}

void InformationLayer::finishEffect(Node* node)
{
	auto f_node = dynamic_cast<Sprite*>(node);
	if (!f_node){
		return;
	}
	ScaleTo* scaleTo = ScaleTo::create(0.5, 1);
	auto fin = FadeTo::create(0.5, 250);
	Spawn* spawn = Spawn::create(fin, scaleTo, nullptr);
	f_node->runAction(spawn);
}

void InformationLayer::scalecEffect(Node* node)
{
	auto seq = Sequence::create(ScaleTo::create(0.7, 1.2), ScaleTo::create(0.6, 0.9), nullptr);
	node->runAction(RepeatForever::create(seq));
}