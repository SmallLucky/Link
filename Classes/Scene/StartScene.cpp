#include "StartScene.h"
#include "Data/Data.h"
#include "GameScene.h"
#include "LevelScene.h"
#include "Interface/SetLayer.h"
#include "Interface/ShopLayer.h"
#include "Interface/QuitLayer.h"
#include "CommonFunction.h"
#include "PreloadUtils.h"
//test
#include "Interface/Interface.h"
#include "Interface/PowerLayer.h"

bool StartScene::init()
{
		if (!Scene::init())
		{
			return false;
		}

		PreloadUtils::getInstance()->preloadBg();
		PreloadUtils::getInstance()->preloadKuang();

		REWARDDATA->praseJsonData();

		initUI();
		PlayStopBGM();
		auto listenerkeyPad = EventListenerKeyboard::create();
		listenerkeyPad->onKeyReleased = CC_CALLBACK_2(StartScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

		return true;
}

void StartScene::initUI()
{
	auto bg_sp = Sprite::create("startScene/bg_image.png");
	if (bg_sp){
		addChild(bg_sp);
		bg_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,0)));
	}

	auto shadow_sp = Sprite::create("startScene/startbtn_shadow.png");
	if (shadow_sp){
		addChild(shadow_sp,9);
		shadow_sp->setPosition(CommonFunction::getVisibleAchor(0.5, 0.25, Vec2(0, -30)));
	}

	auto start_btn = Button::create("startScene/startbtn_nor.png","startScene/startbtn_sel.png");
	if (start_btn){
		//start_btn->setScale(0.7f);
		addChild(start_btn,10);
		start_btn->setPosition(CommonFunction::getVisibleAchor(0.5,0.25,Vec2(0,-35)));
		start_btn->addClickEventListener(CC_CALLBACK_1(StartScene::StartGame,this));
	}

	auto light_sp = Sprite::create("startScene/light.png");
	if (light_sp){
		addChild(light_sp);
		light_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, -35)));
		rotateAction(light_sp);
		fadeInAction(light_sp,1.5f);
	}

	auto title_sp = Sprite::create("startScene/tital.png");
	if (title_sp){
		addChild(title_sp);
		title_sp->setPosition(CommonFunction::getVisibleAchor(0.5,0.80 , Vec2(0, 0)));
		fadeInAction(title_sp, 0.2f);
	}

	auto cloud_1 = Sprite::create("startScene/cloud_1.png");
	if (cloud_1){
		addChild(cloud_1);
		cloud_1->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.4f, Vec2(0, 0)));
		cloudMoveAction(cloud_1,0,60.0f);
	}

	auto cloud_2 = Sprite::create("startScene/cloud_2.png");
	if (cloud_2){
		addChild(cloud_2,11);
		cloud_2->setPosition(CommonFunction::getVisibleAchor(1.0f, 0.5f, Vec2(0, -100)));
		cloudMoveAction(cloud_2, 0, 50.0f);
	}

	auto cloud_3 = Sprite::create("startScene/cloud_2.png");
	if (cloud_3){
		addChild(cloud_3);
		cloud_3->setPosition(CommonFunction::getVisibleAchor(0.75f, 0.5f, Vec2(0, 0)));
	}

	auto cloud_4 = Sprite::create("startScene/cloud_3.png");
	if (cloud_4){
		cloud_4->setScale(1.3f);
		addChild(cloud_4,11);
		cloud_4->setPosition(CommonFunction::getVisibleAchor(0.25f, 0.35f, Vec2(80, 20)));
	}

	auto cloud_8 = Sprite::create("startScene/cloud_3.png");
	if (cloud_8){
		cloud_8->setScale(1.5f);
		addChild(cloud_8,11);
		cloud_8->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.35f, Vec2(50, 20)));
	}


	auto cloud_5 = Sprite::create("startScene/cloud_2.png");
	if (cloud_5){
		addChild(cloud_5,10);
		cloud_5->setPosition(CommonFunction::getVisibleAchor(0.45f, 0.6f, Vec2(0, 0)));
		cloudMoveAction(cloud_5, 0, 60.0f);
	}

	auto cloud_6 = Sprite::create("startScene/cloud_2.png");
	if (cloud_6){
		addChild(cloud_6, 10);
		cloud_6->setPosition(CommonFunction::getVisibleAchor(0.9f, 0.8f, Vec2(0, 65)));
		cloudMoveAction(cloud_6, 0, 100.0f);
	}

	auto cloud_7 = Sprite::create("startScene/cloud_2.png");
	if (cloud_7){
		cloud_7->setScale(0.8f);
		addChild(cloud_7, 10);
		cloud_7->setPosition(CommonFunction::getVisibleAchor(0.25f, 0.8f, Vec2(0, -50)));
		cloudMoveAction(cloud_7, 0, 50.0f);
	}
	//企鹅
	auto penguin_sp = Sprite::create("startScene/animal_penguin.png");
	if (penguin_sp){
		penguin_sp->setScale(0.8f);
		addChild(penguin_sp, 10);
		penguin_sp->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f,this,Vec2(0, -55)));
		fadeInAction(penguin_sp, 0.4f);
		penguin_shakeAction(penguin_sp,2.0f);
	}
	else{
		return;
	}

	auto candy_2 = Sprite::create("startScene/candy_2.png");
	if (candy_2){
		addChild(candy_2,8);
		candy_2->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 0)));
		auto to = CommonFunction::getVisibleAchor(Anchor::Center, Vec2(130, -120));
		//auto to = CommonFunction::getVisibleAchor(0.75, 0.5, Vec2(10, 70));
		fadeInAction(candy_2, candy_2->getPosition(), to, 0.8f);
		moveAction(candy_2, Vec2(15, 10), 0.9f, 5.0f);
	}

	//星星
	auto star_sp = Sprite::create("startScene/star.png");
	if (star_sp){
		addChild(star_sp, 10);
		star_sp->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, this, Vec2(0, -20)));
		fadeInAction(star_sp, 2.0f);
		//blinkAction(star_sp,2.0f,3);
	}

	//小鬼
	auto cat_sp_1 = Sprite::create("startScene/animal_ghost.png");
	if (cat_sp_1){
		addChild(cat_sp_1);
		cat_sp_1->setScale(0.8f);
		auto to = CommonFunction::getVisibleAchor(0.20, 0.5, Vec2(0, 40));
		fadeInAction(cat_sp_1, penguin_sp->getPosition(), to, 0.5f);
		moveAction(cat_sp_1, Vec2(-10,10),0.7f,5.0f);
	}

	auto candy_1 = Sprite::create("startScene/candy_1.png");
	if (candy_1 && cat_sp_1){
		cat_sp_1->addChild(candy_1);
		candy_1->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftMid, cat_sp_1, Vec2(60, -15)));
	}
	//方猫
	/*auto cat_sp_2 = Sprite::create("startScene/animal_cat_2.png");
	if (cat_sp_2){
		addChild(cat_sp_2);
		cat_sp_2->setScale(0.8f);
		auto to = CommonFunction::getVisibleAchor(0.25, 0.35, Vec2(30, -15));
		fadeInAction(cat_sp_2, penguin_sp->getPosition(), to, 1.0f);
		moveAction(cat_sp_2, Vec2(-15, -10), 1.1f, 5.0f);
	}*/
	//小鸡
	auto chicken_sp = Sprite::create("startScene/animal_chicken.png");
	if (chicken_sp){
		chicken_sp->setScale(0.8f);
		addChild(chicken_sp);
		chicken_sp->setPosition(CommonFunction::getVisibleAchor(0.75, 0.5, Vec2(10, 70)));
		auto to = CommonFunction::getVisibleAchor(0.75, 0.5, Vec2(10, 70));
		fadeInAction(chicken_sp, penguin_sp->getPosition(), to, 0.8f);
		moveAction(chicken_sp, Vec2(15, 10), 0.9f, 5.0f);
	}

	//小鬼
	/*auto ghost_sp = Sprite::create("startScene/animal_ghost.png");
	if (ghost_sp){
		ghost_sp->setScale(0.8f);
		addChild(ghost_sp);
		ghost_sp->setPosition(CommonFunction::getVisibleAchor(0.75, 0.35, Vec2(0, 0)));
		auto to = CommonFunction::getVisibleAchor(0.65, 0.35, Vec2(0, 0));
		fadeInAction(ghost_sp, penguin_sp->getPosition(), to, 1.2f);
		moveAction(ghost_sp, Vec2(15, -10), 1.3f, 5.0f);
	}*/
}

