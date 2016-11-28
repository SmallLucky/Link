
#include "GameScene.h"
#include "State/StateList.h"
#include "StartScene.h"
#include "Data/GameData.h"
#include "CommonFunction.h"
#include "Interface/ShopLayer.h"
#include "Interface/StopLayer.h"
#include "Interface/Interface.h"
#include "LevelScene.h"
#include "Interface/AddCount.h"

int  GameScene::_power = 0;
GameScene::GameScene() : m_blue(REWARDDATA->getBlue(GAMEDATA->getCurLevel())), m_purple(REWARDDATA->getPurple(GAMEDATA->getCurLevel())),
m_green(REWARDDATA->getGreen(GAMEDATA->getCurLevel())), m_rad(REWARDDATA->getRad(GAMEDATA->getCurLevel())), 
m_yellor(REWARDDATA->getYellor(GAMEDATA->getCurLevel()))
{
	auto _listenerReresh = EventListenerCustom::create(REFRESHPROPS, [=](EventCustom*event){
		refreshProps();
	});
	auto _listenerIsGameOverTrue = EventListenerCustom::create(ISGAMEOVERTRUE, [=](EventCustom*event){
		setIsGameOver(true);
	});
	auto _listenerIsGameOverFaLse = EventListenerCustom::create(ISGAMEOVERFALSE, [=](EventCustom*event){
		setIsGameOver(false);
		showCount(5);
		//_count += 5;
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerReresh, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerIsGameOverTrue, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerIsGameOverFaLse, 1);

	auto _listenerBlue = EventListenerCustom::create(BLUE, [=](EventCustom*event){
		m_blue -= matrix->removeTargetECount;
		if (m_blue <= 0)
		{
			m_blue = 0;
		}
		showTargetElementNum(0, m_blue);
	});
	auto _listenerPurple = EventListenerCustom::create(PURPLE, [=](EventCustom*event){
		m_purple -= matrix->removeTargetECount;
		if (m_purple <= 0)
		{
			m_purple = 0;
		}
		showTargetElementNum(1, m_purple);
	});
	auto _listenerGreen = EventListenerCustom::create(GREEN, [=](EventCustom*event){
		m_green -= matrix->removeTargetECount;
		if (m_green <= 0)
		{
			m_green = 0;
		}
		showTargetElementNum(2, m_green);
	});
	auto _listenerRad = EventListenerCustom::create(RAD, [=](EventCustom*event){
		m_rad -= matrix->removeTargetECount;
		if (m_rad <= 0)
		{
			m_rad = 0;
		}
		showTargetElementNum(3, m_rad);
	});
	auto _listenerYellor = EventListenerCustom::create(YELLOR, [=](EventCustom*event){
		m_yellor -= matrix->removeTargetECount;
		if (m_yellor <= 0)
		{
			m_yellor = 0;
		}
		showTargetElementNum(4, m_yellor);
	});

	_eventDispatcher->addEventListenerWithFixedPriority(_listenerBlue, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerPurple, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerGreen, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerRad, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerYellor, 1);
}

GameScene::~GameScene()
{
	_eventDispatcher->removeCustomEventListeners(REFRESHPROPS);
	_eventDispatcher->removeCustomEventListeners(ISGAMEOVERTRUE);
	_eventDispatcher->removeCustomEventListeners(ISGAMEOVERFALSE);
	_eventDispatcher->removeCustomEventListeners(BLUE);
	_eventDispatcher->removeCustomEventListeners(PURPLE);
	_eventDispatcher->removeCustomEventListeners(GREEN);
	_eventDispatcher->removeCustomEventListeners(RAD);
	_eventDispatcher->removeCustomEventListeners(YELLOR);
}
bool GameScene::init()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("GameScene::init()");
#endif
	if (!Scene::init())
	{
		return false;
	}

	isGameOver = false;
	addUI();
	stateMachine = StateMachine::createWithGameScene(this); //创建状态机
	stateMachine->changeState(StartState::create(this)); //进入准备开始游戏状态
	this->scheduleUpdate();

	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("GameScene::init() true");
#endif
	return true;
}

bool	GameScene:: getIsGameOver()
{
	return isGameOver;
}
void	GameScene::setIsGameOver(bool b)
{
	isGameOver = b;
}

