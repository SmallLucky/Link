#include "TTButton.h"
#include "CommonFunction.h"
#include "Shake.h"

using namespace ui;

TTButton::TTButton()
{
	//让调用  registerWithTouchDispatcher 函数
	m_bTouchEnabled = true;
	m_bSelected = false;
	m_bDisabled = false;
	m_btnState = BTN_STATE_UNSET;
	m_normalSprite = nullptr;
	m_pressedSprite = nullptr;
	m_disableSprite = nullptr;
	m_normalScaleSprite = nullptr;
	m_pressedScaleSprite = nullptr;
	m_disableScaleSprite = nullptr;
	m_saveObj = nullptr;
	m_touchupInsideSelector = nullptr;
}

TTButton::~TTButton()
{

}

void  TTButton::setSpriteGray(Node *pNode, bool isGray)
{
	if (!pNode){
		return;
	}
	//变颜色
	if (isGray)
	{
		pNode->setScale(1.20f);
	}
}

//添加的动画效果
void  TTButton::addAnimation(Node* pNode)
{
	if (!pNode){
		return;
	}

	if (pNode)
	{
		Size size = pNode->getContentSize();
		this->setContentSize(size);
		this->ignoreAnchorPointForPosition(true);
		this->setAnchorPoint(Vec2(0.5,0.5));
	}

	auto seq1 = Sequence::create(ScaleTo::create(0.1, 1.3, 0.7), ScaleTo::create(0.05, 1, 1), nullptr);
	auto seq2 = Sequence::create(ScaleTo::create(0.1, 1.4, 0.6), ScaleTo::create(0.05, 1, 1), nullptr);
	this->runAction(Sequence::create(seq1, seq2, nullptr));

}

bool  TTButton::initWithImages(std::string  strNormal, std::string strSel, std::string strDisable)
{
	if (strNormal.empty())
	{
		return  false;
	}
	//normal
	SpriteFrame  *   normalFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strNormal.c_str());
	if (normalFrame)
	{
		m_normalSprite = Sprite::createWithSpriteFrame(normalFrame);
	}
	if (m_normalSprite == nullptr)
	{
		m_normalSprite = Sprite::create(strNormal.c_str());
	}

	if (m_normalSprite == nullptr)
	{
		return  false;
	}
	else
	{
		m_normalSprite->setAnchorPoint(Vec2::ZERO);
		m_normalSprite->setPosition(Vec2::ZERO);
		this->addChild(m_normalSprite);
		this->setContentSize(m_normalSprite->getContentSize());
	}

	//sel
	if (strSel.empty())
	{
		SpriteFrame  *   normalFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strNormal.c_str());
		if (normalFrame)
		{
			m_pressedSprite = Sprite::createWithSpriteFrame(normalFrame);
		}
		if (m_pressedSprite == nullptr)
		{
			m_pressedSprite = Sprite::create(strNormal.c_str());
		}
	}
	else
	{
		SpriteFrame  *   selFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strSel.c_str());
		if (selFrame)
		{
			m_pressedSprite = Sprite::createWithSpriteFrame(selFrame);
		}
		if (m_pressedSprite == nullptr)
		{
			m_pressedSprite = Sprite::create(strSel.c_str());
		}
	}

	if (m_pressedSprite)
	{
		m_pressedSprite->setAnchorPoint(Vec2::ZERO);
		m_pressedSprite->setPosition(Vec2::ZERO);
		this->addChild(m_pressedSprite);
	}

	//disabel
	if (strDisable.empty())
	{
		SpriteFrame  *   normalFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strNormal.c_str());
		if (normalFrame)
		{
			m_disableSprite = Sprite::createWithSpriteFrame(normalFrame);
		}
		if (m_disableSprite == nullptr)
		{
			m_disableSprite = Sprite::create(strNormal.c_str());
		}

		//如果是用  normal处理的，那么灰化
		if (m_disableSprite)
		{
			//setSpriteGray(m_disableSprite, true);
		}
	}
	else
	{
		SpriteFrame  *   disableFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strDisable.c_str());
		if (disableFrame)
		{
			m_disableSprite = Sprite::createWithSpriteFrame(disableFrame);
		}
		if (m_disableSprite == nullptr)
		{
			m_disableSprite = Sprite::create(strDisable.c_str());
		}
	}

	if (m_disableSprite)
	{
		m_disableSprite->setAnchorPoint(Vec2::ZERO);
		m_disableSprite->setPosition(Vec2::ZERO);
		this->addChild(m_disableSprite);
	}
	//显示正常状态
	this->setUnSelected();
	return true;
}

