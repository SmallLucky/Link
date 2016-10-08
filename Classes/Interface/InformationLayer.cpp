#include "InformationLayer.h" //��Ϣ��

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
	initCountLabel(); //��ʼ��ʱ���ǩ
	initScoreLabel(); //��ʼ��������ǩ
	//initTimeLabel(); //��ʼ��ʱ���ǩ
	//initPowerLabel();//��ʼ������

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

//���һ����ǩ
void InformationLayer::createLabel(Label* &label, char* text, Color4B color, Point pos, float size, Point anchor /* = Point(0.5, 0.5) */)
{
	label = Label::createWithTTF(text, INFORMATION_FONT, INFORMATION_FONTSIZE); //������ǩ
	label->setTextColor(color); //���ñ�ǩ��ɫ
	label->enableShadow(Color4B::BLACK, INFORMATION_SHADOW_OFFSET); //���������Ӱ
	label->setPosition(pos); //���ñ�ǩλ��
	label->setAnchorPoint(anchor); //���ñ�ǩê��
	setSize(label, size); //���ñ�ǩ�ߴ�
	addChild(label);
}

void  InformationLayer::initTargetScore()
{
	targatLabel = LabelAtlas::create(Value(GAMEDATA->getTargetScore(GAMEDATA->getCurLevel())).asString(), "fonts/infor_num_1.png", 20, 30, '0');
	targatLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,targetScore_bg,Vec2(-40,-30)));
	targetScore_bg->addChild(targatLabel);
}
/*
//��ʼ��ʱ���ǩ
void InformationLayer::initTimeLabel()
{
	char text[32];
	sprintf(text, "TIME:0.0s");

	createLabel(timeLabel, text, TIME_NORMAL_COLOR, TIME_POSITION, TIME_SIZE, TIME_ANCHOR); //���ʱ���ǩ��֮��ͨ��showTime�����޸�ʱ��

}

//��ʾʱ��
void InformationLayer::showTime(float t)
{
	//if (t > LITTLE_TIME_FLAG)
	//{
	//	timeLabel->setTextColor(TIME_NORMAL_COLOR); //ʱ���ԣʱ������ɫΪһ����ɫ
	//	
	//}
	//else if (t <= 0)
	//{
	//	t = 0; //ʱ���þ�����ʾʱ��Ϊ0
	//}
	//else
	//{
	//	timeLabel->setTextColor(TIME_LITTLE_COLOR); //ʱ�����ʱ������ɫΪ������ɫ
	//
	//}

	//char text[32];
	//sprintf(text, "TIME:%.1fs", t);

	//if (timeLabel)
	//{
	//	timeLabel->setString(text); //�޸�ʱ���ǩ���ı�Ϊ��Ӧʱ��
	//}
	
}
*/
//��ʼ���÷ֱ�ǩ
void InformationLayer::initScoreLabel()
{
	scoreLabel = LabelAtlas::create(Value(GAMEDATA->getCurSocre()).asString(), "fonts/infor_num_1.png", 20, 30, '0');
	scoreLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, score_bg, Vec2(-10,-30)));
	//scoreLabel->setScale(1.2);
	score_bg->addChild(scoreLabel);
}

//��ʾ�÷�
void InformationLayer::showScore(int s)
{
	scoreLabel->setString(Value(s).asString());

}

//��ʾ�÷�����
void InformationLayer::scoreUp(int delta, int time)
{
	auto addscore = LabelAtlas::create(Value(delta).asString(), "fonts/infor_num_1.png", 20, 30, '0');
	addscore->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,-30)));
	addscore->setScale(2.5);
	addChild(addscore);

	Action* a = Sequence::create(FadeOut::create(time), RemoveSelf::create(), NULL); //���ö�������ǩ��һ��ʱ���ڵ�����Ȼ����ʧ
	addscore->runAction(a);
} 

//��ʼ��������ǩ
void InformationLayer::initCountLabel()
{
	countLabel = LabelAtlas::create(Value(GAMEDATA->getCount(GAMEDATA->getCurLevel())).asString(), "fonts/infor_num_1.png", 20, 30, '0');
	countLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, count_bg, Vec2(-25, -20)));
	countLabel->setScale(1.2);
	count_bg->addChild(countLabel);

	//char text[32];
	//sprintf(text, "COUNT:15");
	//createLabel(countLabel, text, COUNT_NORMAL_COLOR, COUNT_POSITION, COUNT_SIZE, COUNT_ANCHOR); //���ʱ���ǩ��֮��ͨ��showTime�����޸�
}

//��ʾ����
void InformationLayer::showCount(int c)
{
	if (c <= 0)
	{
		c = 0;
	}
	countLabel->setString(Value(c).asString()); //�޸�ʱ���ǩ���ı�Ϊ��Ӧ����


}

////��ʼ��������ǩ
//void InformationLayer::initPowerLabel()
//{
//	targatLabel = LabelAtlas::create(Value(GAMEDATA->getTargetScore(GAMEDATA->getCurLevel())).asString(),"",20,30,'0');
//	targatLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,targetScore_bg,Vec2(0,0)));
//	targetScore_bg->addChild(targatLabel);
//}
//
////��ʾ����
//void InformationLayer::showPower(int p)
//{
//	
//}
