#include "InformationLayer.h" //��Ϣ��

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
	initCountLabel(); //��ʼ��ʱ���ǩ
	initScoreLabel(); //��ʼ��������ǩ


	initTargetScore();
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
	targatLabel = LabelAtlas::create(Value(GAMEDATA->getTargetScore(GAMEDATA->getCurLevel())).asString(), "fonts/game_targetnum.png", 26, 35, '0');
	targatLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,targetScore_bg,Vec2(0,0)));
	targatLabel->setAnchorPoint(Vec2(0.5, 0.5));
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
	scoreLabel = LabelAtlas::create(Value(GAMEDATA->getCurSocre()).asString(), "fonts/game_scorenum.png", 17, 19, '0');
	scoreLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::RightMid, score_bg, Vec2(10,0)));
	//scoreLabel->setScale(1.2);
	scoreLabel->setAnchorPoint(Vec2(0,0.5));
	score_bg->addChild(scoreLabel);
	scalecEffect(scoreLabel);
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
	Action* a = Sequence::create(FadeOut::create(time), RemoveSelf::create(), nullptr); //���ö�������ǩ��һ��ʱ���ڵ�����Ȼ����ʧ
	addscore->runAction(a);
} 

//��ʼ��������ǩ
void InformationLayer::initCountLabel()
{
	countLabel = LabelAtlas::create(Value(GAMEDATA->getCount(GAMEDATA->getCurLevel())).asString(), "fonts/game_countnum.png", 38, 48, '0');
	countLabel->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, count_bg, Vec2(0, 0)));
	//countLabel->setScale(1.2);
	countLabel->setAnchorPoint(Vec2(0.5,0.5));
	count_bg->addChild(countLabel);
}

//��ʾ����
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


void InformationLayer::scalecEffect(Node* node)
{
	auto seq = Sequence::create(ScaleTo::create(0.7, 1.2), ScaleTo::create(0.6, 0.9), nullptr);
	node->runAction(RepeatForever::create(seq));
}