bool  TTButton::initWithImage(std::string  strNormal)
{
	return  this->initWithImages(strNormal, "", "");
}

bool  TTButton::initWithImages(std::string  strNormal, std::string strSel, std::string strDisable, Size   rectSize)
{
	if (strNormal.empty())
	{
		return  false;
	}
	
	//normal
	SpriteFrame  *normalFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strNormal.c_str());
	if (normalFrame)
	{
		m_normalScaleSprite = Scale9Sprite::createWithSpriteFrame(normalFrame);
	}
	if (m_normalScaleSprite == nullptr)
	{
		m_normalScaleSprite = Scale9Sprite::create(strNormal.c_str());
	}

	if (m_normalScaleSprite == nullptr)
	{
		return  false;
	}
	else
	{
		m_normalScaleSprite->setContentSize(rectSize);
		m_normalScaleSprite->setAnchorPoint(Vec2::ZERO);
		m_normalScaleSprite->setPosition(Vec2::ZERO);
		this->addChild(m_normalScaleSprite);
		//set  contentSize
		this->setContentSize(rectSize);
	}
	//sel
	if (strSel.empty())
	{
		SpriteFrame  *   normalFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strNormal.c_str());
		if (normalFrame)
		{
			m_pressedScaleSprite = Scale9Sprite::createWithSpriteFrame(normalFrame);
		}
		if (m_pressedScaleSprite == nullptr)
		{
			m_pressedScaleSprite = Scale9Sprite::create(strNormal.c_str());
		}
	}
	else
	{
		SpriteFrame  *   selFrame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(strSel.c_str());
		if (selFrame)
		{
			m_pressedScaleSprite = Scale9Sprite::createWithSpriteFrame(selFrame);
		}
		if (m_pressedScaleSprite == nullptr)
		{
			m_pressedScaleSprite = Scale9Sprite::create(strSel.c_str());
		}
	}

	if (m_pressedScaleSprite)
	{
		m_pressedScaleSprite->setContentSize(rectSize);
		m_pressedScaleSprite->setAnchorPoint(Vec2::ZERO);
		m_pressedScaleSprite->setPosition(Vec2::ZERO);
		this->addChild(m_pressedScaleSprite);
	}
	//disabel
	if (strDisable.empty())
	{
		SpriteFrame  *   normalFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strNormal.c_str());
		if (normalFrame)
		{
			m_disableScaleSprite = Scale9Sprite::createWithSpriteFrame(normalFrame);
		}
		if (m_disableScaleSprite == nullptr)
		{
			m_disableScaleSprite = Scale9Sprite::create(strNormal.c_str());
		}
		//如果是用  normal处理的，那么灰化
		if (m_disableScaleSprite)
		{
			setSpriteGray(m_disableScaleSprite, true);
		}
	}
	else
	{
		SpriteFrame  *   disableFrame = SpriteFrameCache::getInstance()->spriteFrameByName(strDisable.c_str());
		if (disableFrame)
		{
			m_disableScaleSprite = Scale9Sprite::createWithSpriteFrame(disableFrame);
		}
		if (m_disableScaleSprite == nullptr)
		{
			m_disableScaleSprite = Scale9Sprite::create(strDisable.c_str());
		}
	}
	if (m_disableScaleSprite)
	{
		m_disableScaleSprite->setContentSize(rectSize);
		m_disableScaleSprite->setAnchorPoint(Vec2::ZERO);
		m_disableScaleSprite->setPosition(Vec2::ZERO);
		this->addChild(m_disableScaleSprite);
	}
	//显示正常状态
	this->setUnSelected();
	return true;
}

bool  TTButton::initWithImage(std::string  strNormal, Size  rectSize)
{
	return  this->initWithImages(strNormal, "", "", rectSize);
}

