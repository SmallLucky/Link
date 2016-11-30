#pragma once 

#include "cocos2d.h"
#include "Global.h"
#include "Data/Data.h"
#include "Interface/Interface.h"
#include "State/StateMachine.h"
#include "User/User.h"

class GameScene : public Scene
{
protected:
	BackgroundLayer* background; // ������
	//MatrixTLayer* matrix;  //�����
	InformationLayer* information; //��Ϣ��//��Ϣ��
	Players* players;//�����Ϣ
	MapLayer* matrix;

	//ʱ��
	float time;//--
	//void showTime(float t); //��ʾ��Ϸʣ��ʱ��--
	//float timeLapse(float dt); //ʱ������--

	//����
	int _count;
	int m_count = 1;
	void showCount(int c); //��ʾ��Ϸʣ�ಽ��
	int curScore;

	int m_blue, m_purple,m_green,m_rad,m_yellor;
	void showTargetElementNum(int ele,int n); //��ʾ��Ϸʣ�ಽ��
	//����
	 static int _power;

	 LabelAtlas* refreshNum;
	 LabelAtlas* boombNum;


	void restart(Ref* pSender); //��ʼ����Ϸ
	void createQuitButton(string normal, string selected);//�����˳���Ϸ��ť
	void quitGame(Ref* pSender); //�˳���Ϸ
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

	bool isGameOver;
public:
	bool getIsLine(){ return matrix->getIsLine(); }

	bool isRefreshElement(){ return matrix->checkIsNeedRefresh(); }
	void refreshElement() { matrix->refreshAllElement();  }
	void refreshProps();

	void addCountLayer();
	bool getIsGameOver();
	void setIsGameOver(bool b);

	void addRefreshPropsCallBarck(); //���߰�ť�Ա���ӵ��߻ص�����
	void addBoomPropsCallBarck();

	bool  enoughTargetElement();

public:
	GameScene();
	~GameScene();
	CREATE_FUNC(GameScene);
	bool init();

	void addUI();

	void initGame();		//��ʼ��һ����Ϸ
	void initBackground();	//��ʼ��������
	void initMatrix(); //��ʼ�������

	void initInformation(); //��ʼ����Ϣ��

	void initPlayers(); //��ʼ�����
	void initMaster(); //��ʼ���������

	//float timeLapseShow(float dt); //ʱ�����Ų���ʾ--

	int showScoreUp(); //һ�������������߲���ʾ��������ߺ�ķ���
	int showScoreUp(int delta); //������߲���ʾ��������ߺ�ķ���

	int MyShowScoreUp();

	 //һ��������ʣ������в���
	int getMyCount();
	void showAllCount();

	void showPower(int p); // ��ʾ����

	bool elementsFall();//����Ԫ�����䣬�����Ƿ���������޿�λ


	void allowLink(); //����ʼ���ӣ�ʹ�������Ӧ����
	void forbiddenLink();//��ֹ��ʼ���ӣ�ʹ����㲻��Ӧ����
	bool isLinkFinish();//�Ƿ�����Ч�����ӽ���
	void prepareLink(); //׼����һ������
	bool isResponse();//�Ƿ���Դ���
	void removeLink(); //�������������������ӵ�Ԫ�أ���������ʧЧ
	void gameOver();//��Ϸ��������ʾ��������ʾ��ʼ����Ϸ
	void gameNextLevel();
public:
	StateMachine* stateMachine; //״̬��
	StateMachine* getStateMachine();//����״̬��ָ��
	void update(float dt);
};