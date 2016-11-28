#pragma once 

#include "Global.h"
#include "LayerEffect/LayerEffect.h"

class StopLayer :public EasePop
{
public:
	CREATE_FUNC(StopLayer);
	bool init();
	void	newGameCallBack();
	void	musicCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type);
	void	soundCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type);
	void	vibrationCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type);
private:
	void addUI();
};