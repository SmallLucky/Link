
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
GameScene::GameScene()
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
}

GameScene::~GameScene()
{
	_eventDispatcher->removeCustomEventListeners(REFRESHPROPS);
	_eventDispatcher->removeCustomEventListeners(ISGAMEOVERTRUE);
	_eventDispatcher->removeCustomEventListeners(ISGAMEOVERFALSE);
}
bool GameScene::init()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("GameScene::init()");
#endif
	if (!Scene::init())
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		LOGD("GameScene::init() false");
#endif
		return false;
	}
	isGameOver = false;
	addUI();
	stateMachine = StateMachine::createWithGameScene(this); //����״̬��
	stateMachine->changeState(StartState::create(this)); //����׼����ʼ��Ϸ״̬
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
	stopButton->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, bg, Vec2(-180, -30)));
	bg->addChild(stopButton);
	stopButton->addClickEventListener([=](Ref*){
		if (matrix->isResponse())
		{
			if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
			StopLayer* stopLayer = StopLayer::create();
			addChild(stopLayer, 2);
		}
	});

	for (int i = 0; i < 2; i++)
	{
		auto props_bg = Sprite::create("infor/props_bg.png");
		props_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,bg,Vec2(i*160-30,-30)));
		bg->addChild(props_bg);

		auto numbg = Sprite::create("infor/props_numbg.png");
		numbg->setPosition(CommonFunction::getVisibleAchor(Anchor::RightTop, props_bg, Vec2(0, 0)));
		props_bg->addChild(numbg);

		if (i==0)
		{
			refreshNum = LabelAtlas::create(Value(GAMEDATA->getRefresh()).asString(), "fonts/game_propsnum.png", 16, 20, '0');
			refreshNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, numbg, Vec2(0, 0)));
			refreshNum->setAnchorPoint(Vec2(0.5, 0.5));
			numbg->addChild(refreshNum);
		}
		if (i==1)
		{
			boombNum = LabelAtlas::create(Value(GAMEDATA->getBoomb()).asString(), "fonts/game_propsnum.png", 16, 20, '0');
			boombNum->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, numbg, Vec2(0, 0)));
			boombNum->setAnchorPoint(Vec2(0.5, 0.5));
			numbg->addChild(boombNum);
		}

		string str = StringUtils::format("infor/props_%d.png", i);
		auto props = Button::create(str);
		if (props)
		{
			props->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,props_bg,Vec2(0,0)));
			props_bg->addChild(props);
			props->addClickEventListener([=](Ref*){
				if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CLICK_BUTTON);
				if (i == 0)
				{
					if (GAMEDATA->getRefresh() > 0)
					{
						GAMEDATA->setrefresh(GAMEDATA->getRefresh() - 1);
						refreshElement();
						refreshNum->setString(Value(GAMEDATA->getRefresh()).asString());
					}

				}
				if (i == 1)
				{
					if (GAMEDATA->getBoomb())
					{
						if (matrix->getIsBoom())
						{
							matrix->setIsBoom(false);
						}
						else
						{
							matrix->setIsBoom(true);
						}
					}
				}
			});
		}
	}

}

void GameScene::refreshProps()
{
	boombNum->setString(Value(GAMEDATA->getBoomb()).asString());
}
//��ʼ��һ����Ϸ
void GameScene::initGame()
{
	//time = ROUND_TIME;
	//showTime(time); //��ʾʣ��ʱ��
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("GameScene::initGame()");
#endif
	_count = GAMEDATA->getCount(GAMEDATA->getCurLevel());
	showCount(_count);

}

//��ʼ��������
void GameScene::initBackground()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("GameScene::initBackground()");
#endif
	background = BackgroundLayer::create();
	addChild(background);
}

//��ʼ�������
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

//��ʼ����Ϣ��
void GameScene::initInformation()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD(" GameScene::initInformation()");
#endif
	information = InformationLayer::create();
	addChild(information);
}

//��ʼ�����
void GameScene::initPlayers()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD(" GameScene::initPlayers()");
#endif
	players = Players::create();
	addChild(players);
}

//��ʼ���������
void GameScene::initMaster()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("GameScene::initMaster()");
#endif
	players->createAlly(MASTER_ID);
	players->setMaster(MASTER_ID); //�����������
}