void  TTButton::upDateImageForState(BTN_STATE  btnState, Texture2D *texture)
{
	if (texture == nullptr)
	{
		return;
	}

	if (btnState == BTN_STATE_NORMAL)
	{
		if (m_normalSprite)
		{
			m_normalSprite->setTexture(texture);
		}
	}

	if (btnState == BTN_STATE_PRESS)
	{
		if (m_pressedSprite)
		{
			m_pressedSprite->setTexture(texture);
		}
	}

	if (btnState == BTN_STATE_DISABLE)
	{
		if (m_disableSprite)
		{
			m_disableSprite->setTexture(texture);
		}
	}
}

TTButton *TTButton::createWithImage(std::string  strNormal)
{
	TTButton *  pRet = new  TTButton();
	if (pRet  &&  pRet->initWithImage(strNormal))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
	return  pRet;
}

TTButton *TTButton::createWithImages(std::string  strNormal, std::string strSel, std::string strDisable)
{
	TTButton *  pRet = new  TTButton();
	if (pRet  &&  pRet->initWithImages(strNormal, strSel, strDisable))
	{
		pRet->autorelease();
	}
	else
	{
		delete  pRet;
		pRet = nullptr;
		return nullptr;
	}
	return  pRet;
}

TTButton *TTButton::createWithImage(std::string  strNormal, Size   rectSize)
{
	TTButton * pRet = new TTButton();
	if (pRet && pRet->initWithImage(strNormal, rectSize))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
	return pRet;
}

TTButton *TTButton::createWithImages(std::string  strNormal, std::string strSel, std::string strDisable, Size   rectSize)
{
	TTButton  * pRet = new  TTButton;
	if (pRet && pRet->initWithImages(strNormal, strSel, strDisable, rectSize))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
	return pRet;
}

void  TTButton::setColorForState(BTN_STATE btnState, Color3B  color)
{
	switch (btnState)
	{
	case BTN_STATE_NORMAL:
	{
		if (m_normalSprite)
		{
			m_normalSprite->setColor(color);
		}

		if (m_normalScaleSprite)
		{
			m_normalScaleSprite->setColor(color);
		}
	}
	break;
	case  BTN_STATE_PRESS:
	{
		if (m_pressedSprite)
		{
			m_pressedSprite->setColor(color);
		}
		if (m_pressedScaleSprite)
		{
			m_pressedScaleSprite->setColor(color);
		}
	}
	break;
	case   BTN_STATE_DISABLE:
	{
		if (m_disableSprite)
		{
			m_disableSprite->setColor(color);
		}

		if (m_disableScaleSprite)
		{
			m_disableScaleSprite->setColor(color);
		}
	}
	break;

	default:
		break;
	}
}

void  TTButton::upDateStatusUI()
{
	switch (m_btnState)
	{
	case BTN_STATE_PRESS:
	{
		if (m_normalSprite)
		{
			m_normalSprite->setVisible(false);
		}
		if (m_normalScaleSprite)
		{
			m_normalScaleSprite->setVisible(false);
		}

		if (m_disableSprite)
		{
			m_disableSprite->setVisible(false);
		}
		if (m_disableScaleSprite)
		{
			m_disableScaleSprite->setVisible(false);
		}

		if (m_pressedSprite)
		{
			m_pressedSprite->setVisible(true);
		}
		if (m_pressedScaleSprite)
		{
			m_pressedScaleSprite->setVisible(true);
		}
	}
		break;

	case  BTN_STATE_NORMAL:
	{
		if (m_normalSprite)
		{
			m_normalSprite->setVisible(true);
		}
		if (m_normalScaleSprite)
		{
			m_normalScaleSprite->setVisible(true);
		}
		if (m_disableSprite)
		{
			m_disableSprite->setVisible(false);
		}
		if (m_disableScaleSprite)
		{
			m_disableScaleSprite->setVisible(false);
		}
		if (m_pressedSprite)
		{
			m_pressedSprite->setVisible(false);
		}
		if (m_pressedScaleSprite)
		{
			m_pressedScaleSprite->setVisible(false);
		}
	}
		break;

	case  BTN_STATE_DISABLE:
	{
		if (m_normalSprite)
							   {
			m_normalSprite->setVisible(false);
		}
		if (m_normalScaleSprite)
		{
			m_normalScaleSprite->setVisible(false);
		}
		if (m_disableSprite)
		{
			m_disableSprite->setVisible(true);
		}
		if (m_disableScaleSprite)
		{
			m_disableScaleSprite->setVisible(true);
		}
		if (m_pressedSprite)
		{
			m_pressedSprite->setVisible(false);
		}
		if (m_disableScaleSprite)
		{
			m_disableScaleSprite->setVisible(false);
		}
	}
		break;

	default:
		break;
	}
}