void GameScene::addUI()
{
	auto bg = Sprite::create("infor/inforxiabg.png");
	bg->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom,Vec2(0,75)));  
	addChild(bg,1);


	auto stopButton = Button::create("button/stop_button2.png", "button/stop_button2.png");
	stopButton->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg, Vec2(-268, -20)));
	bg->addChild(stopButton);
	stopButton->addClickEventListener([=](Ref*){
		if (matrix->isResponse())
		{
			if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
				AudioData::getInstance()->addButtonEffect(1);
			StopLayer* stopLayer = StopLayer::create();
			addChild(stopLayer, 2);
		}
	});

	for (int i = 0; i < 2; i++)
	{
		auto props_bg = Sprite::create("infor/props_bg.png");
		props_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(i*170,-35)));
		bg->addChild(props_bg);

		auto numbg = Button::create("infor/props_numbg.png");
		numbg->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, props_bg, Vec2(-5, 15)));
		props_bg->addChild(numbg);

		if (i==0)
		{
			refreshNum = LabelAtlas::create(Value(GAMEDATA->getRefresh()).asString(), "fonts/game_propsnum.png", 20, 24, '0');
			refreshNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, numbg, Vec2(10, 0)));
			refreshNum->setAnchorPoint(Vec2(0.5, 0.5));
			refreshNum->setScale(0.6);
			numbg->addChild(refreshNum);
			numbg->addClickEventListener(CC_CALLBACK_0(GameScene::addRefreshPropsCallBarck, this));
		}
		if (i==1)
		{
			boombNum = LabelAtlas::create(Value(GAMEDATA->getBoomb()).asString(), "fonts/game_propsnum.png", 20, 24, '0');
			boombNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, numbg, Vec2(10 , 0)));
			boombNum->setAnchorPoint(Vec2(0.5, 0.5));
			boombNum->setScale(0.6);
			numbg->addChild(boombNum);
			numbg->addClickEventListener(CC_CALLBACK_0(GameScene::addBoomPropsCallBarck,this));
		}

		string str = StringUtils::format("infor/props_%d.png", i);
		auto props = Button::create(str);
		if (props)
		{
			props->setScale(1.3);
			props->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,props_bg,Vec2(0,20)));
			props_bg->addChild(props);
			props->addClickEventListener([=](Ref*){
				if (i == 0)
				{
					if (GAMEDATA->getRefresh() > 0)
					{
						if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
							AudioData::getInstance()->addSpecialEffect(4);
						GAMEDATA->setRefresh(GAMEDATA->getRefresh() - 1);
						refreshElement();
						refreshNum->setString(Value(GAMEDATA->getRefresh()).asString());
					}
					else
					{
						//道具不够提醒购买
						cout << "道具不够提醒购买" << endl;
					}

				}
				if (i == 1)
				{
					if (GAMEDATA->getBoomb() > 0)
					{
						if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
							AudioData::getInstance()->addButtonEffect(1);
						if (matrix->getIsBoom())
						{
							matrix->setIsBoom(false);
						}
						else
						{
							matrix->setIsBoom(true);
						}
					}
					else
					{
						//道具不够提醒购买
						cout << "道具不够提醒购买" << endl;
					}
				}
			});
		}
	}

}

//道具按钮旁边添加道具回调方法
void GameScene::addRefreshPropsCallBarck()
{
	bool isBy = false;
	if (GAMEDATA->getLoveNum() >= 2)
	{
		GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() - 2); //爱心大于等于2
		isBy = true;
	}
	else
	{
		isBy = false; //数量不够，弹出购买弹框
		cout << "爱心不够" << endl;
	}
	if (GAMEDATA->getMoneyNum() >= 60)
	{
		if (isBy)
		{
			GAMEDATA->setMoneyNum(GAMEDATA->getMoneyNum() - 60); //爱心扣成功，扣除金币
			GAMEDATA->setRefresh(GAMEDATA->getRefresh() + 1); //两者都扣除后，添加道具
			refreshNum->setString(Value(GAMEDATA->getRefresh()).asString()); //刷新道具数量
		}
	}
	else
	{
		isBy = false;
		cout << "金币不够" << endl;
	}
}