void StartScene::rotateAction(Node* node)
{
	if (!node){
		return;
	}
	
	auto r_node = dynamic_cast<Sprite*>(node);
	auto rotate_ac = RotateBy::create(60.0f,360.0);
	if (r_node){
		r_node->runAction(RepeatForever::create(rotate_ac));
	}
}

void StartScene::blinkAction(Node* node, float delay,int blink)
{
	auto r_node = dynamic_cast<Sprite*>(node);
	if (!r_node){
		return;
	}

	auto bl = Blink::create(0.5f, blink);
	auto del = DelayTime::create(delay);
	auto seq = Sequence::create(del, bl, nullptr);
	r_node->runAction(RepeatForever::create(seq));
}

void StartScene::penguin_shakeAction(Node* node, float delay)
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

void StartScene::fadeInAction(Node* node, float delay)
{
	if (!node){
		return;
	}

	auto f_node = dynamic_cast<Sprite*>(node);
	auto d_act = DelayTime::create(delay);
	auto f_act = FadeIn::create(0.3f);
	auto s_act = ScaleTo::create(0.6f,1.0f);
	//auto ease = EaseExponentialOut::create(s_act);
	auto ease_act = EaseElasticOut::create(s_act);
	auto spawn = Spawn::create(f_act, ease_act, nullptr);
	auto seq = Sequence::create(d_act,spawn,nullptr);
	if (f_node){
		f_node->setOpacity(0);
		f_node->setScale(0);
		f_node->runAction(seq);
	}
}

