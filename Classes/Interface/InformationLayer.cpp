#include "InformationLayer.h" //信息层

#include "Data/Data.h"
#include "Data/RoundData.h"
#include "CommonFunction.h"

bool InformationLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	addUI();
	initCountLabel(); //初始化时间标签
	initScoreLabel(); //初始化分数标签
	//initTimeLabel(); //初始化时间标签
	//initPowerLabel();//初始化体力

	initTargetScore();
	return true;
	
}

void	InformationLayer::addUI()
{
	Size s = Director::getInstance()->getVisibleSize();
	info_bg = Sprite::create("infor/infor_bg.png");
	info_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidTop,Vec2(0,-120)));
	addChild(info_bg);

	count_bg = Sprite::create("infor/count_bg.png");
	count_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid,info_bg,Vec2(120,0)));
	info_bg->addChild(count_bg);
	
	targetScore_bg = Sprite::create("infor/target_bg.png");
	targetScore_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,info_bg,Vec2(0,35)));
	info_bg->addChild(targetScore_bg);

	score_bg = Sprite::create("infor/score_bg.png");
	score_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,info_bg,Vec2(0,-40)));
	info_bg->addChild(score_bg);

	auto dog = Sprite::create("infor/dog.png");
	dog->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid,info_bg,Vec2(-100,-20)));
	info_bg->addChild(dog);

	auto sugar = Sprite::create("infor/sugar.png");
	sugar->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid,info_bg,Vec2(100,-50)));
	info_bg->addChild(sugar);
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
	targatLabel = LabelAtlas::create(Value(GAMEDATA->getTargetScore(GAMEDATA->getCurLevel())).asString(), "fonts/infor_num_1.png", 20, 30, '0');
	targatLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,targetScore_bg,Vec2(-40,-30)));
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
	scoreLabel = LabelAtlas::create(Value(GAMEDATA->getCurSocre()).asString(), "fonts/infor_num_1.png", 20, 30, '0');
	scoreLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, score_bg, Vec2(-10,-30)));
	//scoreLabel->setScale(1.2);
	score_bg->addChild(scoreLabel);
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

	Action* a = Sequence::create(FadeOut::create(time), RemoveSelf::create(), NULL); //设置动画，标签在一定时间内淡出，然后消失
	addscore->runAction(a);
} 

//初始化步数标签
void InformationLayer::initCountLabel()
{
	countLabel = LabelAtlas::create(Value(GAMEDATA->getCount(GAMEDATA->getCurLevel())).asString(), "fonts/infor_num_1.png", 20, 30, '0');
	countLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, count_bg, Vec2(-25, -20)));
	countLabel->setScale(1.2);
	count_bg->addChild(countLabel);

	//char text[32];
	//sprintf(text, "COUNT:15");
	//createLabel(countLabel, text, COUNT_NORMAL_COLOR, COUNT_POSITION, COUNT_SIZE, COUNT_ANCHOR); //添加时间标签，之后通过showTime函数修改
}

//显示步数
void InformationLayer::showCount(int c)
{
	if (c <= 0)
	{
		c = 0;
	}
	countLabel->setString(Value(c).asString()); //修改时间标签的文本为对应步数


}

////初始化体力标签
//void InformationLayer::initPowerLabel()
//{
//	targatLabel = LabelAtlas::create(Value(GAMEDATA->getTargetScore(GAMEDATA->getCurLevel())).asString(),"",20,30,'0');
//	targatLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,targetScore_bg,Vec2(0,0)));
//	targetScore_bg->addChild(targatLabel);
//}
//
////显示体力
//void InformationLayer::showPower(int p)
//{
//	
//}