void GameScene::addBoomPropsCallBarck()
{
	bool isBy = false;
	if (GAMEDATA->getLoveNum() >= 2)
	{
		GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() - 2); //爱心大于等于2
		isBy = true;
	}
	else
	{
		isBy = false; //数量不够，弹出购买弹框
		cout << "爱心不够" << endl;
	}
	if (GAMEDATA->getMoneyNum() >= 60)
	{
		if (isBy)
		{
			GAMEDATA->setMoneyNum(GAMEDATA->getMoneyNum() - 60); //爱心扣成功，扣除金币
			GAMEDATA->setRefresh(GAMEDATA->getBoomb() + 1); //两者都扣除后，添加道具
			refreshNum->setString(Value(GAMEDATA->getBoomb()).asString()); //刷新道具数量
		}
	}
	else
	{
		isBy = false;
		cout << "金币不够" << endl;
	}
}

void GameScene::refreshProps()
{
	boombNum->setString(Value(GAMEDATA->getBoomb()).asString());
}
//初始化一局游戏
void GameScene::initGame()
{
	//time = ROUND_TIME;
	//showTime(time); //显示剩余时间
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("GameScene::initGame()");
#endif
	_count = GAMEDATA->getCount(GAMEDATA->getCurLevel());
	showCount(_count);

}

//初始化背景层
void GameScene::initBackground()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("GameScene::initBackground()");
#endif
	background = BackgroundLayer::create();
	addChild(background);
}

//初始化矩阵层
void GameScene::initMatrix()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("GameScene::initMatrix()");
#endif
	//matrix = MatrixTLayer::create();
	//addChild(matrix);
	matrix = MapLayer::create();
	addChild(matrix);
}

//初始化信息层
void GameScene::initInformation()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD(" GameScene::initInformation()");
#endif
	information = InformationLayer::create();
	addChild(information);
}

//初始化玩家
void GameScene::initPlayers()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD(" GameScene::initPlayers()");
#endif
	players = Players::create();
	addChild(players);
}

//初始化主控玩家
void GameScene::initMaster()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("GameScene::initMaster()");
#endif
	players->createAlly(MASTER_ID);
	players->setMaster(MASTER_ID); //设置主控玩家
}

/***************/
//显示游戏剩余时间
//void GameScene::showTime(float t)
//{
//	//information->showTime(t);
//}
////时间流逝--
//float GameScene::timeLapse(float dt)
//{
//	//time -= dt;
//
//	return 1;
//}
////时间流逝并显示--
//float GameScene::timeLapseShow(float dt)
//{
//	//float t = timeLapse(dt);
//	//showTime(t);
//
//	return 1;
//}
/************************************/
void GameScene::addCountLayer()
{
	auto addcountLayer = AddCount::create();
	addChild(addcountLayer);
}

//显示游戏剩余步数
void GameScene::showCount(int c)
{
	information->showCount(c);
}

void  GameScene::showTargetElementNum(int ele, int n)
{
	information->showTargetElementNum(ele,n);
}
// 显示体力
void GameScene::showPower(int p)
{
	/*information->showPower(p);*/
}


//一次消除后剩余的所有步数

void GameScene::showAllCount()
{
	//if (!enoughTargetElement || GAMEDATA->getCurSocre() < GAMEDATA->getTargetScore(GAMEDATA->getCurLevel()))
	//{

	//}
	m_count = GAMEDATA->getCount(GAMEDATA->getCurLevel()) - matrix->removeMyCount();
	if (m_count <= 0 )
	{
		m_count = 0;
	}
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD(" GameScene::showAllCount():%d",m_count);
#endif
	information->showCount(m_count);
}

int GameScene::getMyCount()
{
	return m_count;
}

//一次消除后分数提高并显示，返回提高后的分数
int GameScene::showScoreUp()
{
	int score = getScoreByLinkCount(matrix->getRemoveCount(), matrix->getElementType()) + matrix->getSpecialScore();
	//log(" GameScene::showScoreUp()score :%d ,%d", score, matrix->getSpecialScore());
	return showScoreUp(score); //通过消除元素个数计算获得的分数并显示
}

int GameScene::MyShowScoreUp()
{
	return 0;//showScoreUp(getScoreByLinkCount(6, 5)); //通过消除元素个数计算获得的分数并显示
}
//分数提高并显示，返回提高后的分数
int GameScene::showScoreUp(int delta)
{
	int s = players->scoreUp(delta);
	information->showScore(s);
	if (delta > 0) //提高分数为0则不显示
	{
		information->scoreUp(delta, SCORE_UP_TIME);
	}

	return s; 
}


