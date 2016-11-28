#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

/*
	//ʹ��
	TTButton  *  buttonTest = TTButton::createWithImage("CloseNormal.png");
	buttonTest->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	buttonTest->addTouchupInsideHandle(this, callfuncO_selector(HelloWorld::menuCloseCallback));
	this->addChild(buttonTest);
*/

typedef   enum  _BTN_STATE
{
	BTN_STATE_UNSET = -1,
	BTN_STATE_NORMAL = 0,
	BTN_STATE_PRESS = 1,
	BTN_STATE_DISABLE = 2,

} BTN_STATE;

class   TTButton :public  Node
{
public:
	TTButton();
	~TTButton();

	void  setSpriteGray(Node *pNode, bool isGray);
	void  addAnimation(Node* pNode);
	bool  initWithImages(std::string  strNormal, std::string strSel, std::string strDisable);
	bool  initWithImage(std::string  strNormal);

	bool  initWithImages(std::string  strNormal, std::string strSel, std::string strDisable, Size   rectSize);
	bool  initWithImage(std::string  strNormal, Size   rectSize);

	//����ĳ��״̬�µ�ͼ��
	void   upDateImageForState(BTN_STATE  btnState, Texture2D *texture);

	static  TTButton * createWithImage(std::string  strNormal);
	static  TTButton * createWithImages(std::string  strNormal, std::string strSel, std::string strDisable);
	static  TTButton  * createWithImage(std::string  strNormal, Size   rectSize);
	static  TTButton  * createWithImages(std::string  strNormal, std::string strSel, std::string strDisable, Size   rectSize);

	//���� ��ɫ
	void   setColorForState(BTN_STATE btnState, Color3B  color);
	//����״̬UI
	void  upDateStatusUI();
	//����ѡ��״̬
	void   setSelected();
	//���÷�ѡ��״̬
	void   setUnSelected();
	//�����Ƿ���disable״̬
	void   setIsDisable(bool  bDisabel);
	//��õ�ǰ״̬
	BTN_STATE  getCurState();
	//��� ����handle
	void   addTouchupInsideHandle(Node  * target, SEL_CallFuncO selector);
	//�ж��Ƿ��ǵ������Ч����
	bool   isTouchInvalidated(Touch* touch);

	virtual void  onEnter();
	virtual void  onExit();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

public:
	BTN_STATE  m_btnState;

	Sprite *  m_normalSprite;
	Sprite *  m_pressedSprite;
	Sprite *  m_disableSprite;

	ui::Scale9Sprite * m_normalScaleSprite;
	ui::Scale9Sprite * m_pressedScaleSprite;
	ui::Scale9Sprite * m_disableScaleSprite;

	bool m_bTouchEnabled;
	CC_SYNTHESIZE(bool, m_bSelected, IsSelected);
	CC_SYNTHESIZE(bool, m_bDisabled, IsDisabled);
	CC_SYNTHESIZE(SEL_CallFuncO, m_touchupInsideSelector, TouchupSelector);
	CC_SYNTHESIZE(Node *, m_target, Target);
	CC_SYNTHESIZE_RETAIN(Ref  *, m_saveObj, SaveObj);
};