void StartScene::fadeInAction(Node* node, Vec2 from, Vec2 to, float delay)
{
	if (!node){
		return;
	}

	auto f_node = dynamic_cast<Sprite*>(node);
	auto d_act = DelayTime::create(delay);
	auto f_act = FadeIn::create(0.4f);
	auto m_ease = EaseExponentialOut::create(MoveTo::create(1.0f, to));
	auto ease_act = EaseExponentialOut::create(ScaleTo::create(1.0f, 1.0f));
	auto spawn = Spawn::create(f_act, ease_act, m_ease , nullptr);
	auto seq = Sequence::create(d_act, spawn, nullptr);
	if (f_node){
		f_node->setOpacity(0);
		f_node->setScale(0);
		f_node->setPosition(from);
		f_node->runAction(seq);
	}
}

void StartScene::moveAction(Node* node, Vec2 to, float delay, float d)
{
	if (!node){
		return;
	}

	auto f_node = dynamic_cast<Sprite*>(node);
	auto d_act = DelayTime::create(delay);
	auto mov_act = MoveBy::create(d, to);
	auto mov_re = mov_act->reverse();
	auto seq = Sequence::create(d_act, mov_act, mov_re, nullptr);
	
	if (f_node){
		f_node->runAction(RepeatForever::create(seq));
	}
}

void StartScene::cloudMoveAction(Node* node, float delay, float d)
{
	auto f_node = dynamic_cast<Sprite*>(node);
	if (!f_node){
		return;
	}

	auto visSize = Director::getInstance()->getVisibleSize();
	auto pos = Vec2(-f_node->getContentSize().width / 2,f_node->getPositionY());

	auto d_act = DelayTime::create(delay);
	auto mov_act = MoveTo::create(d, pos);
	auto callf = CallFunc::create([=](){
		if (f_node->getPositionX()< 0 ){
			f_node->setPosition(Vec2(visSize.width + f_node->getContentSize().width / 2, f_node->getPositionY()));
		}
	});
	auto seq = Sequence::create(d_act,mov_act,callf,nullptr);
	f_node->runAction(RepeatForever::create(seq));
}

void	StartScene::PlayStopBGM()
{
	UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true); // 
	bool flag = UserDefault::getInstance()->getBoolForKey("IS_MUSIC",true);
	if (flag)
	{
		AudioData::getInstance()->addBgMusic(1);
	}
	else
	{
		AudioData::getInstance()->stopBgMusic();
	}
}
//开始游戏 
void StartScene::StartGame(Ref* ref)
{
	
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
	{
		AudioData::getInstance()->addButtonEffect(1);
	}

	LevelScene* levelScene = LevelScene::create();
	if (levelScene)
	{
		Director::getInstance()->replaceScene(levelScene);
	}

	//auto layer = LoserLayer::create();
	//addChild(layer,10);
	
}

long StartScene::millisecondNow()
{
	struct timeval now;
	gettimeofday(&now,nullptr ); //--nullptr
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

void StartScene::logTime()
{
	log("Time:%ld", millisecondNow());
}
//设置
void StartScene::SetButton()
{
	SetLayer* setLayer = SetLayer::create();
	if (setLayer)
	{
		addChild(setLayer);
	}
}
//退出游戏
void StartScene::QuitGame(Ref* ref)
{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
//	return;
//#endif
	auto quitLayer = QuitLayer::create();
	if (quitLayer)
	{
		addChild(quitLayer);
	}
	//Director::getInstance()->end();

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	exit(0);
//#endif
}

void StartScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
			{
				//auto quitLayer = QuitLayer::create();
				auto quitLayer = PowerLayer::create();
				if (quitLayer)
				{
					addChild(quitLayer,100);
				}
			}
		}
	}
}