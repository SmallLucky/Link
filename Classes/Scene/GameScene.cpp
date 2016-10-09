
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
	stateMachine = StateMachine::createWithGameScene(this); //����״̬��
	stateMachine->changeState(StartState::create(this)); //����׼����ʼ��Ϸ״̬
	this->scheduleUpdate();

	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	return true;
}
void GameScene::addUI()
{
	//createQuitButton("button/QuitNormal.png", "button/QuitSelected.png"); //����˳���Ϸ��ť

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
//��ʼ��һ����Ϸ
void GameScene::initGame()
{
	//time = ROUND_TIME;
	//showTime(time); //��ʾʣ��ʱ��

	_count = GAMEDATA->getCount(GAMEDATA->getCurLevel());
	showCount(_count);

	////��ʼһ�ּ���һ������
	//int _p = ROUND_POWER;
	//
	//showPower(_p - _power);
	//_power++;

}

//��ʼ��������
void GameScene::initBackground()
{
	background = BackgroundLayer::create();
	addChild(background);
}

//��ʼ�������
void GameScene::initMatrix()
{
	//matrix = MatrixTLayer::create();
	//addChild(matrix);
	matrix = MapLayer::create();
	addChild(matrix);
}

//��ʼ����Ϣ��
void GameScene::initInformation()
{
	information = InformationLayer::create();
	addChild(information);
}

//��ʼ�����
void GameScene::initPlayers()
{
	players = Players::create();
	addChild(players);
}

//��ʼ���������
void GameScene::initMaster()
{
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
	return showScoreUp(getScoreByLinkCount(matrix->getRemoveCount(),matrix->getElementType())); //ͨ������Ԫ�ظ��������õķ�������ʾ
}

int GameScene::MyShowScoreUp()
{
	return showScoreUp(getScoreByLinkCount(6, 5)); //ͨ������Ԫ�ظ��������õķ�������ʾ
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
	return matrix->isLinkFinish();
}

//׼����һ������
void GameScene::prepareLink()
{
	matrix->prepareLink();
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
	addChild(nextlevel);
}

//��ʼ����Ϸ
void GameScene::restart(Ref* pSender)
{
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