void  TTButton::setSelected()
{
	m_bSelected = true;
	m_btnState = BTN_STATE_NORMAL;
	upDateStatusUI();
}

void  TTButton::setUnSelected()
{
	m_bSelected = false;
	m_btnState = BTN_STATE_PRESS;
	upDateStatusUI();
}

void  TTButton::setIsDisable(bool  bDisabel)
{
	m_bDisabled = bDisabel;
	if (bDisabel)
	{
		m_btnState = BTN_STATE_DISABLE;
	}
	else
	{
		m_btnState = BTN_STATE_NORMAL;
	}
	upDateStatusUI();
}

BTN_STATE  TTButton::getCurState()
{
	return   m_btnState;
}

void TTButton::addTouchupInsideHandle(Node  * target, SEL_CallFuncO selector)
{
	m_target = target;
	m_touchupInsideSelector = selector;
}

bool TTButton::isTouchInvalidated(Touch* touch)
{
	Vec2 touchLocation = touch->getLocation();
	Vec2 localLocation = convertToNodeSpace(touchLocation);
	Rect   rc = Rect(
		0,
		0,
		getContentSize().width,
		getContentSize().height
		);
	if (rc.containsPoint(localLocation))
	{
		return  true;
	}
	else
	{
		return  false;
	}
}

void TTButton::onEnter()
{
	Node::onEnter();
	//touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(TTButton::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(TTButton::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TTButton::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(TTButton::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
}

void TTButton::onExit()
{
	Node::onExit();
}

bool TTButton::onTouchBegan(Touch *touch, Event *unused_event)
{
	//如果是disabled的状态，那么就返回false
	if (m_btnState == BTN_STATE_DISABLE)
	{
		return  false;
	}
	//如果自己不可见，那么返回false
	if (isVisible() == false)
	{
		return  false;
	}
	//如果有parent不可见，那么返回false
	Node  * parent = nullptr;
	for (parent = this->getParent(); parent != nullptr; parent = parent->getParent())
	{
		if (parent->isVisible() == false)
		{
			return  false;
		}
	}
	//然后判断是否是在自己的范围内
	if (isTouchInvalidated(touch))
	{
		if (m_bSelected)
		{
			m_btnState = BTN_STATE_NORMAL;
		}
		else
		{
			m_btnState = BTN_STATE_PRESS;
			//todo 添加动画
			addAnimation(m_pressedSprite);

		}
		upDateStatusUI();
		return  true;
	}

	return  false;
}
void TTButton::onTouchMoved(Touch *touch, Event *unused_event)
{
	//如果是有效范围内，那么设置selected状态
	if (isTouchInvalidated(touch))
	{
		if (m_bSelected)
		{
			m_btnState = BTN_STATE_NORMAL;
		}
		else
		{
			m_btnState = BTN_STATE_PRESS;
		}
		upDateStatusUI();
	}
	else
	{
		if (m_bSelected)
		{
			m_btnState = BTN_STATE_PRESS;
		}
		else
		{
			m_btnState = BTN_STATE_NORMAL;
		}
		upDateStatusUI();
	}
}

void TTButton::onTouchEnded(Touch *touch, Event *unused_event)
{
	//然后判断是否是在自己的范围内
	if (isTouchInvalidated(touch))
	{
		/*if (m_bSelected)
		{
			setUnSelected();
		}
		else
		{
			setSelected();
		}*/
		if (m_target  && m_touchupInsideSelector)
		{
			(m_target->*m_touchupInsideSelector)(this);
		}
	}
}

void TTButton::onTouchCancelled(Touch *touch, Event *unused_event)
{
	if (m_btnState != BTN_STATE_DISABLE)
	{
		//this->setUnSelected();
	}
}