/***************/
//��ʾ��Ϸʣ��ʱ��
//void GameScene::showTime(float t)
//{
//	//information->showTime(t);
//}
////ʱ������--
//float GameScene::timeLapse(float dt)
//{
//	//time -= dt;
//
//	return 1;
//}
////ʱ�����Ų���ʾ--
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

//��ʾ��Ϸʣ�ಽ��
void GameScene::showCount(int c)
{
	information->showCount(c);
}

// ��ʾ����
void GameScene::showPower(int p)
{
	/*information->showPower(p);*/
}


//һ��������ʣ������в���

void GameScene::showAllCount()
{
	m_count = _count - matrix->removeMyCount();
	information->showCount(m_count);
}

int GameScene::getMyCount()
{
	return m_count;
}

//һ�������������߲���ʾ��������ߺ�ķ���
int GameScene::showScoreUp()
{
	int score = getScoreByLinkCount(matrix->getRemoveCount(), matrix->getElementType()) + matrix->getSpecialScore();
	//log(" GameScene::showScoreUp()score :%d ,%d", score, matrix->getSpecialScore());
	return showScoreUp(score); //ͨ������Ԫ�ظ��������õķ�������ʾ
}

int GameScene::MyShowScoreUp()
{
	return 0;//showScoreUp(getScoreByLinkCount(6, 5)); //ͨ������Ԫ�ظ��������õķ�������ʾ
}
//������߲���ʾ��������ߺ�ķ���
int GameScene::showScoreUp(int delta)
{
	int s = players->scoreUp(delta);
	information->showScore(s);
	if (delta > 0) //��߷���Ϊ0����ʾ
	{
		information->scoreUp(delta, SCORE_UP_TIME);
	}

	return s; 
}


//����Ԫ�����䣬�����Ƿ���������޿�λ
bool GameScene::elementsFall()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("GameScene::elementsFall()");
//#endif
	log(" GameScene::elementsFall()");

	return matrix->elementsFall();
}

//����ʼ���ӣ�ʹ�������Ӧ����
void GameScene::allowLink()
{
	matrix->allowResponse();
}

//��ֹ��ʼ���ӣ�ʹ����㲻��Ӧ����
void GameScene::forbiddenLink()
{
	matrix->forbiddenResponse();
}

//�Ƿ�����Ч�����ӽ���
bool GameScene::isLinkFinish()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD(" GameScene::isLinkFinish()");
//#endif
	return matrix->isLinkFinish();
}

//׼����һ������
void GameScene::prepareLink()
{
	matrix->prepareLink();
}

bool GameScene::isResponse()//�Ƿ���Դ���
{
	return matrix->isResponse();
}

//�������������������ӵ�Ԫ�أ���������ʧЧ
void GameScene::removeLink()
{
	matrix->removeLink();
}

//��Ϸ��������ʾ��������ʾ��ʼ����Ϸ
void GameScene::gameOver()
{
	log("gameOver()");
	if (GAMEDATA->getCurSocre() < GAMEDATA->getTargetScore(GAMEDATA->getCurLevel()))
	{
		LoserLayer* loserLayer = LoserLayer::create();
		addChild(loserLayer,2);
	}
}

void  GameScene::gameNextLevel()
{
	auto nextlevel = TotalLayer::create();
	addChild(nextlevel,1);
}

//��ʼ����Ϸ
void GameScene::restart(Ref* pSender)
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("GameScene::restart(Ref* pSender)");
//#endif
	Scene* s = GameScene::create();
	Director::getInstance()->replaceScene(s); //���µ�Game�����滻ԭ��������ʼ����Ϸ
}

// �����˳���Ϸ��ť
void GameScene::createQuitButton(string normal, string selected)
{
	auto closeItem = MenuItemImage::create(normal, selected, CC_CALLBACK_1(GameScene::quitGame, this));
	closeItem->setPosition(Point(WIDTH / 2, 0));
	closeItem->setAnchorPoint(Point(0.5f, 0));
	setSize(closeItem, 0.08f);
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);
}

//�˳���Ϸ
void GameScene::quitGame(Ref* pSender)
{
	log("Director::getInstance()->popScene();");
	StartScene* startScene = StartScene::create();
	Director::getInstance()->replaceScene(startScene);
}

//����״̬��ָ��
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
		stateMachine->update(dt); //��Ϸ��ѭ����״̬������
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //����
			{
				LevelScene* sccene = LevelScene::create();
				Director::getInstance()->replaceScene(sccene );
			}
		}
	}
}