//矩阵元素下落，返回是否下落结束无空位
bool GameScene::elementsFall()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("GameScene::elementsFall()");
//#endif
	return matrix->elementsFall();
}

//允许开始连接，使矩阵层响应触控
void GameScene::allowLink()
{
	matrix->allowResponse();
}

//禁止开始连接，使矩阵层不响应触控
void GameScene::forbiddenLink()
{
	matrix->forbiddenResponse();
}

//是否有有效的连接结束
bool GameScene::isLinkFinish()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD(" GameScene::isLinkFinish()");
//#endif
	return matrix->isLinkFinish();
}

//准备下一次连接
void GameScene::prepareLink()
{
	matrix->prepareLink();
}

bool GameScene::isResponse()//是否可以触摸
{
	return matrix->isResponse();
}

//符合条件则消除被连接的元素，否则连接失效
void GameScene::removeLink()
{
	matrix->removeLink();
}

//游戏结束，显示分数，提示开始新游戏
void GameScene::gameOver()
{
	if (GAMEDATA->getCurSocre() < GAMEDATA->getTargetScore(GAMEDATA->getCurLevel()))
	{
		LoserLayer* loserLayer = LoserLayer::create();
		addChild(loserLayer,2);
	}
}

void  GameScene::gameNextLevel()
{
	this->unscheduleUpdate();

	auto time = DelayTime::create(1.5);

	auto callFun = CallFunc::create([=]{
		auto nextlevel = TotalLayer::create();
		addChild(nextlevel, 1);
	});

	auto seq = Sequence::create(time, callFun,nullptr);
	this->runAction(seq);

}

bool  GameScene::enoughTargetElement()
{
	int index = GAMEDATA->getCurLevel();
	bool finish = true;
	if (REWARDDATA->getBlue(index) != 0)
	{
		if (m_blue <= 0)//REWARDDATA->getBlue(index)
		{
			finish = true;
		}
		else
		{
			finish = false;
			return finish;
		}
	}
	if (REWARDDATA->getPurple(index) != 0)
	{
		if (m_purple <= 0)//REWARDDATA->getPurple(index
		{
			finish = true;
		}
		else
		{
			finish = false;
			return finish;
		}
	}
	if (REWARDDATA->getGreen(index) != 0)
	{
		if (m_green <= 0) //REWARDDATA->getGreen(index)
		{
			finish = true;
		}
		else
		{
			finish = false;
			return finish;
		}
	}
	if (REWARDDATA->getRad(index) != 0)
	{
		if (m_rad <= 0) //REWARDDATA->getRad(index)
		{
			finish = true;
		}
		else
		{
			finish = false;
			return finish;
		}
	}
	if (REWARDDATA->getYellor(index) != 0)
	{
		if (m_yellor <= 0) //REWARDDATA->getYellor(index)
		{
			finish = true;
		}
		else
		{
			finish = false;
			return finish;
		}
	}
	return finish;
}

//开始新游戏
void GameScene::restart(Ref* pSender)
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("GameScene::restart(Ref* pSender)");
//#endif
	Scene* s = GameScene::create();
	Director::getInstance()->replaceScene(s); //用新的Game场景替换原场景，开始新游戏
}

// 创建退出游戏按钮
void GameScene::createQuitButton(string normal, string selected)
{
	auto closeItem = MenuItemImage::create(normal, selected, CC_CALLBACK_1(GameScene::quitGame, this));
	closeItem->setPosition(Point(WIDTH / 2, 0));
	closeItem->setAnchorPoint(Point(0.5f, 0));
	setSize(closeItem, 0.08f);
	auto menu = Menu::create(closeItem, nullptr); //--nullptr
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);
}

//退出游戏
void GameScene::quitGame(Ref* pSender)
{
	StartScene* startScene = StartScene::create();
	Director::getInstance()->replaceScene(startScene);
}

//返回状态机指针
StateMachine* GameScene::getStateMachine()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//		LOGD("GameScene::getStateMachine()");
//#endif
	return stateMachine;
}
void GameScene::update(float dt)
{
	if (stateMachine)
	{
		stateMachine->update(dt); //游戏主循环由状态机调度
	}
}


void GameScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
			{
				LevelScene* sccene = LevelScene::create();
				Director::getInstance()->replaceScene(sccene );
			}
		}
	}
}