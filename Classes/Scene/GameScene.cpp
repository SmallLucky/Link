
#include "GameScene.h"
#include "State/StateList.h"
#include "StartScene.h"
#include "Data/GameData.h"
#include "CommonFunction.h"
#include "Interface/ShopLayer.h"
#include "Interface/StopLayer.h"
#include "Interface/Interface.h"
#include "LevelScene.h"

int  GameScene::_power = 0;
bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	addUI();
	stateMachine = StateMachine::createWithGameScene(this); //创建状态机
	stateMachine->changeState(StartState::create(this)); //进入准备开始游戏状态
	this->scheduleUpdate();

	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	return true;
}
void GameScene::addUI()
{
	//createQuitButton("button/QuitNormal.png", "button/QuitSelected.png"); //添加退出游戏按钮

	auto leftYun = Sprite::create("bg/left_yun.png");
	leftYun->setPosition(CommonFunction::getVisibleAchor(Anchor::LeftButtom,Vec2(0,0)));
	leftYun->setAnchorPoint(Vec2(0,0));
	addChild(leftYun,1);

	auto rightYun = Sprite::create("bg/right_yun.png");
	rightYun->setPosition(CommonFunction::getVisibleAchor(Anchor::RightButtom, Vec2(0, 0)));
	rightYun->setAnchorPoint(Vec2(1, 0));
	addChild(rightYun,1);

	auto stopButton = Button::create("button/stop_button.png");
	stopButton->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,leftYun,Vec2(-30,-30)));
	leftYun->addChild(stopButton);
	stopButton->addClickEventListener([=](Ref*){
		StopLayer* stopLayer = StopLayer::create();
		addChild(stopLayer, 2);
	});

	for (int i = 0; i < 3; i++)
	{
		auto props_bg = Button::create("button/props_button.png", "button/props_button.png");
		props_bg->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, rightYun,Vec2(i*120 -120 ,-30)));
		/*props_bg->setTag(i);*/
		rightYun->addChild(props_bg);
		props_bg->addClickEventListener([=](Ref*){
			ShopLayer* shop = ShopLayer::create();
			addChild(shop,2);
		});

		string str = StringUtils::format("popbox/props_%d.png",i+1);
		auto props = Sprite::create(str);
		if (props)
		{
			props->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, props_bg, Vec2(-10, 10)));
			props_bg->addChild(props);
		}
	}

}
//初始化一局游戏
void GameScene::initGame()
{
	//time = ROUND_TIME;
	//showTime(time); //显示剩余时间

	_count = GAMEDATA->getCount(GAMEDATA->getCurLevel());
	showCount(_count);

	////开始一局减少一点体力
	//int _p = ROUND_POWER;
	//
	//showPower(_p - _power);
	//_power++;

}

//初始化背景层
void GameScene::initBackground()
{
	background = BackgroundLayer::create();
	addChild(background);
}

//初始化矩阵层
void GameScene::initMatrix()
{
	//matrix = MatrixTLayer::create();
	//addChild(matrix);
	matrix = MapLayer::create();
	addChild(matrix);
}

//初始化信息层
void GameScene::initInformation()
{
	information = InformationLayer::create();
	addChild(information);
}

//初始化玩家
void GameScene::initPlayers()
{
	players = Players::create();
	addChild(players);
}

//初始化主控玩家
void GameScene::initMaster()
{
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
//显示游戏剩余步数
void GameScene::showCount(int c)
{
	information->showCount(c);
}

// 显示体力
void GameScene::showPower(int p)
{
	/*information->showPower(p);*/
}


//一次消除后剩余的所有步数

void GameScene::showAllCount()
{
	m_count = _count - matrix->removeMyCount();
	information->showCount(m_count);
}

int GameScene::getMyCount()
{
	return m_count;
}

//一次消除后分数提高并显示，返回提高后的分数
int GameScene::showScoreUp()
{
	return showScoreUp(getScoreByLinkCount(matrix->getRemoveCount(),matrix->getElementType())); //通过消除元素个数计算获得的分数并显示
}

int GameScene::MyShowScoreUp()
{
	return showScoreUp(getScoreByLinkCount(6, 5)); //通过消除元素个数计算获得的分数并显示
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
	return matrix->isLinkFinish();
}

//准备下一次连接
void GameScene::prepareLink()
{
	matrix->prepareLink();
}


//符合条件则消除被连接的元素，否则连接失效
void GameScene::removeLink()
{
	matrix->removeLink();
}

//游戏结束，显示分数，提示开始新游戏
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
	addChild(nextlevel);
}

//开始新游戏
void GameScene::restart(Ref* pSender)
{
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
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);
}

//退出游戏
void GameScene::quitGame(Ref* pSender)
{
	log("Director::getInstance()->popScene();");
	StartScene* startScene = StartScene::create();
	Director::getInstance()->replaceScene(startScene);
}

//返回状态机指针
StateMachine* GameScene::getStateMachine()
{
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